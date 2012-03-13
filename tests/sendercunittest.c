/*
 * Sender Module CUnit Test
 * @author ECHOES Technologies (FPO)
 * @date 18/02/2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "sender.h"

/*
 * CUnit Test Suite
 */

int initSuite(void)
{
    return 0;
}

int cleanSuite(void)
{
    return 0;
}

void testSenderUDP()
{
    int port = 7171;
    int proto = 1;
    int res = sender("127.0.0.1", &port, &proto);
    CU_ASSERT_EQUAL(res, EXIT_SUCCESS);
}

void testSenderTCP()
{
    //TODO
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("sendercunittest", initSuite, cleanSuite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testSenderUDP", testSenderUDP)) /*||
            (NULL == CU_add_test(pSuite, "testSenderTCP", testSenderTCP))*/)
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
