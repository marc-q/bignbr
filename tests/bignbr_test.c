/* Copyright 2016 Marc Volker Dickmann */
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
	short passed, i;
	unsigned char data[12][10];
	bignbr a, b;
	
	/* Testdata */
	/* Copy positive to positive. */
	strcpy (data[0], "+1005");
	strcpy (data[1], "+2005");
	strcpy (data[2], "+2005");
	
	/* Copy positive to negative. */
	strcpy (data[3], "-1005");
	strcpy (data[4], "+2005");
	strcpy (data[5], "+2005");
	
	/* Copy negative to positive. */
	strcpy (data[6], "+1005");
	strcpy (data[7], "-2005");
	strcpy (data[8], "-2005");
	
	/* Copy negative to negative. */
	strcpy (data[9], "-1005");
	strcpy (data[10], "-2005");
	strcpy (data[11], "-2005");
	
	bignbr_init (&a, 10, "+0");
	bignbr_init (&b, 4, "+0");
	
	passed = TESTS_PASS;
	
	for (i = 0; i < 12; i++)
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
	Function: bignbr_test_core_cat_digit (void);
	Description: Tests the bignbr_cat_digit function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_core_cat_digit (void)
{
	short passed, i;
	unsigned char data[9][10];
	bignbr a;
	
	/* Testdata */
	strcpy (data[0], "+0");
	data[1][0] = 1;
	strcpy (data[2], "+01");
	
	strcpy (data[3], "-1");
	data[4][0] = 0;
	strcpy (data[5], "-10");
	
	strcpy (data[6], "+0");
	data[7][0] = 0;
	strcpy (data[8], "+00");
	
	bignbr_init (&a, 10, "+0");
	
	passed = TESTS_PASS;
	
	for (i = 0; i < 9; i++)
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
	Function: bignbr_test_core_fill (void);
	Description: Tests the bignbr_fill function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_core_fill (void)
{
	short passed, i;
	unsigned char data[4][20];
	bignbr a;
	
	/* Testdata */
	/* Fill a positive. */
	strcpy (data[0], "+01234567890");
	memcpy (data[1], "+\x00\x09\x08\x07\x06\x05\x04\x03\x02\x01\x00\x45", 13);
	
	/* Fill a negative. */
	strcpy (data[2], "-09876543210");
	memcpy (data[3], "-\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x00\x45", 13);
	
	bignbr_init (&a, 11, "+0");
	
	passed = TESTS_PASS;
	
	for (i = 0; i < 4; i += 2)
	{
		bignbr_fill (&a, data[i]);
		
		if (!bignbr_cmp_str (&a, data[i]) ||
		    memcmp (a.data, data[i+1], strlen (data[i])+1) != 0)
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
	short passed, i;
	unsigned char data[30][30];
	bignbr a, b;
	
	/* Testdata */
	/* Add positive to positive. */
	strcpy (data[0], "+204242999999999999999999");
	strcpy (data[1], "+1");
	strcpy (data[2], "+204243000000000000000000");
	
	/* Add positive to negative. */
	strcpy (data[3], "-204242999999999999999999");
	strcpy (data[4], "+1");
	strcpy (data[5], "-204242999999999999999998");
	
	/* Add negative to positive. */
	strcpy (data[6], "+204242999999999999999999");
	strcpy (data[7], "-1");
	strcpy (data[8], "+204242999999999999999998");
	
	/* Add negative to negative. */
	strcpy (data[9], "-204242999999999999999999");
	strcpy (data[10], "-1");
	strcpy (data[11], "-204243000000000000000000");
	
	/* Add negative greater to positive. */
	strcpy (data[12], "+150");
	strcpy (data[13], "-200");
	strcpy (data[14], "-50");
	
	/* Add negative greater to positive. */
	strcpy (data[15], "+15");
	strcpy (data[16], "-200");
	strcpy (data[17], "-185");
	
	/* Add positive greater to negative. */
	strcpy (data[18], "-150");
	strcpy (data[19], "+200");
	strcpy (data[20], "+50");
	
	/* Add positive greater to negative. */
	strcpy (data[21], "-15");
	strcpy (data[22], "+200");
	strcpy (data[23], "+185");
	
	/* Add positive equal to negative. */
	strcpy (data[24], "-15");
	strcpy (data[25], "+15");
	strcpy (data[26], "+0");
	
	/* Add negative equal to postive. */
	strcpy (data[27], "+15");
	strcpy (data[28], "-15");
	strcpy (data[29], "+0");
	
	bignbr_init (&a, 30, "+0");
	bignbr_init (&b, 10, "+0");
	
	passed = TESTS_PASS;
	
	for (i = 0; i < 30; i++)
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
	Function: bignbr_test_int_sub (void);
	Description: Tests the bignbr_sub function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_int_sub (void)
{
	short passed, i;
	unsigned char data[12][30];
	bignbr a, b;
	
	/* Testdata */
	/* Subtract positive from positive. */
	strcpy (data[0], "+204242999999999999999999");
	strcpy (data[1], "+1");
	strcpy (data[2], "+204242999999999999999998");
	
	/* Subtract positive from negative. */
	strcpy (data[3], "-204242999999999999999999");
	strcpy (data[4], "+1");
	strcpy (data[5], "-204243000000000000000000");
	
	/* Subtract negative from positive. */
	strcpy (data[6], "+204242999999999999999999");
	strcpy (data[7], "-1");
	strcpy (data[8], "+204243000000000000000000");
	
	/* Subtract negative from negative. */
	strcpy (data[9], "-204242999999999999999999");
	strcpy (data[10], "-1");
	strcpy (data[11], "-204242999999999999999998");
	
	bignbr_init (&a, 30, "+0");
	bignbr_init (&b, 1, "+0");
	
	passed = TESTS_PASS;
	
	for (i = 0; i < 12; i++)
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
	Function: bignbr_test_int_mpl (void);
	Description: Tests the bignbr_mpl function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_int_mpl (void)
{
	short passed, i;
	unsigned char data[12][20];
	bignbr a, b;
	
	/* Testdata */
	/* Multiply positive with positive. */
	strcpy (data[0], "+200050008");
	strcpy (data[1], "+3");
	strcpy (data[2], "+600150024");
	
	/* Multiply positive with negative. */
	strcpy (data[3], "-200050008");
	strcpy (data[4], "+3");
	strcpy (data[5], "-600150024");
	
	/* Multiply negative with positive. */
	strcpy (data[6], "+200050008");
	strcpy (data[7], "-3");
	strcpy (data[8], "-600150024");
	
	/* Multiply negative with negative. */
	strcpy (data[9], "-200050008");
	strcpy (data[10], "-3");
	strcpy (data[11], "+600150024");
	
	bignbr_init (&a, 20, "+0");
	bignbr_init (&b, 1, "+0");
	
	passed = TESTS_PASS;
	
	for (i = 0; i < 12; i++)
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
	Function: bignbr_test_int_div (void);
	Description: Tests the bignbr_div function from bignbr.c.
	InitVersion: 0.0.1
*/
static short bignbr_test_int_div (void)
{
	unsigned int i;
	short passed;
	unsigned char data[30][30];
	bignbr a, b, r, p;
	
	/* Testdata */
	/* Divide positive with positive. */
	strcpy (data[0], "+200050008");
	strcpy (data[1], "+3");
	strcpy (data[2], "+10");
	strcpy (data[3], "+66683336");
	strcpy (data[4], "+0");
	
	/* Divide positive with negative. */
	strcpy (data[5], "-200050008");
	strcpy (data[6], "+3");
	strcpy (data[7], "+10");
	strcpy (data[8], "-66683336");
	strcpy (data[9], "+0");
	
	/* Divide negative with positive. */
	strcpy (data[10], "+200050008");
	strcpy (data[11], "-3");
	strcpy (data[12], "+10");
	strcpy (data[13], "-66683336");
	strcpy (data[14], "+0");
	
	/* Divide negative with negative. */
	strcpy (data[15], "-200050008");
	strcpy (data[16], "-3");
	strcpy (data[17], "+10");
	strcpy (data[18], "+66683336");
	strcpy (data[19], "+0");
	
	strcpy (data[20], "+1");
	strcpy (data[21], "+7");
	strcpy (data[22], "+10");
	strcpy (data[23], "+0");
	strcpy (data[24], "+142857142");
	
	strcpy (data[25], "+20");
	strcpy (data[26], "+5");
	strcpy (data[27], "+10");
	strcpy (data[28], "+4");
	strcpy (data[29], "+0");
	
	bignbr_init (&a, 100, "+0");
	bignbr_init (&b, 30, "+0");
	bignbr_init (&r, 100, "+0");
	bignbr_init (&p, 30, "+0");
	
	passed = TESTS_PASS;
	
	for (i = 0; i < 30; i++)
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

int main (int argc, char* argv[])
{
	int points;
	
	points = 0;
	
	printf ("BigNbr v. 0.0.1 A (c) 2016 Marc Volker Dickmann\n\n");
	
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
