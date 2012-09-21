/* 
 * Tests of Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 12/09/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include <glib.h>

#include "addon/addon.h"

static CollectQueue test_collectQueue = COLLECT_QUEUE_INITIALIZER;
static unsigned int test_idPlg = 12;
static unsigned int test_idAsset = 258;
static unsigned int test_idSrc = 6456;
static unsigned int test_idSearch = 6;
static IDInfo test_idInfo = {
    &test_idPlg,
    &test_idAsset,
    &test_idSrc,
    &test_idSearch,
    NULL
};
static IDList test_idList = &test_idInfo;
static unsigned int test_valueNum = 4;
static unsigned short test_lotNum = 641;
static unsigned int test_lineNum = 28;
static char test_res[] = "retyguij%éèopk";

static void test_pushCollectQueue(void)
{
    time_t now;
    time(&now);

    g_assert(pushCollectQueue(
                              &test_collectQueue,
                              &test_idList,
                              test_valueNum,
                              test_lotNum,
                              test_lineNum,
                              test_res,
                              now
                              ) == EXIT_SUCCESS);
    
    if (test_collectQueue.first != NULL)
    {
        CollectQueueElement *popedElement = test_collectQueue.first;
        
        g_assert_cmpint(popedElement->idPlg, ==, test_idPlg);
        g_assert_cmpint(popedElement->idAsset, ==, test_idAsset);
        g_assert_cmpint(popedElement->idSrc, ==, test_idSrc);
        g_assert_cmpint(popedElement->idSearch, ==, test_idSearch);
        g_assert_cmpint(popedElement->valueNum, ==, test_valueNum);
        g_assert_cmpint(popedElement->lotNum, ==, test_lotNum);
        g_assert_cmpint(popedElement->lineNum, ==, test_lineNum);
        g_assert_cmpstr(popedElement->value, ==, test_res);
        g_assert(popedElement->time == now);

        test_collectQueue.first = popedElement->next;
        free(popedElement);
    }
    else
    {
        g_test_fail();
    }
}

/**
 * Main function of Tests of Addons Manager
 * @return Tests results
 */
int main(int argc, char** argv)
{
    g_type_init();
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/plugin/pushCollectQueue", test_pushCollectQueue);

    return g_test_run();
}

