/* Copyright 2016 - 2017 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bignbr.h"

/* |--------------------------------------------|
   |		    Core			|
   |--------------------------------------------| */

void
bignbr_init (bignbr *a, const size_t len, const char *v)
{
	// Amount of digits + 2 bytes: 1 for the sign + 1 for the eon.
	a->len = len + 2;
	
  	// Length + 1 byte : 1 byte for the binary null.
	a->data = malloc (sizeof (char) * (a->len + 1));
	
	bignbr_fill (a, v);
}

void
bignbr_free (bignbr *a)
{
	free (a->data);
}

void
bignbr_cpy (bignbr *a, const bignbr *b)
{
	bool end_b = false; 
	
	// Start at zero to include the sign.
	for (size_t i = 0; i < a->len; i++)
	{
		a->data[i] = (end_b ? 0 : b->data[i]);
		
		if (a->data[i] == BIGNBR_EON)
		{
			end_b = true;
		}
	}
}

void
bignbr_cat_digit (bignbr *a, const char v)
{
	size_t i = bignbr_get_eon_pos (a);
	
	a->data[i + 1] = BIGNBR_EON;
	
	for (; i > 1; i--)
	{
		a->data[i] = a->data[i - 1];
	}
	a->data[1] = v;
}

void
bignbr_fill (bignbr *a, const char *v)
{	
	int j = strlen (v) - 1;
		
	for (size_t i = 1; i < a->len; i++, j--)
	{	
		a->data[i] = (j > 0 ? v[j] - '0' : 0);
	}
	
	// Set the sign, eon and binary null.
	a->data[strlen (v)] = BIGNBR_EON;
	a->data[BIGNBR_SIGN] = v[BIGNBR_SIGN];
	a->data[a->len] = '\0';
}

void
bignbr_print (const bignbr *a)
{
	printf ("%c", a->data[BIGNBR_SIGN]);
	
	size_t i = bignbr_get_eon_pos (a);
	for (i--; i > 0; i--)
	{	
		printf ("%c", a->data[i] + '0');
	}
	
	printf ("\n");
}

size_t
bignbr_get_eon_pos (const bignbr *a)
{
	for (size_t i = 1; i < a->len; i++)
	{
		if (a->data[i] == BIGNBR_EON)
		{
			return i;
		}
	}
	// EON should never be 0, indicates an error!
	return 0;
}

/* |--------------------------------------------|
   |		    Check			|
   |--------------------------------------------| */

bool
bignbr_cmp_str (const bignbr *a, const char *v)
{
	size_t i = bignbr_get_eon_pos (a);
	const size_t j = strlen (v);
	
	if (i != j ||
	    a->data[BIGNBR_SIGN] != v[BIGNBR_SIGN])
	{
		return false;
	}
	
	for (i--; i > 0; i--)
	{
		if (a->data[i] + '0' != v[j - i])
		{
			return false;
		}
	}
	
	return true;
}

bool
bignbr_is_null (const bignbr *a)
{
	// Zeros are always saved in this format!
	return a->data[1] == 0 && a->data[2] == BIGNBR_EON;
}

void
bignbr_set_negative (bignbr *a, const bool v)
{
	a->data[BIGNBR_SIGN] = (v && !bignbr_is_null (a) ? '-' : '+');
}

bool
bignbr_is_negative (const bignbr *a)
{	
	return a->data[BIGNBR_SIGN] == '-';
}

bool
bignbr_is_greater (const bignbr *a, const bignbr *b)
{
	const bool state_a = bignbr_is_negative (a);
	const bool state_b = bignbr_is_negative (b);
	
	// If one of the numbers (or both) equals null, this gives the answer!
	if (bignbr_is_null (a) ||
	    bignbr_is_null (b))
	{
		return (!bignbr_is_null (a) && !state_a) ^ (!bignbr_is_null (b) && state_b);
	}
	
	// Unequal signs only need this!
	if (state_a ^ state_b)
	{
		return !state_a && state_b;
	}
	
	// If A and B are from unequal length, this gives the answer.
	size_t i = bignbr_get_eon_pos (a);
	const size_t j = bignbr_get_eon_pos (b);
	if (i != j)
	{
		return state_a ^ (i > j);
	}
	
	// NOTE: i is equal for A and B.
	for (i--; i > 0; i--)
	{
		// If they are unequal, return this!
		if (a->data[i] != b->data[i])
		{
			return state_a ^ (a->data[i] > b->data[i]);
		}
	}
	
	// Return false because they are equal.
	return false;
}

/* |--------------------------------------------|
   |		    Arithmetic			|
   |--------------------------------------------| */

void
bignbr_add (bignbr *a, bignbr *b)
{
	const bool state_a = bignbr_is_negative (a);
	const bool state_b = bignbr_is_negative (b);
	
	// Use for the case of B being negative and greater than A!
	bignbr_set_negative (a, false);
	bignbr_set_negative (b, false);
	const bool sw = bignbr_is_greater (b, a) && (state_a ^ state_b);
	bignbr_set_negative (a, state_a);
	bignbr_set_negative (b, state_b);
	
	size_t i = 1;
	char t, va, vb;
	char carry = 0;
	bool end_a = false;
	bool end_b = false;
	
	for (; i < a->len; i++)
	{
		va = (end_a ? 0 : a->data[i]);
		// B doesn't change so we need this.
		vb = (end_b ? 0 : b->data[i]);
		
		if (va == BIGNBR_EON)
		{
			end_a = true;
			va = 0;
		}
		
		if (vb == BIGNBR_EON)
		{
			end_b = true;
			vb = 0;
		}
		
		if (sw)
		{
			t = va;
			va = vb;
			vb = t;
		}
		
		t = va + (state_a ^ state_b ? (-vb) : vb) + carry;
		
		if (t < 0)
		{
			a->data[i] = t + 10;
			carry = -1;
		}
		else if (t >= 10)
		{
			a->data[i] = t - 10;
			carry = 1;
		}
		else
		{
			a->data[i] = t;
			carry = 0;
		}
	}
	
	// Sets the EON after the last digit.
	for (i = a->len - 1; i > 0; i--)
	{
		if (a->data[i] != 0)
		{
			a->data[i + 1] = BIGNBR_EON;
			break;
		}
		else if (i == 1)
		{
			// If the number equals zero, make it positive!
			a->data[i + 1] = BIGNBR_EON;
			bignbr_set_negative (a, false);
			break;
		}
	}
	
	if (sw)
	{
		bignbr_set_negative (a, state_b);
	}
}

void
bignbr_sub (bignbr *a, bignbr *b)
{	
	const bool state_a = bignbr_is_negative (a);
	const bool state_b = bignbr_is_negative (b);
	
	if (!state_a && !state_b)
	{
		bignbr_set_negative (b, true);
		bignbr_add (a, b);
		bignbr_set_negative (b, false);
	}
	else if (state_b)
	{
		bignbr_set_negative (b, false);
		bignbr_add (a, b);
		bignbr_set_negative (b, true);
	}
	else if (state_a)
	{
		bignbr_set_negative (a, false);
		bignbr_add (a, b);
		bignbr_set_negative (a, true);
	}
}

void
bignbr_mpl (bignbr *a, bignbr *b)
{
	bignbr out, tmp, cb;
	
	// Since a->len would be len + 2, subtract 2 to get equal length.
	bignbr_init (&out, a->len - 2, "+0");
	bignbr_init (&tmp, 1, "+1");
	
	// Make a copy of B since it doesn't change.
	bignbr_init (&cb, b->len - 2, "+0");
	bignbr_cpy (&cb, b);
	
	const bool state_a = bignbr_is_negative (a);
	const bool state_b = bignbr_is_negative (b);
	
	bignbr_set_negative (&tmp, state_b);
	bignbr_set_negative (&out, state_a);
	
	while (!bignbr_is_null (&cb))
	{
		bignbr_add (&out, a);
		bignbr_sub (&cb, &tmp);
	}
	
	bignbr_set_negative (&out, state_a ^ state_b);
	bignbr_cpy (a, &out);
	
	bignbr_free (&out);
	bignbr_free (&tmp);
	bignbr_free (&cb);
}

void
bignbr_div (bignbr *a, bignbr *b, bignbr *r, bignbr *p)
{
	if (bignbr_is_null (a) ||
	    bignbr_is_null (b))
	{
		// Division through zero!
		return;
	}
	
	const bool state_a = bignbr_is_negative (a);
	const bool state_b = bignbr_is_negative (b);
	
	// Make all positive in order to work correctly!
	bignbr_set_negative (a, false);
	bignbr_set_negative (b, false);
	
	bignbr out, tmp, one, cp;
	bignbr_init (&out, a->len - 2, "+");
	bignbr_init (&tmp, a->len - 2, "+");
	bignbr_init (&one, 1, "+1");
	bignbr_init (&cp, p->len - 2, "+0");
	
	size_t i = bignbr_get_eon_pos (a) - 1;
	char va, j;
	bool end_a = false;

	// Divide until the precision P is reached.	
	for (bignbr_cpy (&cp, p); !bignbr_is_null (&cp); bignbr_sub (&cp, &one))
	{
		// Generate the part of A.
		va = (end_a ? 0 : a->data[i]);
		
		if (i == 0)
		{
			end_a = true;
			va = 0;
		}
		
		bignbr_cat_digit (&tmp, va);
		
		if (!end_a)
		{
			i--;
		}
		
		// Generate and append the next digit.
		j = 0;
		while (!bignbr_is_null (&tmp) &&
		       !bignbr_is_greater (b, &tmp))
		{
			bignbr_sub (&tmp, b);
			j++;
		}
		
		bignbr_cat_digit ((end_a ? r : &out), j);
		
		// Break if no more digits are left or reset tmp if its zero.
		if (end_a &&
		    bignbr_is_null (&tmp))
		{
			break;
		}
		else if (bignbr_is_null (&tmp))
		{
			bignbr_fill (&tmp, "+");
		}
	}
	
	// Sets the EON after the last digit if its unequal to zero.
	if (!bignbr_is_null (&out))
	{
		for (i = out.len - 1; i > 0; i--)
		{
			if (out.data[i] == BIGNBR_EON)
			{
				out.data[i] = 0;
			}
			else if (out.data[i] != 0)
			{
				out.data[i + 1] = BIGNBR_EON;
				break;
			}
		}
	}
	
	bignbr_cpy (a, &out);
	
	// Set the sign for the numbers.
	bignbr_set_negative (a, state_a ^ state_b);
	bignbr_set_negative (b, state_b);
	bignbr_set_negative (r, state_a ^ state_b);
	
	bignbr_free (&out);
	bignbr_free (&tmp);
	bignbr_free (&one);
	bignbr_free (&cp);
}
