/* 
 * Tests of Addons Manager
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
#ifndef NDEBUG
#include "CUnit/Basic.h"
#else
#include "CUnit/Automated.h"
#endif
#include "addon.h"
#include "addonList.h"

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
    /* Creer 3 threads AddonFile */
    pthread_t identifier;
    int signum = 0;
    int i = 0;
    
    
    AddonsMgrParams addonsMgrParams = ADDON_PARAMS_INITIALIZER;
    AddonInfo *addonInfo1 = calloc(1, sizeof (AddonInfo));
    AddonInfo *addonInfo2 = calloc(1, sizeof (AddonInfo));
    AddonInfo *addonInfo3 = calloc(1, sizeof (AddonInfo));

    unsigned int idAddon = 2;
    unsigned int period = 60;

    addonInfo1->idAddon = &idAddon;
    addonInfo1->addonParamsList = calloc(1, sizeof (AddonParamsInfo));
    addonInfo1->addonParamsList->period = &period;

    addonInfo2->idAddon = &idAddon;
    addonInfo2->nxt = addonInfo1;
    addonInfo2->addonParamsList = calloc(1, sizeof (AddonParamsInfo));
    addonInfo2->addonParamsList->period = &period;

    addonInfo3->idAddon = &idAddon;
    addonInfo3->nxt = addonInfo2;
    addonInfo3->addonParamsList = calloc(1, sizeof (AddonParamsInfo));
    addonInfo3->addonParamsList->period = &period;

    addonsMgrParams.addonsList = addonInfo3;

    addonsMgrParams.addonsThreads = calloc(1, sizeof (pthread_t));

    CU_ASSERT_FALSE(pthread_create(&identifier, NULL, addon, (void*) &addonsMgrParams));

    sleep(1);

    /* Verifie que les threads sont actifs et les arrete */
    CU_ASSERT_FALSE(pthread_cancel(addonsMgrParams.addonsThreads[0]));
    CU_ASSERT_FALSE(pthread_cancel(addonsMgrParams.addonsThreads[1]));
    CU_ASSERT_FALSE(pthread_cancel(addonsMgrParams.addonsThreads[2]));

    sleep(1);

    /* Verifie que le thread addonsMgrParams est termine */
    CU_ASSERT(pthread_cancel(identifier));
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
    if ((NULL == CU_add_test(pSuite, "testAddon", testAddon)))
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
    CU_set_output_filename("cunit-result/addon");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif

    CU_cleanup_registry();
    return CU_get_error();
}
