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
#include <signal.h>
#ifndef NDEBUG
#include "CUnit/Basic.h"
#else
#include "CUnit/Automated.h"
#endif
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
    SDElementQueueElement* sdElementQueueElement = NULL;
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
    char** values = calloc(1, sizeof (char*));
    CollectQueueElement* collectQueueElement = calloc(1, sizeof (CollectQueueElement));
    values[0] = strdup("a");
    collectQueueElement->idPlg = 1;
    collectQueueElement->idAsset = 2;
    collectQueueElement->idSrc = 3;
    collectQueueElement->idSearch = 4;
    collectQueueElement->lineNum = 5;
    collectQueueElement->valuesLength = 1;
    collectQueueElement->lotNum = 6;
    collectQueueElement->values = values;
    collectQueueElement->time = 7;
    collectQueueElement->next = NULL;
    collectQueue.first = collectQueueElement;
    char** values2 = calloc(1, sizeof (char*));
    CollectQueueElement* collectQueueElement2 = calloc(1, sizeof (CollectQueueElement));
    values2[0] = strdup("b");
    collectQueueElement2->idPlg = 8;
    collectQueueElement2->idAsset = 9;
    collectQueueElement2->idSrc = 10;
    collectQueueElement2->idSearch = 11;
    collectQueueElement2->lineNum = 12;
    collectQueueElement2->valuesLength = 1;
    collectQueueElement2->lotNum = 13;
    collectQueueElement2->values = values2;
    collectQueueElement2->time = 14;
    collectQueueElement2->next = NULL;
    collectQueueElement->next = collectQueueElement2;
    int signum = 0;
    FormatParams formatParams = {
        &collectQueue,
        &sdElementQueue,
        &signum
    };
    pthread_t identifier;
    
    CU_ASSERT_FALSE(pthread_create(&identifier, NULL, format, (void*) &formatParams));
    sleep(1);
    signum = SIGTERM;
    sleep(1);
    /* Test arret du thread apres reception d'un signal */
    CU_ASSERT_EQUAL(pthread_cancel(identifier), ESRCH);
    
    /* Test formatage des deux elements */
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 7);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " 1-2-3-4-1-6-5=\"YQ==\"");
    CU_ASSERT_NOT_EQUAL(sdElementQueueElement->next, NULL);
    sdElementQueueElement = sdElementQueueElement->next;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 14);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " 8-9-10-11-1-13-12=\"Yg==\"");
    CU_ASSERT_EQUAL(sdElementQueueElement->next, NULL);    
}

void testPopCollectQueue()
{
    SDElementQueueElement* sdElementQueueElement = NULL;
    int result = 0;
    
    /* Creer une queue d'envoi vide */
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
    
    /* Creer une queue de collecte contenant deux elements */
    CollectQueue collectQueue = COLLECT_QUEUE_INITIALIZER;
    int valuesLength = 4;
    CollectQueueElement* collectQueueElement = calloc(1, sizeof (CollectQueueElement));
    char** values = calloc(valuesLength, sizeof (char*));
    CollectQueueElement* collectQueueElement2 = calloc(1, sizeof (CollectQueueElement));
    char** values2 = calloc(valuesLength, sizeof (char*));
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
    values2[0] = strdup("e");
    values2[1] = strdup("f");
    values2[2] = strdup("g");
    values2[3] = strdup("h");
    collectQueueElement2->idPlg = 5;
    collectQueueElement2->idAsset = 6;
    collectQueueElement2->idSrc = 7;
    collectQueueElement2->idSearch = 8;
    collectQueueElement2->lineNum = 9;
    collectQueueElement2->valuesLength = valuesLength;
    collectQueueElement2->lotNum = 10;
    collectQueueElement2->values = values2;
    collectQueueElement2->time = 11;
    collectQueueElement2->next = NULL;
    collectQueueElement->next = collectQueueElement2;

    /* Test depile un element de la queue de collecte */
    result = popCollectQueue(&collectQueue, &sdElementQueue);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " 5-6-7-8-1-10-9=\"YQ==\" 5-6-7-8-2-10-9=\"Yg==\" 5-6-7-8-3-10-9=\"Yw==\" 5-6-7-8-4-10-9=\"ZA==\"");
    CU_ASSERT_NOT_EQUAL(collectQueue.first, NULL);
    
    /* Test depile second element de la queue de collecte */
    result = popCollectQueue(&collectQueue, &sdElementQueue);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " 5-6-7-8-1-10-9=\"YQ==\" 5-6-7-8-2-10-9=\"Yg==\" 5-6-7-8-3-10-9=\"Yw==\" 5-6-7-8-4-10-9=\"ZA==\"");
    sdElementQueueElement = sdElementQueueElement->next;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " 5-6-7-8-1-10-9=\"ZQ==\" 5-6-7-8-2-10-9=\"Zg==\" 5-6-7-8-3-10-9=\"Zw==\" 5-6-7-8-4-10-9=\"aA==\"");
    CU_ASSERT_EQUAL(collectQueue.first, NULL);
    
    /* Test depile alors que la queue de collecte est vide */ 
    result = popCollectQueue(&collectQueue, &sdElementQueue);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " 5-6-7-8-1-10-9=\"YQ==\" 5-6-7-8-2-10-9=\"Yg==\" 5-6-7-8-3-10-9=\"Yw==\" 5-6-7-8-4-10-9=\"ZA==\"");
    sdElementQueueElement = sdElementQueueElement->next;
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, " 5-6-7-8-1-10-9=\"ZQ==\" 5-6-7-8-2-10-9=\"Zg==\" 5-6-7-8-3-10-9=\"Zw==\" 5-6-7-8-4-10-9=\"aA==\"");
    CU_ASSERT_EQUAL(collectQueue.first, NULL);
    
    free(sdElementQueue.first);
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
    
    /* Test ajoute un element dans la queue*/
    int result = pushSDElementQueue(&sdElementQueue, "value=1", 5);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    sdElementQueueElement = sdElementQueue.first;
    CU_ASSERT_EQUAL(sdElementQueueElement->time, 5);
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->beforeMsgID, "127.0.0.1 ea-probe 3 ID");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterMsgID, "[prop@40311 ver=2 probe=1 token=\"abcd1234\"][res1@40311 offset=");
    CU_ASSERT_STRING_EQUAL(sdElementQueueElement->afterOffset, "value=1");
    CU_ASSERT_EQUAL(sdElementQueueElement->next, NULL);
    
    /* Test ajoute un deuxieme element dans la queue*/
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
    if ((NULL == CU_add_test(pSuite, "testFormat", testFormat)) ||
        (NULL == CU_add_test(pSuite, "testPopCollectQueue", testPopCollectQueue)) ||
        (NULL == CU_add_test(pSuite, "testPushSDElementQueue", testPushSDElementQueue)))
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
    CU_set_output_filename("cunit-result/format");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif

    CU_cleanup_registry();
    return CU_get_error();
}
