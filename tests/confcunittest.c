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
    
}

void testLoadConf()
{
    char confDir[] = "conf/echoes-alert.conf";
    Conf conf;
    int result = loadConf(confDir, &conf);
    if(result == 0 && conf.engineFQDN == "192.168.1.48" && conf.enginePort == 7171 && conf.pluginDir == "plugins/" && conf.probeProto == 0)
    {
        CU_PASS("loadConf() succeeded.");
    }
    else
    {
        CU_FAIL("loadConf() failed.");
    }
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

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
