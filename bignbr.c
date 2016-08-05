/* Copyright 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bignbr.h"

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

void bignbr_init (bignbr *a, unsigned int len, unsigned char *v)
{
 	a->len = len;
  	
  	/* Length + 2 Bytes : 1 Byte for sign and 1 byte for the binary null. */
  	a->data = (unsigned char*) malloc (sizeof (unsigned char) * (a->len+2));
  
	bignbr_fill (a, v);
}

void bignbr_free (bignbr *a)
{
	free (a->data);
}

unsigned int bignbr_get_eon_pos (bignbr *a)
{
	int i;
	
	for (i = 1; i < a->len; i++)
	{
		if (a->data[i] == BIGNBR_EON)
		{
			break;
		}
	}
	
	return i-1;
}

void bignbr_print (bignbr *a)
{
	int i;
	
	printf ("%c", a->data[BIGNBR_SIGN]);
	
	for (i = bignbr_get_eon_pos (a); i > 0; i--)
	{	
		printf ("%c", (char)(a->data[i] + '0'));
	}
	
	printf ("\n");
}

void bignbr_cpy (bignbr *a, bignbr *b)
{
	int i;

	a->len = b->len;
	
	for (i = 0; i < a->len; i++)
	{
		a->data[i] = b->data[i];
	}
}

bool bignbr_cmp_str (bignbr *a, unsigned char *v)
{
	int i, j;
	
	if (strlen (v) > a->len ||
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
	int i;
	  
	for (i = a->len-1; i > 0; i--)
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
	int i;
	
	if (bignbr_is_negative (a) &&
	   !bignbr_is_negative (b))
	{
		return false;
	}
	else if (!bignbr_is_negative (a) &&
		  bignbr_is_negative (b))
	{
		return true;
	}
	
	if (a->len != b->len)
	{
		printf ("Error: A and B must have the same length!\n");
		return false;
	}
	
	for (i = a->len-1; i > 0; i--)
	{
		if (a->data[i] > b->data[i])
		{
			return false;
		}
	}
	
	return false;
}

void bignbr_add (bignbr *a, bignbr *b)
{
	int i;
	char t;
	char carry;
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
		if (a->data[i] == BIGNBR_EON)
		{
			end_a = true;
			a->data[i] = 0;
		}
		
		if (b->data[i] == BIGNBR_EON)
		{
			end_b = true;
			b->data[i] = 0;
		}
		
		if (carry == 0 && end_a == true && end_b == true)
		{
			a->data[i] = BIGNBR_EON;
			break;
		}
		
		t = a->data[i] + (state_a ^ state_b ? (-b->data[i]) : b->data[i]) + carry;
		
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
