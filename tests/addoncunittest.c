/* 
 * Test of addon.c
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
#include "CUnit/Basic.h"
#include "addon/addon.h"
#include "addon/addonList.h"

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

void testAddon()
{
    if (1 /*check result*/)
    {
        CU_ASSERT(0);
    }
}

void testAddonSleep()
{
    unsigned int period;
    time_t t1, t2;
    period = 3;
    time(&t1);
    addonSleep(period);
    time(&t2);
    CU_ASSERT((t2 - t1) <= period);
    CU_ASSERT(((int) (t2 / period) * period) == t2);
}

void testIncreaseLotNum()
{
    pthread_mutex_t mutexPtr = PTHREAD_MUTEX_INITIALIZER;
    unsigned short lotNumPtr = 0;
    unsigned short result = 0;
    lotNumPtr = 1;
    result = increaseLotNum(&mutexPtr, &lotNumPtr);
    CU_ASSERT_EQUAL(lotNumPtr, 2);
    CU_ASSERT_EQUAL(result, 2);
    lotNumPtr = 65534;
    result = increaseLotNum(&mutexPtr, &lotNumPtr);
    CU_ASSERT_EQUAL(lotNumPtr, 0);
    CU_ASSERT_EQUAL(result, 65535);

}

void testPushCollectQueue()
{
    CollectQueue collectQueue = COLLECT_QUEUE_INITIALIZER;
    CollectQueueElement *collectQueueElement = NULL;
    unsigned int idPlg = 1, idAsset = 2, idSrc = 3, idSearch = 4;
    IDInfo idInfo = {&idPlg, &idAsset, &idSrc, &idSearch, NULL};
    IDList idList = &idInfo;
    const unsigned short lotNum = 9;
    const unsigned int lineNum = 10;
    const unsigned int valuesLength = 4;
    time_t time = 12;
    int result = 0;
    char** values = calloc(valuesLength, sizeof (char*));
    values[0] = strdup("a");
    values[1] = strdup("b");
    values[2] = strdup("c");
    values[3] = strdup("d");
    result = pushCollectQueue(&collectQueue, &idList, lotNum, lineNum, valuesLength, values, time);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    collectQueueElement = collectQueue.first;
    CU_ASSERT_EQUAL(collectQueueElement->idPlg, 1);
    CU_ASSERT_EQUAL(collectQueueElement->idAsset, 2);
    CU_ASSERT_EQUAL(collectQueueElement->idSrc, 3);
    CU_ASSERT_EQUAL(collectQueueElement->idSearch, 4);
    CU_ASSERT_EQUAL(collectQueueElement->lotNum, 9);
    CU_ASSERT_EQUAL(collectQueueElement->lineNum, 10);
    CU_ASSERT_EQUAL(collectQueueElement->valuesLength, 4);
    CU_ASSERT_EQUAL(collectQueueElement->time, 12);
    CU_ASSERT_STRING_EQUAL(collectQueueElement->values[0], "a");
    CU_ASSERT_STRING_EQUAL(collectQueueElement->values[1], "b");
    CU_ASSERT_STRING_EQUAL(collectQueueElement->values[2], "c");
    CU_ASSERT_STRING_EQUAL(collectQueueElement->values[3], "d");
    CU_ASSERT_EQUAL(collectQueueElement->next, NULL);
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("addoncunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ( /*(NULL == CU_add_test(pSuite, "testAddon", testAddon)) ||*/
        (NULL == CU_add_test(pSuite, "testAddonSleep", testAddonSleep)) ||
        (NULL == CU_add_test(pSuite, "testIncreaseLotNum", testIncreaseLotNum)) ||
        (NULL == CU_add_test(pSuite, "testPushCollectQueue", testPushCollectQueue)))
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
