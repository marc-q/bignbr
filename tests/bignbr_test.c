/* Copyright 2016 - 2017 Marc Volker Dickmann */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/dutils.h"
#include "../bignbr.h"

#define TESTS_AMOUNT 12
#define TESTS_FAIL 0
#define TESTS_PASS 1

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */

/*
 *	Function: tst_print_success (char* tstname);
 *	Description: Prints the test success message.
 *	InitVersion: 0.0.1
 */
static void
tst_print_success (const char *tstname)
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
 *	Function: tst_print_fail (char* tstname);
 *	Description: Prints the test failure message.
 *	InitVersion: 0.0.1
 */
static void
tst_print_fail (const char *tstname)
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
 *	Function: tst_print_summary (int points);
 *	Description: Prints the summary of all tests.
 *	InitVersion: 0.0.1
 */
static void
tst_print_summary (const int points)
{
	printf ("\n+=======================+\n");
	printf ("|  Summary of all tests |\n");
	printf ("+=======================+\n");
	printf ("|  Tests: \t%i\t|\n", TESTS_AMOUNT);
	printf ("+-----------------------+\n");
	printf ("|  Passed:\t%i\t|\n", points);
	printf ("|  Failed:\t%i\t|\n", TESTS_AMOUNT - points);
	printf ("+-----------------------+\n");
}

/* |--------------------------------------------|
   |		    Tests-Core			|
   |--------------------------------------------| */

/*
 *	Function: bignbr_test_core_cpy (void);
 *	Description: Tests the bignbr_cpy function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_core_cpy (void)
{
	size_t cases = 0;
	char data[12][10];
	
	// Testdata
	// Copy positive to positive.
	strcpy (data[cases++], "+1005");
	strcpy (data[cases++], "+2005");
	strcpy (data[cases++], "+2005");
	
	// Copy positive to negative.
	strcpy (data[cases++], "-1005");
	strcpy (data[cases++], "+2005");
	strcpy (data[cases++], "+2005");
	
	// Copy negative to positive.
	strcpy (data[cases++], "+1005");
	strcpy (data[cases++], "-2005");
	strcpy (data[cases++], "-2005");
	
	// Copy negative to negative.
	strcpy (data[cases++], "-1005");
	strcpy (data[cases++], "-2005");
	strcpy (data[cases++], "-2005");
	
	bignbr a, b;
	bignbr_init (&a, 10, "+0");
	bignbr_init (&b, 4, "+0");
	
	int passed = TESTS_PASS;
	
	for (size_t i = 0; i < cases; i++)
	{
		bignbr_fill (&a, data[i++]);
		bignbr_fill (&b, data[i++]);
		
		bignbr_cpy (&a, &b);
		
		if (!bignbr_cmp_str (&a, data[i]) ||
		    !bignbr_cmp_str (&b, data[i]))
		{
			passed = TESTS_FAIL;
			break;
		}
	}
	
	if (passed == TESTS_PASS)
	{
		tst_print_success ("CORE_Copy");
	}
	else
	{
		tst_print_fail ("CORE_Copy");
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
}

/*
 *	Function: bignbr_test_core_cat_digit (void);
 *	Description: Tests the bignbr_cat_digit function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_core_cat_digit (void)
{
	size_t cases = 0;
	char data[9][10];
	
	// Testdata
	strcpy (data[cases++], "+0");
	data[cases++][0] = 1;
	strcpy (data[cases++], "+01");
	
	strcpy (data[cases++], "-1");
	data[cases++][0] = 0;
	strcpy (data[cases++], "-10");
	
	strcpy (data[cases++], "+0");
	data[cases++][0] = 0;
	strcpy (data[cases++], "+00");
	
	bignbr a;
	bignbr_init (&a, 10, "+0");
	
	int passed = TESTS_PASS;
	
	for (size_t i = 0; i < cases; i++)
	{
		bignbr_fill (&a, data[i++]);
		
		bignbr_cat_digit (&a, data[i++][0]);
		
		if (!bignbr_cmp_str (&a, data[i]))
		{
			passed = TESTS_FAIL;
			break;
		}
	}
	
	if (passed == TESTS_PASS)
	{
		tst_print_success ("CORE_Cat_Digit");
	}
	else
	{
		tst_print_fail ("CORE_Cat_Digit");
	}
	
	bignbr_free (&a);
	return passed;
}

/*
 *	Function: bignbr_test_core_fill (void);
 *	Description: Tests the bignbr_fill function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_core_fill (void)
{
	size_t cases = 0;
	char data[4][20];
	
	// Testdata
	// Fill a positive.
	strcpy (data[cases++], "+01234567890");
	memcpy (data[cases++], "+\x00\x09\x08\x07\x06\x05\x04\x03\x02\x01\x00\x45", 13);
	
	// Fill a negative.
	strcpy (data[cases++], "-09876543210");
	memcpy (data[cases++], "-\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x00\x45", 13);
	
	bignbr a;
	bignbr_init (&a, 11, "+0");
	
	int passed = TESTS_PASS;
	
	for (size_t i = 0; i < cases; i += 2)
	{
		bignbr_fill (&a, data[i]);
		
		if (!bignbr_cmp_str (&a, data[i]) ||
		     memcmp (a.data, data[i + 1], strlen (data[i]) + 1) != 0)
		{
			passed = TESTS_FAIL;
			break;
		}
	}
	
	if (passed == TESTS_PASS)
	{
		tst_print_success ("CORE_Fill");
	}
	else
	{
		tst_print_fail ("CORE_Fill");
	}
	
	bignbr_free (&a);
	return passed;
}

/*
 *	Function: bignbr_test_core_geteonpos (void);
 *	Description: Tests the bignbr_get_eon_pos function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_core_geteonpos (void)
{
	int passed = TESTS_PASS;
	
	bignbr a;
	// Get the EON from an positive 1 digit number.
	bignbr_init (&a, 10, "+0");
	
	size_t pos = bignbr_get_eon_pos (&a);
	
	if (a.data[pos] != BIGNBR_EON)
	{
		passed = TESTS_FAIL;
	}
	
	// Get the EON from an negative 10 digit number.
	bignbr_fill (&a, "-0987654321");
	
	pos = bignbr_get_eon_pos (&a);
	
	if (passed == TESTS_PASS &&
	    a.data[pos] == BIGNBR_EON)
	{
		tst_print_success ("CORE_GetEonPos");
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
 *	Function: bignbr_test_check_cmp_str (void);
 *	Description: Tests the bignbr_cmp_str function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_check_cmp_str (void)
{
	int passed = TESTS_PASS;
	
	bignbr a;
	// Compare a positive with a negative number.
	bignbr_init (&a, 11, "+01234567890");
	
	if ( bignbr_cmp_str (&a, "-01234567890") ||
	    !bignbr_cmp_str (&a, "+01234567890") ||
	     bignbr_cmp_str (&a, "+0123456789") ||
	     bignbr_cmp_str (&a, "+1234567890") ||
	     bignbr_cmp_str (&a, "+0") ||
	     bignbr_cmp_str (&a, "-0"))
	{
		passed = TESTS_FAIL;
	}
	
	// Compare a negative with a positive number.
	bignbr_fill (&a, "-01234567890");
	
	if ( passed == TESTS_PASS &&
	    !bignbr_cmp_str (&a, "+01234567890") &&
	     bignbr_cmp_str (&a, "-01234567890") &&
	    !bignbr_cmp_str (&a, "-0123456789") &&
	    !bignbr_cmp_str (&a, "-1234567890") &&
	    !bignbr_cmp_str (&a, "+0") &&
	    !bignbr_cmp_str (&a, "-0"))
	{
		tst_print_success ("CHECK_Compare_Str");
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
 *	Function: bignbr_test_check_is_null (void);
 *	Description: Tests the bignbr_is_null function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_check_is_null (void)
{
	int passed = TESTS_PASS;
	
	bignbr a;
	// Positive null.
	bignbr_init (&a, 11, "+0");
	
	if (!bignbr_is_null (&a))
	{
		passed = TESTS_FAIL;
	}
	
	// Negative null.
	bignbr_fill (&a, "-0");
	
	if (!bignbr_is_null (&a))
	{
		passed = TESTS_FAIL;
	}
	
	// Positive number.
	bignbr_fill (&a, "+01234567890");
	
	if (bignbr_is_null (&a))
	{
		passed = TESTS_FAIL;
	}
	
	// Negative number.
	bignbr_fill (&a, "-01234567890");
	
	if ( passed == TESTS_PASS &&
	    !bignbr_is_null (&a))
	{
		tst_print_success ("CHECK_Is_Null");
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
 *	Function: bignbr_test_check_negative (void);
 *	Description: Tests the bignbr_is_negative and bignbr_set_negative functions from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_check_negative (void)
{
	int passed = TESTS_PASS;
	
	bignbr a;
	// Positive null.
	bignbr_init (&a, 11, "+0");
	
	if (bignbr_is_negative (&a))
	{
		passed = TESTS_FAIL;
	}
	
	// Negative null. (Impossible)
	bignbr_set_negative (&a, true);
	
	if (bignbr_is_negative (&a))
	{
		passed = TESTS_FAIL;
	}
	
	// Negative number.
	bignbr_fill (&a, "-01234567890");
	
	if (!bignbr_is_negative (&a))
	{
		passed = TESTS_FAIL;
	}
	
	// Positive number.
	bignbr_set_negative (&a, false);
	
	if ( passed == TESTS_PASS &&
	    !bignbr_is_negative (&a))
	{
		tst_print_success ("CHECK_Negative");
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
 *	Function: bignbr_test_check_is_greater (void);
 *	Description: Tests the bignbr_is_greater function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_check_is_greater (void)
{
	int passed = TESTS_PASS;
	
	bignbr a, b;
	// Check two positive nulls.
	bignbr_init (&a, 3, "+0");
	bignbr_init (&b, 10, "+0");
	
	if (bignbr_is_greater (&a, &b))
	{
		passed = TESTS_FAIL;
	}
	
	// Check two negative nulls.
	bignbr_fill (&a, "-0");
	bignbr_fill (&b, "-0");
	
	if (bignbr_is_greater (&a, &b))
	{
		passed = TESTS_FAIL;
	}
	
	// Check two nulls with unequal signs.
	bignbr_fill (&a, "+0");
	bignbr_fill (&b, "-0");
	
	if (bignbr_is_greater (&a, &b) ||
	    bignbr_is_greater (&b, &a))
	{
		passed = TESTS_FAIL;
	}
	
	// NUMBERS
	
	// Check two unequal positive numbers.
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "+50");
	
	if (!bignbr_is_greater (&a, &b) ||
	     bignbr_is_greater (&b, &a))
	{
		passed = TESTS_FAIL;
	}
	
	// Check two unequal negative numbers.
	bignbr_fill (&a, "-128");
	bignbr_fill (&b, "-50");
	
	if ( bignbr_is_greater (&a, &b) ||
	    !bignbr_is_greater (&b, &a))
	{
		passed = TESTS_FAIL;
	}
	
	// Check a positive and an negative number.
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "-50");
	
	if (!bignbr_is_greater (&a, &b))
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-128");
	bignbr_fill (&b, "+0");
	
	if (bignbr_is_greater (&a, &b))
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-0");
	bignbr_fill (&b, "-128");
	
	if (!bignbr_is_greater (&a, &b))
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "-128");
	
	if (!bignbr_is_greater (&a, &b) ||
	     bignbr_is_greater (&b, &a))
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+128");
	bignbr_fill (&b, "+128");
	
	if (bignbr_is_greater (&a, &b))
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-128");
	bignbr_fill (&b, "-128");
	
	if (bignbr_is_greater (&a, &b))
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+120");
	bignbr_fill (&b, "+128");
	
	if ( bignbr_is_greater (&a, &b) ||
	    !bignbr_is_greater (&b, &a))
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "-120");
	bignbr_fill (&b, "-128");
	
	if (!bignbr_is_greater (&a, &b) ||
	     bignbr_is_greater (&b, &a))
	{
		passed = TESTS_FAIL;
	}
	
	bignbr_fill (&a, "+11");
	bignbr_fill (&b, "+40");
	
	if ( passed == TESTS_PASS &&
	    !bignbr_is_greater (&a, &b))
	{
		tst_print_success ("CHECK_Is_Greater");
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
static int
bignbr_test_int_add (void)
{
	size_t cases = 0;
	char data[30][30];
	
	// Testdata
	// Add positive to positive.
	strcpy (data[cases++], "+204242999999999999999999");
	strcpy (data[cases++], "+1");
	strcpy (data[cases++], "+204243000000000000000000");
	
	// Add positive to negative.
	strcpy (data[cases++], "-204242999999999999999999");
	strcpy (data[cases++], "+1");
	strcpy (data[cases++], "-204242999999999999999998");
	
	// Add negative to positive.
	strcpy (data[cases++], "+204242999999999999999999");
	strcpy (data[cases++], "-1");
	strcpy (data[cases++], "+204242999999999999999998");
	
	// Add negative to negative.
	strcpy (data[cases++], "-204242999999999999999999");
	strcpy (data[cases++], "-1");
	strcpy (data[cases++], "-204243000000000000000000");
	
	// Add negative greater to positive.
	strcpy (data[cases++], "+150");
	strcpy (data[cases++], "-200");
	strcpy (data[cases++], "-50");
	
	// Add negative greater to positive.
	strcpy (data[cases++], "+15");
	strcpy (data[cases++], "-200");
	strcpy (data[cases++], "-185");
	
	// Add positive greater to negative.
	strcpy (data[cases++], "-150");
	strcpy (data[cases++], "+200");
	strcpy (data[cases++], "+50");
	
	// Add positive greater to negative.
	strcpy (data[cases++], "-15");
	strcpy (data[cases++], "+200");
	strcpy (data[cases++], "+185");
	
	// Add positive equal to negative.
	strcpy (data[cases++], "-15");
	strcpy (data[cases++], "+15");
	strcpy (data[cases++], "+0");
	
	// Add negative equal to postive.
	strcpy (data[cases++], "+15");
	strcpy (data[cases++], "-15");
	strcpy (data[cases++], "+0");
	
	bignbr a, b;
	bignbr_init (&a, 30, "+0");
	bignbr_init (&b, 10, "+0");
	
	int passed = TESTS_PASS;
	
	for (size_t i = 0; i < cases; i++)
	{
		bignbr_fill (&a, data[i++]);
		bignbr_fill (&b, data[i++]);
		
		bignbr_add (&a, &b);
		
		if (!bignbr_cmp_str (&a, data[i]))
		{
			passed = TESTS_FAIL;
			break;
		}
	}
	
	if (passed == TESTS_PASS)
	{
		tst_print_success ("INT_Addition");
	}
	else
	{
		tst_print_fail ("INT_Addition");
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
}

/*
 *	Function: bignbr_test_int_sub (void);
 *	Description: Tests the bignbr_sub function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_int_sub (void)
{
	size_t cases = 0;
	char data[12][30];
	
	// Testdata
	// Subtract positive from positive.
	strcpy (data[cases++], "+204242999999999999999999");
	strcpy (data[cases++], "+1");
	strcpy (data[cases++], "+204242999999999999999998");
	
	// Subtract positive from negative.
	strcpy (data[cases++], "-204242999999999999999999");
	strcpy (data[cases++], "+1");
	strcpy (data[cases++], "-204243000000000000000000");
	
	// Subtract negative from positive.
	strcpy (data[cases++], "+204242999999999999999999");
	strcpy (data[cases++], "-1");
	strcpy (data[cases++], "+204243000000000000000000");
	
	// Subtract negative from negative.
	strcpy (data[cases++], "-204242999999999999999999");
	strcpy (data[cases++], "-1");
	strcpy (data[cases++], "-204242999999999999999998");
	
	bignbr a, b;
	bignbr_init (&a, 30, "+0");
	bignbr_init (&b, 1, "+0");
	
	int passed = TESTS_PASS;
	
	for (size_t i = 0; i < cases; i++)
	{
		bignbr_fill (&a, data[i++]);
		bignbr_fill (&b, data[i++]);
		
		bignbr_sub (&a, &b);
		
		if (!bignbr_cmp_str (&a, data[i]))
		{
			passed = TESTS_FAIL;
			break;
		}
	}
	
	if (passed == TESTS_PASS)
	{
		tst_print_success ("INT_Subtraction");
	}
	else
	{
		tst_print_fail ("INT_Subtraction");
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
}

/*
 *	Function: bignbr_test_int_mpl (void);
 *	Description: Tests the bignbr_mpl function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_int_mpl (void)
{
	size_t cases = 0;
	char data[12][20];
	
	// Testdata
	// Multiply positive with positive.
	strcpy (data[cases++], "+200050008");
	strcpy (data[cases++], "+3");
	strcpy (data[cases++], "+600150024");
	
	// Multiply positive with negative.
	strcpy (data[cases++], "-200050008");
	strcpy (data[cases++], "+3");
	strcpy (data[cases++], "-600150024");
	
	// Multiply negative with positive.
	strcpy (data[cases++], "+200050008");
	strcpy (data[cases++], "-3");
	strcpy (data[cases++], "-600150024");
	
	// Multiply negative with negative.
	strcpy (data[cases++], "-200050008");
	strcpy (data[cases++], "-3");
	strcpy (data[cases++], "+600150024");
	
	bignbr a, b;
	bignbr_init (&a, 20, "+0");
	bignbr_init (&b, 1, "+0");
	
	int passed = TESTS_PASS;
	
	for (size_t i = 0; i < cases; i++)
	{
		bignbr_fill (&a, data[i++]);
		bignbr_fill (&b, data[i++]);
		
		bignbr_mpl (&a, &b);
		
		if (!bignbr_cmp_str (&a, data[i]))
		{
			passed = TESTS_FAIL;
			break;
		}
	}
	
	if (passed == TESTS_PASS)
	{
		tst_print_success ("INT_Multiplication");
	}
	else
	{
		tst_print_fail ("INT_Multiplication");
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	return passed;
}

/*
 *	Function: bignbr_test_int_div (void);
 *	Description: Tests the bignbr_div function from bignbr.c.
 *	InitVersion: 0.0.1
 */
static int
bignbr_test_int_div (void)
{
	size_t cases = 0;
	char data[30][30];
	
	// Testdata
	// Divide positive with positive.
	strcpy (data[cases++], "+200050008");
	strcpy (data[cases++], "+3");
	strcpy (data[cases++], "+10");
	strcpy (data[cases++], "+66683336");
	strcpy (data[cases++], "+0");
	
	// Divide positive with negative.
	strcpy (data[cases++], "-200050008");
	strcpy (data[cases++], "+3");
	strcpy (data[cases++], "+10");
	strcpy (data[cases++], "-66683336");
	strcpy (data[cases++], "+0");
	
	// Divide negative with positive.
	strcpy (data[cases++], "+200050008");
	strcpy (data[cases++], "-3");
	strcpy (data[cases++], "+10");
	strcpy (data[cases++], "-66683336");
	strcpy (data[cases++], "+0");
	
	// Divide negative with negative.
	strcpy (data[cases++], "-200050008");
	strcpy (data[cases++], "-3");
	strcpy (data[cases++], "+10");
	strcpy (data[cases++], "+66683336");
	strcpy (data[cases++], "+0");
	
	strcpy (data[cases++], "+1");
	strcpy (data[cases++], "+7");
	strcpy (data[cases++], "+10");
	strcpy (data[cases++], "+0");
	strcpy (data[cases++], "+142857142");
	
	strcpy (data[cases++], "+20");
	strcpy (data[cases++], "+5");
	strcpy (data[cases++], "+10");
	strcpy (data[cases++], "+4");
	strcpy (data[cases++], "+0");
	
	bignbr a, b, r, p;
	bignbr_init (&a, 100, "+0");
	bignbr_init (&b, 30, "+0");
	bignbr_init (&r, 100, "+0");
	bignbr_init (&p, 30, "+0");
	
	int passed = TESTS_PASS;
	
	for (size_t i = 0; i < cases; i++)
	{
		bignbr_fill (&a, data[i++]);
		bignbr_fill (&b, data[i++]);
		bignbr_fill (&r, "+");
		bignbr_fill (&p, data[i++]);
		
		bignbr_div (&a, &b, &r, &p);
		
		if (!bignbr_cmp_str (&a, data[i++]) ||
		    !bignbr_cmp_str (&r, data[i]))
		{
			passed = TESTS_FAIL;
			break;
		}
	}
	
	if (passed == TESTS_PASS)
	{
		tst_print_success ("INT_Division");
	}
	else
	{
		tst_print_fail ("INT_Division");
	}
	
	bignbr_free (&a);
	bignbr_free (&b);
	bignbr_free (&r);
	bignbr_free (&p);
	return passed;
}

int
main (int argc, char* argv[])
{	
	int points = 0;
	
	printf ("BigNbr v. 0.0.1 A (c) 2016 - 2017 Marc Volker Dickmann\n\n");
	
	points += bignbr_test_core_cpy ();
	points += bignbr_test_core_cat_digit ();
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
	points += bignbr_test_int_div ();
	
	tst_print_summary (points);
	
	return 0;
}
