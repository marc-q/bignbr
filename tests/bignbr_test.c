/* Copyright 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/dutils.h"
#include "../bignbr.h"

#define TESTS_AMOUNT 10
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
   |		    Tests-Core			|
   |--------------------------------------------| */

/*
	Function: bignbr_test_core_cpy (void);
	Description: Tests the bignbr_cpy function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_core_cpy (void)
{
	short passed;
	bignbr a, b;
	
	/* Copy a positive to a positive. */
	bignbr_init (&a, 10, "+1005");
	bignbr_init (&b, 4, "+2005");
	
	bignbr_cpy (&a, &b);
	
	if (bignbr_cmp_str (&a, "+2005") &&
	    bignbr_cmp_str (&b, "+2005"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Copy a positive to a negative. */
	bignbr_fill (&a, "-1005");
	bignbr_fill (&b, "+2005");
	
	bignbr_cpy (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "+2005") &&
	    bignbr_cmp_str (&b, "+2005"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Copy a negative to a positive. */
	bignbr_fill (&a, "+1005");
	bignbr_fill (&b, "-2005");
	
	bignbr_cpy (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-2005") &&
	    bignbr_cmp_str (&b, "-2005"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Copy a negative to a negative. */
	bignbr_fill (&a, "-1005");
	bignbr_fill (&b, "-2005");
	
	bignbr_cpy (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-2005") &&
	    bignbr_cmp_str (&b, "-2005"))
	{
		tst_print_success ("CORE_Copy");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("CORE_Copy");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
}

/*
	Function: bignbr_test_core_fill (void);
	Description: Tests the bignbr_fill function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_core_fill (void)
{
	short passed;
	bignbr a;
	
	/* Fill a positive. */
	bignbr_init (&a, 11, "+0");
	
	bignbr_fill (&a, "+01234567890");
	
	if (bignbr_cmp_str (&a, "+01234567890"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Fill a negative. */
	bignbr_fill (&a, "-09876543210");
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-09876543210"))
	{
		tst_print_success ("CORE_Fill");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("CORE_Fill");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	return passed;
}

/*
	Function: bignbr_test_core_geteonpos (void);
	Description: Tests the bignbr_get_eon_pos function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_core_geteonpos (void)
{
	unsigned int pos;
	short passed;
	bignbr a;
	
	/* Get the EON from an positive 1 digit number. */
	bignbr_init (&a, 10, "+0");
	
	pos = bignbr_get_eon_pos (&a);
	
	if (a.data[pos] == BIGNBR_EON)
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Get the EON from an negative 10 digit number. */
	bignbr_fill (&a, "-0987654321");
	
	pos = bignbr_get_eon_pos (&a);
	
	if (passed == TESTS_PASS &&
	    a.data[pos] == BIGNBR_EON)
	{
		tst_print_success ("CORE_GetEonPos");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("CORE_GetEonPos");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	return passed;
}

/* |--------------------------------------------|
   |		    Tests-Check			|
   |--------------------------------------------| */

/*
	Function: bignbr_test_check_cmp_str (void);
	Description: Tests the bignbr_cmp_str function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_check_cmp_str (void)
{
	short passed;
	bignbr a;
	
	/* Compare a positive with a negative number. */
	bignbr_init (&a, 11, "+01234567890");
	
	if (!bignbr_cmp_str (&a, "-01234567890") &&
	     bignbr_cmp_str (&a, "+01234567890") &&
	    !bignbr_cmp_str (&a, "+0123456789") &&
	    !bignbr_cmp_str (&a, "+1234567890") &&
	    !bignbr_cmp_str (&a, "+0") &&
	    !bignbr_cmp_str (&a, "-0"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Compare a negative with a positive number. */
	bignbr_fill (&a, "-01234567890");
	
	if (passed == TESTS_PASS &&
	    !bignbr_cmp_str (&a, "+01234567890") &&
	     bignbr_cmp_str (&a, "-01234567890") &&
	    !bignbr_cmp_str (&a, "-0123456789") &&
	    !bignbr_cmp_str (&a, "-1234567890") &&
	    !bignbr_cmp_str (&a, "+0") &&
	    !bignbr_cmp_str (&a, "-0"))
	{
		tst_print_success ("CHECK_Compare_Str");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("CHECK_Compare_Str");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	return passed;
}

/*
	Function: bignbr_test_check_is_null (void);
	Description: Tests the bignbr_is_null function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_check_is_null (void)
{
	short passed;
	bignbr a;
	
	/* Positive null. */
	bignbr_init (&a, 11, "+0");
	
	if (bignbr_is_null (&a))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Negative null.*/
	bignbr_fill (&a, "-0");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_null (&a))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Positive number. */
	bignbr_fill (&a, "+01234567890");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_null (&a))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Negative number. */
	bignbr_fill (&a, "-01234567890");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_null (&a))
	{
		tst_print_success ("CHECK_Is_Null");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("CHECK_Is_Null");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	return passed;
}

/*
	Function: bignbr_test_check_negative (void);
	Description: Tests the bignbr_is_negative and bignbr_set_negative functions from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_check_negative (void)
{
	short passed;
	bignbr a;
	
	/* Positive null. */
	bignbr_init (&a, 11, "+0");
	
	if (!bignbr_is_negative (&a))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Negative null.*/
	bignbr_set_negative (&a, true);
	
	if (passed == TESTS_PASS &&
	    bignbr_is_negative (&a))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Negative number. */
	bignbr_fill (&a, "-01234567890");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_negative (&a))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Positive number. */
	bignbr_set_negative (&a, false);
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_negative (&a))
	{
		tst_print_success ("CHECK_Negative");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("CHECK_Negative");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	return passed;
}

/*
	Function: bignbr_test_check_is_greater (void);
	Description: Tests the bignbr_is_greater function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_check_is_greater (void)
{
	short passed;
	bignbr a, b;
	
	/* Check two positive nulls. */
	bignbr_init (&a, 3, "+0");
	bignbr_init (&b, 10, "+0");
	
	if (!bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Check two negative nulls. */
	bignbr_fill (&a, "-0");
	bignbr_fill (&b, "-0");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Check a positive with an negative null. */
	bignbr_fill (&a, "+0");
	bignbr_fill (&b, "-0");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Check a negative with an positive null. */
	bignbr_fill (&a, "-0");
	bignbr_fill (&b, "+0");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* NUMBERS */
	
	/* Check two unequal positive numbers. */
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "+50");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+50");
	bignbr_fill (&b, "+128");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Check two unequal negative numbers. */
	bignbr_fill (&a, "-128");
	bignbr_fill (&b, "-50");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-50");
	bignbr_fill (&b, "-128");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Check a positive and an negative number. */
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "-50");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-128");
	bignbr_fill (&b, "+0");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-0");
	bignbr_fill (&b, "-128");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "-128");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-128");
	bignbr_fill (&b, "+128");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "+128");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-128");
	bignbr_fill (&b, "-128");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+120");
	bignbr_fill (&b, "+128");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "+120");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-120");
	bignbr_fill (&b, "-128");
	
	if (passed == TESTS_PASS &&
	    bignbr_is_greater (&a, &b))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-128");
	bignbr_fill (&b, "-120");
	
	if (passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		tst_print_success ("CHECK_Is_Greater");
		passed = TESTS_PASS;
	}
	else
	{
		tst_print_fail ("CHECK_Is_Greater");
		passed = TESTS_FAIL;
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
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
	bignbr_init (&b, 10, "+1");
	
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
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Add negative greater to positive. */
	bignbr_fill (&a, "+150");
	bignbr_fill (&b, "-200");
	
	bignbr_add (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-50"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Add negative greater to positive. */
	bignbr_fill (&a, "+15");
	bignbr_fill (&b, "-200");
	
	bignbr_add (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "-185"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Add positive greater to negative. */
	bignbr_fill (&a, "-150");
	bignbr_fill (&b, "+200");
	
	bignbr_add (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "+50"))
	{
		passed = TESTS_PASS;
	}
	else
	{
		passed = TESTS_FAIL;
	}
	
	/* Add positive greater to negative. */
	bignbr_fill (&a, "-15");
	bignbr_fill (&b, "+200");
	
	bignbr_add (&a, &b);
	
	if (passed == TESTS_PASS &&
	    bignbr_cmp_str (&a, "+185"))
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
	bignbr_init (&b, 1, "+1");
	
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
	bignbr_init (&b, 1, "+3");
	
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
	
	/* Multiply negative to negative. */
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
	
	points += bignbr_test_core_cpy ();
	points += bignbr_test_core_fill ();
	points += bignbr_test_core_geteonpos ();
	
	printf ("\n");
	
	points += bignbr_test_check_cmp_str ();
	points += bignbr_test_check_is_null ();
	points += bignbr_test_check_negative ();
	points += bignbr_test_check_is_greater ();
	
	printf ("\n");
	
	points += bignbr_test_int_add ();
	points += bignbr_test_int_sub ();
	points += bignbr_test_int_mpl ();
	
	tst_print_summary (points);
	
	return 0;
}
