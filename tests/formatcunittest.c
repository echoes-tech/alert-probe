/* 
 * Tests of Format Module
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
#include "format.h"

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

void testFormat()
{
    if (1)
    {
        CU_ASSERT(0);
    }
}

void testPopCollectQueue()
{
    int probeID = 1;
    char transportMsgVersion = 2;
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        "127.0.0.1",
        "ea-probe",
        "abcd1234",
        &probeID,
        &transportMsgVersion,
        3,
        NULL
    };
    CollectQueue collectQueue = COLLECT_QUEUE_INITIALIZER;
    int result = 0;
    int valuesLength = 4;
    SDElementQueueElement* sdElementQueueElement = NULL;
    char** values = calloc(valuesLength, sizeof (char*));
    CollectQueueElement* collectQueueElement = calloc(1, sizeof (CollectQueueElement));
    values[0] = strdup("a");
    values[1] = strdup("b");
    values[2] = strdup("c");
    values[3] = strdup("d");
    collectQueueElement->idPlg = 5;
    collectQueueElement->idAsset = 6;
    collectQueueElement->idSrc = 7;
    collectQueueElement->idSearch = 8;
    collectQueueElement->lineNum = 9;
    collectQueueElement->valuesLength = valuesLength;
    collectQueueElement->lotNum = 10;
    collectQueueElement->values = values;
    collectQueueElement->time = 11;
    collectQueueElement->next = NULL;
    collectQueue.first = collectQueueElement;

    result = popCollectQueue(&collectQueue, &sdElementQueue);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " 5-6-7-8-1-10-9=\"YQ==\" 5-6-7-8-2-10-9=\"Yg==\" 5-6-7-8-3-10-9=\"Yw==\" 5-6-7-8-4-10-9=\"ZA==\"");
    CU_ASSERT_EQUAL(collectQueue.first, NULL);
    free(sdElementQueueElement);
}

void testPushSDElementQueue()
{
    int probeID = 1;
    char transportMsgVersion = 2;
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        "127.0.0.1",
        "ea-probe",
        "abcd1234",
        &probeID,
        &transportMsgVersion,
        3,
        NULL
    };
    SDElementQueueElement* sdElementQueueElement = NULL;
    /* ajoute un element dans la queue*/
    int result = pushSDElementQueue(&sdElementQueue, "value=1", 5);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 5);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, "value=1");
    CU_ASSERT_EQUAL(sdElementQueueElement->next, NULL);
    /* ajoute un deuxieme element dans la queue*/
    result = pushSDElementQueue(&sdElementQueue, "value=2", 6);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 5);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, "value=1");
    CU_ASSERT_NOT_EQUAL(sdElementQueueElement->next, NULL);    
    sdElementQueueElement = sdElementQueueElement->next;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 6);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, "value=2");
    CU_ASSERT_EQUAL(sdElementQueueElement->next, NULL);  
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("formatcunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (/*(NULL == CU_add_test(pSuite, "testFormat", testFormat)) ||*/
        (NULL == CU_add_test(pSuite, "testPopCollectQueue", testPopCollectQueue)) ||
        (NULL == CU_add_test(pSuite, "testPushSDElementQueue", testPushSDElementQueue)))
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
