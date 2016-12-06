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
    confPath = "./unit_tests/conf/complete.conf";
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
    confPath = "./unit_tests/conf/minimum.conf";
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
    confPath = "./unit_tests/conf/without_probe_id.conf";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_FAILURE); 
    
    
    /* Test le chargement d'un fichier de conf ne contenant pas token */
    confPath = "./unit_tests/conf/without_token.conf";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);
    
    
    /* Test le chargement d'un fichier invalide */
    confPath = "./unit_tests/confcunittest.c";
    result = loadConf(&conf, confPath);
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);
    
    
    /* Test le chargement d'un fichier n'existant pas */
    confPath = "./unit_tests/azerty";
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
