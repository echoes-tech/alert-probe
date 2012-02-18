/*
 * File:   sendercunittest.c
 * Author: Florent Poinsaut
 *
 * Created on 18 févr. 2012, 15:34:42
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "sender.h"

/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void testSenderUDP() {
    int res = sender("127.0.0.1", 7171, 1);
    CU_ASSERT_EQUAL(res,EXIT_SUCCESS);
}

void testSenderTCP() {
    //TODO
    int res = sender("127.0.0.1", 7171, 0);
    CU_ASSERT_EQUAL(res,EXIT_SUCCESS);
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("sendercunittest", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testSenderUDP", testSenderUDP)) /*||
            (NULL == CU_add_test(pSuite, "testSenderTCP", testSenderTCP))*/) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
