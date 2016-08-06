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
 	a->len = len;
  	
  	/* Length + 1 Bytes : 1 byte for the binary null. */
  	a->data = (unsigned char*) malloc (sizeof (unsigned char) * (a->len+1));
  
	bignbr_fill (a, v);
}

void bignbr_free (bignbr *a)
{
	free (a->data);
}

void bignbr_cpy (bignbr *a, bignbr *b)
{
	unsigned int i;

	a->len = b->len;
	
	for (i = 0; i < a->len; i++)
	{
		a->data[i] = b->data[i];
	}
}

void bignbr_fill (bignbr *a, unsigned char *v)
{
	int i, j;
	
	for (i = 1; i < a->len; i++)
	{
		j = strlen (v) - i;
		
		/* Copy the first value! */
		if (j > 0)
		{
			/* Substract '0' to get an int like value. */
			a->data[i] = (unsigned char) (v[j] - '0');
		}
		else
		{
			/* Adding leading zeros. */
			a->data[i] = 0;
		}
	}
	
	/* Set the sign. */
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
		printf ("%c", (char)(a->data[i] + '0'));
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
	int i, j;
	
	if (strlen (v) > a->len ||
	    bignbr_get_eon_pos (a) != strlen (v) ||
	    a->data[BIGNBR_SIGN] != v[BIGNBR_SIGN])
	{
		return false;
	}
	
	for (i = 1; ; i++)
	{
		/* Sign before digits! */
		j = strlen (v)-i;
		
		if (a->data[i] == BIGNBR_EON || j < 1)
		{
			break;
		}
		
		if (a->data[i]+'0' != v[j])
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
		if (a->data[i] != 0 && a->data[i] != BIGNBR_EON)
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
	if (a->data[BIGNBR_SIGN] == '-')
	{
		return true;
	}
	
	return false;
}

bool bignbr_is_greater (bignbr *a, bignbr *b)
{
	unsigned int i;
	bool state_a, state_b;
	
	if (a->len != b->len ||
	    (bignbr_is_null (a) &&
	     bignbr_is_null (b)))
	{
		return false;
	}

	state_a = bignbr_is_negative (a);
	state_b = bignbr_is_negative (b);
	
	if (state_a &&
	   !state_b)
	{
		return false;
	}
	else if (!state_a &&
		  state_b)
	{
		return true;
	}
	
	/* If A and B are from unequal length, this gives the answer. */
	i = bignbr_get_eon_pos (a);
	if (i != bignbr_get_eon_pos (b))
	{
		return state_a ^ i > bignbr_get_eon_pos (b);
	}
	
	for (i--; i > 0; i--)
	{
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
	char t, carry, vb;
	bool state_a, state_b, end_a, end_b;
	
	if (a->len != b->len)
	{
		printf ("Error: A and B must have the same length!\n");
		return;
	}
	
	carry = 0;
	
	state_a = bignbr_is_negative (a);
	state_b = bignbr_is_negative (b);
	end_a = false;
	end_b = false;
	
	for (i = 1; i < a->len; i++)
	{
		/* B doesn't change so we need this. */
		vb = (end_b ? 0 : b->data[i]);
		
		if (a->data[i] == BIGNBR_EON)
		{
			end_a = true;
			a->data[i] = 0;
		}
		
		if (vb == BIGNBR_EON)
		{
			end_b = true;
			vb = 0;
		}
		
		if (carry == 0 && end_a && end_b)
		{
			a->data[i] = BIGNBR_EON;
			break;
		}
		
		t = a->data[i] + (state_a ^ state_b ? (-vb) : vb) + carry;
		
		if (t < 0)
		{
			a->data[i] = (unsigned char) (10 + t);
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
	bignbr out, tmp;
	
	bignbr_init (&out, a->len, "+0");
	bignbr_init (&tmp, a->len, "+1");
	
	state_a = bignbr_is_negative (a);
	state_b = bignbr_is_negative (b);
	
	bignbr_set_negative (&tmp, state_b);
	bignbr_set_negative (&out, state_a);
	
	while (!bignbr_is_null (b))
	{
		bignbr_add (&out, a);
		bignbr_sub (b, &tmp);
	}
	
	bignbr_set_negative (&out, state_a ^ state_b);
	bignbr_cpy (a, &out);
	
	bignbr_free (&out);
	bignbr_free (&tmp);
}
