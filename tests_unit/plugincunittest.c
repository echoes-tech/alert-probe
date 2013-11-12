/* 
 * Tests of Plug-in Manager
 * @author ECHOES Technologies (MLA)
 * @date 17/09/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <addonLib.h>
#ifndef NDEBUG
    #include "CUnit/Basic.h"
#else
    #include "CUnit/Automated.h"
#endif
#include "plugin.h"
#include "conf.h"

/*
 * CUnit Test Suite
 */

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

void testVerifExt()
{
    char string[256];
    strcpy(string, "abcd.json");
    CU_ASSERT(verifExt(string));
    strcpy(string, ".json");
    CU_ASSERT(verifExt(string));
    strcpy(string, "abcd");
    CU_ASSERT_FALSE(verifExt(string));
    strcpy(string, "");
    CU_ASSERT_FALSE(verifExt(string));
}

void testAddBackslash()
{
    char string[256];
    strcpy(string, "abcdefghi");
    addBackslash(string);
    CU_ASSERT_STRING_EQUAL(string, "abcdefghi");
    strcpy(string, "abcd\\efghi");
    addBackslash(string);
    CU_ASSERT_STRING_EQUAL(string, "abcd\\\\efghi");
    strcpy(string, "abcd\\\\efghi");
    addBackslash(string);
    CU_ASSERT_STRING_EQUAL(string, "abcd\\\\\\\\efghi");
    strcpy(string, "abcd\\\"efghi");
    addBackslash(string);
    CU_ASSERT_STRING_EQUAL(string, "abcd\\\\\"efghi");
    strcpy(string, "abc\\def\\\"ghi");
    addBackslash(string);
    CU_ASSERT_STRING_EQUAL(string, "abc\\\\def\\\\\"ghi");
    strcpy(string, "abc\\\"def\\\"ghi");
    addBackslash(string);
    CU_ASSERT_STRING_EQUAL(string, "abc\\\\\"def\\\\\"ghi");
    strcpy(string, "abc\\\"d\\ef\\\"ghi");
    addBackslash(string);
    CU_ASSERT_STRING_EQUAL(string, "abc\\\\\"d\\\\ef\\\\\"ghi");
    
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("plugincunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testVerifExt", testVerifExt)) ||
        (NULL == CU_add_test(pSuite, "testAddBackslash", testAddBackslash)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

#ifndef NDEBUG
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
#else
    /* Run all tests using the CUnit Automated interface */
    CU_set_output_filename("cunit-result/plugin");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif

    CU_cleanup_registry();
    return CU_get_error();
}
