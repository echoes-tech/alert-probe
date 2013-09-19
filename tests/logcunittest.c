/*
 * File:   logcunittest.c
 * Author: mla
 *
 * Created on 18 sept. 2013, 16:19:55
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "log.h"

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
    LogParams logParams = {
        "127.0.0.1",
        "ea-probe",
        5,
        ""
    };
    gpointer user_data = (gpointer) & logParams;
    log2Console(NULL, 0, message, user_data);
}

void testLog2File()
{
    FILE *logFile = NULL;
    char line[256] = "", string[256] = "";
    GDateTime *dateTime = NULL;
    gchar *dateTimeFormat = NULL;
    const gchar* message = "abcd";
    LogParams logParams = {
        "127.0.0.1",
        "ea-probe",
        5,
        "probe.log"
    };
    gpointer user_data = (gpointer) & logParams;
    remove(logParams.logFilePath);
    log2File(NULL, 0, message, user_data);
    logFile = fopen(logParams.logFilePath, "r");
    if (logFile != NULL)
    {
        if (fgets(line, 256, logFile) != NULL)
        {
            dateTime = g_date_time_new_now_local();
            dateTimeFormat = g_date_time_format(dateTime, "%FT%T%z");

            strcpy(string, dateTimeFormat);
            strcat(string, " 127.0.0.1 ea-probe 5 - abcd\n");
            CU_ASSERT_STRING_EQUAL(line, string);

            g_date_time_unref(dateTime);
            dateTime = NULL;
            free(dateTimeFormat);
            dateTimeFormat = NULL;
        }
        else
            CU_ASSERT(0);
        fclose(logFile);
        remove(logParams.logFilePath);
    }
    else
        CU_ASSERT(0);
}

void testLogStopProbe()
{
    const char* probeName;
    const char* probeVersion;
    logStopProbe(probeName, probeVersion);
    if (1 /*check result*/)
    {
        CU_ASSERT(0);
    }
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
        (NULL == CU_add_test(pSuite, "testLog2File", testLog2File))/* ||
        (NULL == CU_add_test(pSuite, "testLogStopProbe", testLogStopProbe))*/)
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
