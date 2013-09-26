/* 
 * Tests of Configuration Loader
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
#include "conf.h"

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

void testLoadConf()
{
    Conf conf = CONF_INITIALIZER;
    char* confPath = calloc(256, sizeof(char));
    int result = 0;
    
    /* Test le chargement d'un fichier de conf contenant toutes les infos */
    confPath = "./tests/conf/complete.conf";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_EQUAL(conf.probeID, 10);
    CU_ASSERT_STRING_EQUAL(conf.token, "abcdefghijklmo0123456789");
#ifdef NDEBUG
    CU_ASSERT_STRING_EQUAL(conf.probePluginDir, "./test/");
#else
    CU_ASSERT_STRING_EQUAL(conf.probePluginDir, "./plugins/");
#endif  
    CU_ASSERT_EQUAL(conf.transportProto, 11);
    CU_ASSERT_EQUAL(conf.transportMsgVersion, 12);
    CU_ASSERT_STRING_EQUAL(conf.engineFQDN, "127.0.0.1");
    CU_ASSERT_EQUAL(conf.enginePort, 13);
    
    
    /* Test le chargement d'un fichier de conf contenant uniquement id_probe et token */
    confPath = "./tests/conf/minimum.conf";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_EQUAL(conf.probeID, 10);
    CU_ASSERT_STRING_EQUAL(conf.token, "abcdefghijklmo0123456789");
#ifdef NDEBUG
    CU_ASSERT_STRING_EQUAL(conf.probePluginDir, "/opt/echoes-alert/probe/etc/plugins/");
#else
    CU_ASSERT_STRING_EQUAL(conf.probePluginDir, "./plugins/");
#endif  
    CU_ASSERT_EQUAL(conf.transportProto, 0);
    CU_ASSERT_EQUAL(conf.transportMsgVersion, 2);
    CU_ASSERT_STRING_EQUAL(conf.engineFQDN, "alert-engine.echoes-tech.com");
    CU_ASSERT_EQUAL(conf.enginePort, 443);    
    
    
    /* Test le chargement d'un fichier de conf ne contenant pas id_probe */
    confPath = "./tests/conf/without_probe_id.conf";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_FAILURE); 
    
    
    /* Test le chargement d'un fichier de conf ne contenant pas token */
    confPath = "./tests/conf/without_token.conf";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);
    
    
    /* Test le chargement d'un fichier invalide */
    confPath = "./tests/confcunittest.c";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);
    
    
    /* Test le chargement d'un fichier n'existant pas */
    confPath = "./tests/azerty";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("confcunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testLoadConf", testLoadConf)))
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
    CU_set_output_filename("cunit-result/conf");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif

    CU_cleanup_registry();
    return CU_get_error();
}
