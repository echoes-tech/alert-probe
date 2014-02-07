/* 
 * Tests of Logger
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
#include "log.h"
#include "utilUnitTest.h"

#define FILE_PATH "sortie.txt"

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

void testLog2Console()
{
    const gchar* message = "abcd";
    char* regex = "^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\+[0-9]{4} 127.0.0.1 ea-probe 5 - abcd\n$";
    LogParams logParams = {
        "127.0.0.1",
        "ea-probe",
        5,
        ""
    };
    gpointer user_data = (gpointer) & logParams;

    readStdout_init();

    log2Console(NULL, 0, message, user_data);

    /* Test forme de la sortie */
    CU_ASSERT_TRUE(outputFile_compare(regex));

    readStdout_end();
}

void testLog2File()
{
    const gchar* message = "abcd";
    char* regex = "^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\+[0-9]{4} 127.0.0.1 ea-probe 5 - abcd\n$";
    LogParams logParams = {
        "127.0.0.1",
        "ea-probe",
        5,
        getOuputFilePath()
    };
    gpointer user_data = (gpointer) & logParams;

    outputFile_remove();

    log2File(NULL, 0, message, user_data);
    
    /* Test forme de la sortie */
    CU_ASSERT_TRUE(outputFile_compare(regex));

    outputFile_remove();
}

void testLogStopProbe()
{
    const char* probeName = "test_probe";
    const char* probeVersion = "1.0";
    char* regex = "^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\+[0-9]{4} 127.0.0.1 ea-probe 5 - \\[origin enterpriseId=\"40311\" software=\"test_probe\" swVersion=\"1.0\"\\] stop\n$";
        
    LogParams logParams = {
        "127.0.0.1",
        "ea-probe",
        5,
        getOuputFilePath()
    };
    
    g_message_defineHandler(&logParams);
    
    readStdout_init();

    logStopProbe(probeName, probeVersion);

    /* Test forme de la sortie */
    CU_ASSERT_TRUE(outputFile_compare(regex));

    readStdout_end();
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("logcunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testLog2Console", testLog2Console)) ||
        (NULL == CU_add_test(pSuite, "testLog2File", testLog2File)) ||
        (NULL == CU_add_test(pSuite, "testLogStopProbe", testLogStopProbe)))
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
    CU_set_output_filename("cunit-result/log");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif

    CU_cleanup_registry();
    return CU_get_error();
}
