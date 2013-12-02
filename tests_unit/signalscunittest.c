/*
 * File:   signalscunittest.c
 * Author: mla
 *
 * Created on 23 sept. 2013, 17:08:32
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "signals.h"

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

void* boucleInfinie()
{
    while (1)
    {
        sleep(1);
    }
}

void testRestart()
{
    
}

void testSignalHandling()
{
    int signum = 0;
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;
    threadIdentifiers.nbAddonsThreads = 1;
    threadIdentifiers.addonsThreads = calloc(1, sizeof (pthread_t));

    pthread_create(&threadIdentifiers.addonsThreads[0], NULL, boucleInfinie, NULL);
    
    signalsHandler(&signum, &threadIdentifiers);

    signalHandling(SIGTERM);
    
    sleep(1);
    
    CU_ASSERT_EQUAL(signum, SIGTERM);
    CU_ASSERT(pthread_cancel(threadIdentifiers.addonsThreads[0]));
}

void testSignalsHandler()
{
    int signum = 0;
    printf("handler\n");
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;
    printf("handler\n");
    signalsHandler(&signum, &threadIdentifiers);
    printf("handler\n");

    kill(getpid(), SIGHUP);
    printf("handler\n");
    CU_ASSERT_EQUAL(signum, SIGHUP);
    printf("handler\n");
    signum = 0;
    printf("handler\n");
    kill(getpid(), SIGTERM);
    printf("handler\n");
    CU_ASSERT_EQUAL(signum, SIGTERM);
    printf("handler\n");
    signum = 0;
    printf("handler\n");
    kill(getpid(), SIGQUIT);
    printf("handler\n");
    CU_ASSERT_EQUAL(signum, SIGQUIT);
    printf("handler\n");
    signum = 0;
    printf("handler\n");
    kill(getpid(), SIGSYS);
    printf("handler\n");
    CU_ASSERT_EQUAL(signum, SIGSYS);
    printf("handler\n");
    
    /* signum != 0 le signal est ignore*/
    kill(getpid(), SIGTERM);
    printf("handler\n");
    CU_ASSERT_EQUAL(signum, SIGSYS);
    printf("handler\n");
}

void testWaitForShutdown()
{
    printf("wait\n");
    int signum = 5;
    printf("wait\n");
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;
    printf("wait\n");
    signalsHandler(&signum, &threadIdentifiers);
    printf("wait\n");
    CU_ASSERT_EQUAL(waitForShutdown(), 5);
    printf("wait\n");
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("signalscunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (/*(NULL == CU_add_test(pSuite, "testRestart", testRestart)) ||*/
        (NULL == CU_add_test(pSuite, "testSignalHandling", testSignalHandling)) ||
        (NULL == CU_add_test(pSuite, "testSignalsHandler", testSignalsHandler)) ||
        (NULL == CU_add_test(pSuite, "testWaitForShutdown", testWaitForShutdown)))
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
