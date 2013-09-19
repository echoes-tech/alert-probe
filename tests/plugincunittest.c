/*
 * File:   plugincunittest.c
 * Author: mla
 *
 * Created on 17 sept. 2013, 11:54:36
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "plugin.h"
#include "conf.h"
#include "addon/addon.h"

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

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
