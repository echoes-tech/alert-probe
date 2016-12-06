/* 
 * Alert - Probe is a collect probe, part of the Alert software
 * Copyright (C) 2013 Florent Poinsaut, Thomas Saquet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#ifndef NDEBUG
    #include "CUnit/Basic.h"
#else
    #include "CUnit/Automated.h"
#endif
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

void* boucleInfinie(void* arg)
{
    int t = 0;
    while (arg == NULL || t < *(int*)arg)
    {
        sleep(1);
        t++;
    }
    return 0;
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
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;
    signalsHandler(&signum, &threadIdentifiers);

    kill(getpid(), SIGHUP);
    CU_ASSERT_EQUAL(signum, SIGHUP);
    signum = 0;
    kill(getpid(), SIGTERM);
    CU_ASSERT_EQUAL(signum, SIGTERM);
    signum = 0;
    kill(getpid(), SIGQUIT);
    CU_ASSERT_EQUAL(signum, SIGQUIT);
    
    /* signum != 0 le signal est ignore*/
    kill(getpid(), SIGTERM);
    CU_ASSERT_EQUAL(signum, SIGQUIT);
}

void testWaitForShutdown()
{
    int signum = 5;
    int t = 2;
    int* tptr = &t;
    ThreadIdentifiers threadIdentifiers = THREAD_IDENTIFIERS_INITIALIZER;
    signalsHandler(&signum, &threadIdentifiers);
    
    pthread_create(&threadIdentifiers.formatThread, NULL, boucleInfinie, tptr);
    pthread_create(&threadIdentifiers.senderThread, NULL, boucleInfinie, tptr);
    CU_ASSERT_EQUAL(waitForShutdown(), 5);
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

#ifndef NDEBUG
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
#else
    /* Run all tests using the CUnit Automated interface */
    CU_set_output_filename("cunit-result/signal");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif
    
    CU_cleanup_registry();
    return CU_get_error();
}
