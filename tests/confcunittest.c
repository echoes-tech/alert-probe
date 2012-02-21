/*
 * Configuration Loader CUnit Test
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"

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

void testParseConf()
{
    char line[] = "engine_fqdn=localhost";
    Conf *conf;
    int result = parseLineConf(line, conf);
    CU_ASSERT_EQUAL(result,EXIT_SUCCESS);
}

void testLoadConf()
{
    char confDir[] = "conf/echoes-alert.conf";
    Conf conf;
    int result = loadConf(confDir, &conf);
    CU_ASSERT_STRING_EQUAL(conf.engineFQDN,"localhost");
    CU_ASSERT_EQUAL(conf.enginePort,7171);
    CU_ASSERT_STRING_EQUAL(conf.pluginDir,"plugins/");
    CU_ASSERT_EQUAL(conf.probeProto,0);
    CU_ASSERT_EQUAL(result,EXIT_SUCCESS);
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
    if ((NULL == CU_add_test(pSuite, "testParseConf", testParseConf)) ||
        (NULL == CU_add_test(pSuite, "testLoadConf", testLoadConf)))
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
