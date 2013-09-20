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
    const char* confPath = "./tests/test.conf";
    int result = 0;
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_EQUAL(conf.probeID, 0);
    CU_ASSERT_EQUAL(conf.enginePort, 443);
    CU_ASSERT_EQUAL(conf.transportProto, 0);
    CU_ASSERT_EQUAL(conf.transportMsgVersion, 2);
#ifdef NDEBUG
    CU_ASSERT_STRING_EQUAL(conf.probePluginDir, "/opt/echoes-alert/probe/etc/plugins/");
#else
    CU_ASSERT_STRING_EQUAL(conf.probePluginDir, "./plugins/");
#endif  
    CU_ASSERT_STRING_EQUAL(conf.engineFQDN, "alert-engine.echoes-tech.com");
    CU_ASSERT_STRING_EQUAL(conf.token, "abcdefghijklmo0123456789");
    
    /* Ticket #358
    char* confPath = "./tests/confcunittest.c";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, errno);
    */
    
    confPath = "azerty";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, errno);
}

void testParseLineConf()
{
    Conf conf = CONF_INITIALIZER;
    char line[MAX_SIZE] = "";
    strcpy(line, "probe_id=1\n");
    parseLineConf(&conf, line);
    CU_ASSERT_EQUAL(conf.probeID, 1); 
    strcpy(line, "engine_port=443\n"); 
    parseLineConf(&conf, line);   
    CU_ASSERT_EQUAL(conf.enginePort, 443); 
    strcpy(line, "transport_proto=5\n"); 
    parseLineConf(&conf, line);   
    CU_ASSERT_EQUAL(conf.transportProto, 5);  
    strcpy(line, "transport_message_version=2\n");  
    parseLineConf(&conf, line);
    CU_ASSERT_EQUAL(conf.transportMsgVersion, 2);  
    strcpy(line, "probe_plugin_dir=/opt/echoes-alert/probe/etc/plugins/\n");
    parseLineConf(&conf, line);
#ifdef NDEBUG
    CU_ASSERT_STRING_EQUAL(conf.probePluginDir, "/opt/echoes-alert/probe/etc/plugins/");
#else
    CU_ASSERT_STRING_EQUAL(conf.probePluginDir, "./plugins/");
#endif      
    strcpy(line, "engine_fqdn=alert-engine.echoes-tech.com\n");
    parseLineConf(&conf, line);
    CU_ASSERT_STRING_EQUAL(conf.engineFQDN, "alert-engine.echoes-tech.com"); 
    strcpy(line, "token=abcdefghijklmo0123456789\n");
    parseLineConf(&conf, line);
    CU_ASSERT_STRING_EQUAL(conf.token, "abcdefghijklmo0123456789");
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
    if ((NULL == CU_add_test(pSuite, "testLoadConf", testLoadConf)) ||
        (NULL == CU_add_test(pSuite, "testParseLineConf", testParseLineConf)))
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
