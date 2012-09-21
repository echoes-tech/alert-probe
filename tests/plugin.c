/* 
 * Tests of Plug-in Manager
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

#include <stdlib.h>
#include <stdio.h>

#include <glib.h>

#include "plugin.h"

static const gchar *test_good_plugin_filename = "Linux-System.json";
static const gchar *test_bad_plugin_filename = "Linux-System.jso";

static void test_verifExt(void)
{
    g_assert(verifExt(test_good_plugin_filename) == TRUE);
    g_assert(verifExt(test_bad_plugin_filename) == FALSE);
}

static void test_addBackslash(void)
{
    gchar string[255] = "test\\*123321$!!\\";
    addBackslash(string);
    g_assert_cmpstr(string, ==, "test\\\\*123321$!!\\\\");
}

/**
 * Main function of Tests of Plug-in Manager
 * @return Tests results
 */
int main(int argc, char *argv[])
{
    g_type_init();
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/plugin/verifExt", test_verifExt);
    g_test_add_func("/plugin/addBackslash", test_addBackslash);

    return g_test_run();
}