/* Copyright 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/dutils.h"
#include "../bignbr.h"

#define TESTS_AMOUNT 3
#define TESTS_FAIL 0
#define TESTS_PASS 1

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */

/*
	Function: tst_print_success (char* tstname);
	Description: Prints the test success message.
	InitVersion: 0.0.1
*/
static void tst_print_success (char* tstname)
{
	printf ("%s:", tstname);
	
	if (strlen (tstname) < 15)
	{
		printf ("\t\t\t");
	}
	else if (strlen (tstname) < 23)
	{
		printf ("\t\t");
	}
	else
	{
		printf ("\t");
	}
	
	printf ("%sSUCCESS%s\n", BC_BLD_GREEN, BC_TXT_RST);
}

/*
	Function: tst_print_fail (char* tstname);
	Description: Prints the test failure message.
	InitVersion: 0.0.1
*/
static void tst_print_fail (char* tstname)
{
	printf ("%s:", tstname);
	
	if (strlen (tstname) < 15)
	{
		printf ("\t\t\t");
	}
	else if (strlen (tstname) < 23)
	{
		printf ("\t\t");
	}
	else
	{
		printf ("\t");
	}
	
	printf ("%sFAIL%s\n", BC_BLD_RED, BC_TXT_RST);
}

/*
	Function: tst_print_summary (int points);
	Description: Prints the summary of all tests.
	InitVersion: 0.0.1
*/
static void tst_print_summary (int points)
{
	printf ("\n+=======================+\n");
	printf ("|  Summary of all tests |\n");
	printf ("+=======================+\n");
	printf ("|  Tests: \t%i\t|\n", TESTS_AMOUNT);
	printf ("+-----------------------+\n");
	printf ("|  Passed:\t%i\t|\n", points);
	printf ("|  Failed:\t%i\t|\n", TESTS_AMOUNT-points);
	printf ("+-----------------------+\n");
}

/* |--------------------------------------------|
   |		    Tests-Int			|
   |--------------------------------------------| */

/*
	Function: bignbr_test_int_add (void);
	Description: Tests the bignbr_add function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_int_add (void)
{
	short passed;
	bignbr a, b;
	
	/* Add positive to positive. */
	bignbr_init (&a, 100, "+204242999999999999999999");
	bignbr_init (&b, 100, "+1");
	
	bignbr_add (&a, &b);
	
	if (bignbr_cmp_str (&a, "+204243000000000000000000"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Add positive to negative. */
	bignbr_fill (&a, "-204242999999999999999999");
	bignbr_fill (&b, "+1");
	
	bignbr_add (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-204242999999999999999998"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Add negative to positive. */
	bignbr_fill (&a, "+204242999999999999999999");
	bignbr_fill (&b, "-1");
	
	bignbr_add (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "+204242999999999999999998"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Add negative to negative. */
	bignbr_fill (&a, "-204242999999999999999999");
	bignbr_fill (&b, "-1");
	
	bignbr_add (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-204243000000000000000000"))
	{
		tst_print_success ("INT_Addition");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("INT_Addition");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
}

/*
	Function: bignbr_test_int_sub (void);
	Description: Tests the bignbr_sub function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_int_sub (void)
{
	short passed;
	bignbr a, b;
	
	/* Subtract positive from positive. */
	bignbr_init (&a, 100, "+204242999999999999999999");
	bignbr_init (&b, 100, "+1");
	
	bignbr_sub (&a, &b);
	
	if (bignbr_cmp_str (&a, "+204242999999999999999998"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Subtract positive from negative. */
	bignbr_fill (&a, "-204242999999999999999999");
	bignbr_fill (&b, "+1");
	
	bignbr_sub (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-204243000000000000000000"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Subtract negative from positive. */
	bignbr_fill (&a, "+204242999999999999999999");
	bignbr_fill (&b, "-1");
	
	bignbr_sub (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "+204243000000000000000000"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Subtract negative from negative. */
	bignbr_fill (&a, "-204242999999999999999999");
	bignbr_fill (&b, "-1");
	
	bignbr_sub (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-204242999999999999999998"))
	{
		tst_print_success ("INT_Subtraction");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("INT_Subtraction");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
}

/*
	Function: bignbr_test_int_mpl (void);
	Description: Tests the bignbr_mpl function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_int_mpl (void)
{
	short passed;
	bignbr a, b;
	
	/* Multiply positive with positive. */
	bignbr_init (&a, 100, "+200050008");
	bignbr_init (&b, 100, "+3");
	
	bignbr_mpl (&a, &b);
	
	if (bignbr_cmp_str (&a, "+600150024"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Multiply positive with negative. */
	bignbr_fill (&a, "-200050008");
	bignbr_fill (&b, "+3");
	
	bignbr_mpl (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-600150024"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Multiply negative with positive. */
	bignbr_fill (&a, "+200050008");
	bignbr_fill (&b, "-3");
	
	bignbr_mpl (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-600150024"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Add negative to negative. */
	bignbr_fill (&a, "-200050008");
	bignbr_fill (&b, "-3");
	
	bignbr_mpl (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "+600150024"))
	{
		tst_print_success ("INT_Multiplication");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("INT_Multiplication");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
}

int main (int argc, char* argv[])
{
	int points;
	
	points = 0;
	
	printf ("BigNbr v. 0.0.1 A (c) 2016 Marc Volker Dickmann\n\n");
	
	points += bignbr_test_int_add ();
	points += bignbr_test_int_sub ();
	points += bignbr_test_int_mpl ();
	
	tst_print_summary (points);
	
	return 0;
}
