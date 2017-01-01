/* Copyright 2016 - 2017 Marc Volker Dickmann */
#include <stdio.h>
#include <stdbool.h>
#include "../../bignbr.h"

int main (int argc, char *argv[])
{
	bignbr a, b, limit, tmp;
	
	bignbr_init (&a, 100, "+10");
	bignbr_init (&b, 100, "+98");
	bignbr_init (&limit, 100, "+10");
	bignbr_init (&tmp, 100, "+1");
	
	/* Calculate 10^99 (100 digits) since bignbr doesn't currently 
	   have a pow function! */
	while (!bignbr_is_null (&b))
	{
		bignbr_mpl (&limit, &a);
		bignbr_sub (&b, &tmp);
	}
	
	/* Calculate the smallest Fibonacci number with 100 digits! */
	bignbr_fill (&a, "+0");
	bignbr_fill (&b, "+1");
	bignbr_fill (&tmp, "+0");
	
	while (bignbr_is_greater (&limit, &a))
	{
		bignbr_add (&a, &b);
		
		/* Swap A with B! */
		bignbr_cpy (&tmp, &a);
		bignbr_cpy (&a, &b);
		bignbr_cpy (&b, &tmp);
	}
	
	printf ("The smallest Fibonacci number with 100 digits is: ");
	bignbr_print (&a);
	
	/* Cleanup */
	bignbr_free (&a);
	bignbr_free (&b);
	bignbr_free (&limit);
	bignbr_free (&tmp);
	
	return 0;
}
