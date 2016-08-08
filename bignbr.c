/* Copyright 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bignbr.h"

/* |--------------------------------------------|
   |		    Core			|
   |--------------------------------------------| */

void bignbr_init (bignbr *a, unsigned int len, unsigned char *v)
{
	/* Amount of digits + 2 bytes: 1 for the sign + 1 for the eon. */
 	a->len = len + 2;
  	
  	/* Length + 1 byte : 1 byte for the binary null. */
  	a->data = (unsigned char*) malloc (sizeof (unsigned char) * (a->len + 1));
  
	bignbr_fill (a, v);
}

void bignbr_free (bignbr *a)
{
	free (a->data);
}

void bignbr_cpy (bignbr *a, bignbr *b)
{
	unsigned int i;
	unsigned char vb;
	bool end_b;
	
	end_b = false; 
	
	for (i = 0; i < a->len; i++)
	{
		vb = (end_b ? 0 : b->data[i]);
		
		if (vb == BIGNBR_EON)
		{
			end_b = true;
		}
		
		a->data[i] = vb;
	}
}

void bignbr_fill (bignbr *a, unsigned char *v)
{
	unsigned int i;
	int j;
	
	j = strlen (v) - 1;
		
	for (i = 1; i < a->len; i++, j--)
	{	
		a->data[i] = (j > 0 ? v[j] - '0' : 0);
	}
	
	/* Set the sign, eon and binary null. */
	a->data[strlen (v)] = BIGNBR_EON;
	a->data[BIGNBR_SIGN] = v[BIGNBR_SIGN];
	a->data[a->len] = '\0';
}

void bignbr_print (bignbr *a)
{
	unsigned int i;
	
	printf ("%c", a->data[BIGNBR_SIGN]);
	
	i = bignbr_get_eon_pos (a);
	for (i--; i > 0; i--)
	{	
		printf ("%c", a->data[i] + '0');
	}
	
	printf ("\n");
}

unsigned int bignbr_get_eon_pos (bignbr *a)
{
	unsigned int i;
	
	for (i = 1; i < a->len; i++)
	{
		if (a->data[i] == BIGNBR_EON)
		{
			break;
		}
	}
	
	return i;
}

/* |--------------------------------------------|
   |		    Check			|
   |--------------------------------------------| */

bool bignbr_cmp_str (bignbr *a, unsigned char *v)
{
	unsigned int i;
	int j;
	
	if (strlen (v) > a->len ||
	    bignbr_get_eon_pos (a) != strlen (v) ||
	    a->data[BIGNBR_SIGN] != v[BIGNBR_SIGN])
	{
		return false;
	}
		
	j = strlen (v) - 1;
	
	for (i = 1; ; i++, j--)
	{
		if (a->data[i] == BIGNBR_EON || j < 1)
		{
			break;
		}
		else if (a->data[i] + '0' != v[j])
		{
			return false;
		}
	}
	
	return true;
}

bool bignbr_is_null (bignbr *a)
{
	unsigned int i;
	  
	for (i = 1; i < a->len; i++)
	{
		if (BIGNBR_GETNBR(a->data[i]) != 0)
		{
			return false;
		}
	}
	
	return true;
}

void bignbr_set_negative (bignbr *a, bool v)
{
	a->data[BIGNBR_SIGN] = (v ? '-' : '+');
}

bool bignbr_is_negative (bignbr *a)
{	
	return a->data[BIGNBR_SIGN] == '-';
}

bool bignbr_is_greater (bignbr *a, bignbr *b)
{
	unsigned int i;
	bool state_a, state_b;

	state_a = bignbr_is_negative (a);
	state_b = bignbr_is_negative (b);
	
	/* If one of the numbers (or both) equals null, this gives the answer! */
	if (bignbr_is_null (a) ||
	    bignbr_is_null (b))
	{
		return (!bignbr_is_null (a) && !state_a) ^ (!bignbr_is_null (b) && state_b);
	}
	
	/* Unequal signs only need this! */
	if (state_a ^ state_b)
	{
		return !state_a && state_b;
	}
	
	/* If A and B are from unequal length, this gives the answer. */
	i = bignbr_get_eon_pos (a);
	if (i != bignbr_get_eon_pos (b))
	{
		return state_a ^ i > bignbr_get_eon_pos (b);
	}
	
	/* NOTE: i is equal for A and B. */
	for (i--; i > 0; i--)
	{
		/* Both tests needed due to return false on equal numbers! */
		if ((state_a && BIGNBR_GETNBR (a->data[i]) < BIGNBR_GETNBR (b->data[i])) ||
		    (!state_a && BIGNBR_GETNBR (a->data[i]) > BIGNBR_GETNBR (b->data[i])))
		{
			return true;
		}
	}
	
	return false;
}

/* |--------------------------------------------|
   |		    Arithmetic			|
   |--------------------------------------------| */


void bignbr_add (bignbr *a, bignbr *b)
{
	unsigned int i;
	char t, carry, va, vb;
	bool state_a, state_b, end_a, end_b, sw;
	
	carry = 0;
	
	state_a = bignbr_is_negative (a);
	state_b = bignbr_is_negative (b);
	end_a = false;
	end_b = false;
	
	/* Use for the case of B being negative and greater than A! */
	bignbr_set_negative (a, false);
	bignbr_set_negative (b, false);
	sw = bignbr_is_greater (b, a) && (state_b ^ state_a);
	bignbr_set_negative (a, state_a);
	bignbr_set_negative (b, state_b);
	
	for (i = 1; i < a->len; i++)
	{
		va = (end_a ? 0 : a->data[i]);
		/* B doesn't change so we need this. */
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
			a->data[i] = (unsigned char) (t + 10);
			carry = -1;
		}
		else if (t >= 10)
		{
			a->data[i] = (unsigned char) (t - 10);
			carry = 1;
		}
		else
		{
			a->data[i] = (unsigned char) (t);
			carry = 0;
		}
	}
	
	/* Sets the EON after the last digit. */
	for (i = a->len; i > 0; i--)
	{
		if (a->data[i] != 0)
		{
			a->data[i+1] = BIGNBR_EON;
			break;
		}
	}
	
	if (sw)
	{
		bignbr_set_negative (a, state_b);
	}
}

void bignbr_sub (bignbr *a, bignbr *b)
{
	bool state_a, state_b;
	
	state_a = bignbr_is_negative (a);
	state_b = bignbr_is_negative (b);
	
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

void bignbr_mpl (bignbr *a, bignbr *b)
{
	bool state_a, state_b;
	bignbr out, tmp, cb;
	
	bignbr_init (&out, a->len, "+0");
	bignbr_init (&tmp, 1, "+1");
	
	/* Make a copy of B since it doesn't change. */
	bignbr_init (&cb, b->len, "+0");
	bignbr_cpy (&cb, b);
	
	state_a = bignbr_is_negative (a);
	state_b = bignbr_is_negative (b);
	
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
