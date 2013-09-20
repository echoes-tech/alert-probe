/* 
 * Tests of Search Functions
 * @author ECHOES Technologies (MLA)
 * @date 18/09/2013
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
#ifndef NDEBUG
    #include "CUnit/Basic.h"
#else
    #include "CUnit/Automated.h"
#endif
#include "search.h"

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

void testSearchLocation()
{
    char** values = calloc(1, sizeof(char*));
    const char* line = "0123456789ABCDEF";
    const unsigned int length = 10;
    const unsigned int firstChar = 5;
    gushort result = searchLocation(values, line, &length, &firstChar);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_STRING_EQUAL(values[0], "456789ABCD");
    free(values[0]);
    free(values);
}

void testSearchRegex()
{
    regex_t preg;
    int err = 0;
    size_t nmatch = 0;
    regmatch_t* pmatch = NULL;
    char** values = NULL;
    char line[256];
    gushort result = 0;
    unsigned int i = 0;

    err = regcomp(&preg, "^([0-9]+\\.[0-9]+)\\s([0-9]+\\.[0-9]+)\\s([0-9]+\\.[0-9]+)", REG_EXTENDED);
    nmatch = (preg.re_nsub + 1);
    pmatch = malloc(sizeof (*pmatch) * nmatch);
    strcpy(line, "0.38 0.36 0.31 1/484 8170\n");
    values = calloc(preg.re_nsub, sizeof (char*));
    result = searchRegex(values, line, &err, &preg, &nmatch, pmatch);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_STRING_EQUAL(values[0], "0.38");
    CU_ASSERT_STRING_EQUAL(values[1], "0.36");
    CU_ASSERT_STRING_EQUAL(values[2], "0.31");
    for (i = 0; i < preg.re_nsub; i++)
        free(values[i]);
    free(values);

    err = regcomp(&preg, "^\\s*(\\w+):\\s*([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+[0-9]+\\s+[0-9]+\\s+[0-9]+\\s+[0-9]+\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+[0-9]+\\s+[0-9]+\\s+[0-9]+\\s+[0-9]+", REG_EXTENDED);
    nmatch = (preg.re_nsub + 1);
    pmatch = malloc(sizeof (*pmatch) * nmatch);
    strcpy(line, "  eth0: 31246514   23682    0    0    0     0          0        76  1518635   13827    0    0    0     0       0          0\n");
    values = calloc(preg.re_nsub, sizeof (char*));
    result = searchRegex(values, line, &err, &preg, &nmatch, pmatch);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_STRING_EQUAL(values[0], "eth0");
    CU_ASSERT_STRING_EQUAL(values[1], "31246514");
    CU_ASSERT_STRING_EQUAL(values[2], "23682");
    CU_ASSERT_STRING_EQUAL(values[3], "0");
    CU_ASSERT_STRING_EQUAL(values[4], "0");
    CU_ASSERT_STRING_EQUAL(values[5], "1518635");
    CU_ASSERT_STRING_EQUAL(values[6], "13827");
    CU_ASSERT_STRING_EQUAL(values[7], "0");
    CU_ASSERT_STRING_EQUAL(values[8], "0");
    for (i = 0; i < preg.re_nsub; i++)
        free(values[i]);
    free(values);

    err = regcomp(&preg, "^Cached:\\s+([0-9]+)\\skB", REG_EXTENDED);
    nmatch = (preg.re_nsub + 1);
    pmatch = malloc(sizeof (*pmatch) * nmatch);
    strcpy(line, "Cached:           881748 kB\n");
    values = calloc(preg.re_nsub, sizeof (char*));
    result = searchRegex(values, line, &err, &preg, &nmatch, pmatch);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_STRING_EQUAL(values[0], "881748");
    free(values[0]);
    free(values);

    err = regcomp(&preg, "^Cached:\\s+([0-9]+)\\skB", REG_EXTENDED);
    nmatch = (preg.re_nsub + 1);
    pmatch = malloc(sizeof (*pmatch) * nmatch);
    strcpy(line, "Unevictable:           881748 kB\n");
    values = calloc(preg.re_nsub, sizeof (char*));
    result = searchRegex(values, line, &err, &preg, &nmatch, pmatch);
    CU_ASSERT_NOT_EQUAL(result, EXIT_SUCCESS);
    free(values);
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("searchcunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testSearchLocation", testSearchLocation)) ||
        (NULL == CU_add_test(pSuite, "testSearchRegex", testSearchRegex)))
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
    CU_set_output_filename("cunit-result/search");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif

    CU_cleanup_registry();
    return CU_get_error();
}
