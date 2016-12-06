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
#include <glib.h>
#include <gio/gio.h>
#ifndef NDEBUG
#include "CUnit/Basic.h"
#else
#include "CUnit/Automated.h"
#endif
#include "sender.h"
#include "utilUnitTest.h"

/*
 * CUnit Test Suite
 */

typedef struct ParamAndReturn ParamAndReturn;

struct ParamAndReturn
{
    char **value;
    GMainLoop *loop;
    guint16 port;
    int lenght, cpt;
};

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

/**
 * S'active a la reception d'une connection par le serveur()
 * Demande au serveur de s'arreter quand lenght = cpt
 */
static gboolean incoming_callback(GSocketService *service,
                                  GTlsConnection *connection,
                                  GObject *source_object,
                                  gpointer data)
{
    GError * error = NULL;
    GTlsCertificate *tlscert = NULL;
    const char *tls_cert_file = "./unit_tests/ssl/servunittest.crt";
    const char *tls_cert_key = "./unit_tests/ssl/servunittest.key";
    GIOStream *tls_conn;
    GInputStream * istream = NULL;
    gchar *message = calloc(1024, sizeof (char));
    ParamAndReturn *paramAndReturn = (ParamAndReturn*) data;

    /* authentification tsl */
    tlscert = g_tls_certificate_new_from_files(tls_cert_file, tls_cert_key, &error);
    if (!tlscert)
    {
        g_printerr("Could not read server certificate '%s': %s\n", tls_cert_file, error->message);
        return 1;
    }
    tls_conn = g_tls_server_connection_new(G_IO_STREAM(connection), tlscert, &error);
    if (!tls_conn)
    {
        g_printerr("Could not create TLS connection: %s\n", error->message);
        return 1;
    }
    if (!g_tls_connection_handshake(G_TLS_CONNECTION(tls_conn), NULL, &error))
    {
        g_printerr("Error during TLS handshake: %s\n", error->message);
        return 1;
    }

    /* lecture input stream */
    istream = g_io_stream_get_input_stream(tls_conn);
    g_input_stream_read(istream, message, 1024, NULL, &error);
    if (error != NULL)
    {
        g_printerr("Error during input stream read: %s\n", error->message);
        return 1;
    }

    paramAndReturn->value[paramAndReturn->cpt++] = strdup(message);
    free(message);

    /* Demande au serveur de s'arreter */
    if (paramAndReturn->lenght == paramAndReturn->cpt)
        g_main_loop_quit(paramAndReturn->loop);

    return FALSE;
}

/**
 * Serveur testant l'envoi des messages par sendMessage()
 */
static void *server(void *data)
{
    GError *error = NULL;
    ParamAndReturn *paramAndReturn = (ParamAndReturn*) data;

    /* create the new socketservice */
    GSocketService *service = g_socket_service_new();

    /* connect to the port */
    paramAndReturn->port = g_socket_listener_add_any_inet_port((GSocketListener*) service, NULL, &error);
    if (error != NULL)
    {
        g_printerr("Error during add inet port: %s\n", error->message);
    }

    /* listen to the 'incoming' signal */
    g_signal_connect(service, "incoming", G_CALLBACK(incoming_callback), data);

    /* start the socket service */
    g_socket_service_start(service);

    /* Fonction bloquante, debloquee par g_main_loop_quit() dans incoming_callback() */
    g_main_loop_run(paramAndReturn->loop);

    /* Arrete l'ecoute */
    g_socket_service_stop(service);
    g_socket_listener_close((GSocketListener*) service);

    return 0;
}

void testPopSDElementQueue()
{
    const char* address = "127.0.0.1";
    unsigned char protocol = 0;
    unsigned short msgID = 123;
    pthread_t identifier;

    char* regex = "^<118>1 [0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{6}Z ABC123 DEF[0-9]+GHI\\]\n$";
    char* regex2 = "^<118>1 [0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{6}Z JKL123 MNO[0-9]+PQR\\]\n$";

    int result = 0;
    ParamAndReturn paramAndReturn = {calloc(2, sizeof (char*)), g_main_loop_new(NULL, FALSE), 0, 1, 0};

    /* Creer une queue d'envoi contenant deux elements */
    unsigned int probeID = 1;
    unsigned char transportMsgVersion = 2;
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        "127.0.0.1",
        "ea-probe",
        "abcd1234",
        &probeID,
        &transportMsgVersion,
        3,
        NULL
    };
    SDElementQueueElement* sdElementQueueElement = calloc(1, sizeof (SDElementQueueElement));
    SDElementQueueElement* sdElementQueueElement2 = calloc(1, sizeof (SDElementQueueElement));
    strcpy(sdElementQueueElement->beforeMsgID, "ABC");
    strcpy(sdElementQueueElement->afterMsgID, "DEF");
    sdElementQueueElement->afterOffset = strdup("GHI");
    sdElementQueueElement->time = 1000;
    sdElementQueue.first = sdElementQueueElement;
    strcpy(sdElementQueueElement2->beforeMsgID, "JKL");
    strcpy(sdElementQueueElement2->afterMsgID, "MNO");
    sdElementQueueElement2->afterOffset = strdup("PQR");
    sdElementQueueElement2->time = 1000;
    sdElementQueueElement->next = sdElementQueueElement2;

    /* lance le serveur qui va attendre un premier message */
    pthread_create(&identifier, NULL, server, &paramAndReturn);
    
    sleep(1);

    /* depile le premier element */
    result = popSDElementQueue(address, &paramAndReturn.port, &protocol, &sdElementQueue, &msgID);

    /* attend que le serveur est recu le premier message*/
    pthread_join(identifier, NULL);

    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    /* Test la forme du message envoye par sendMessage()*/
    CU_ASSERT(regex_compare(regex, paramAndReturn.value[0]));
    /* Verifie que la queue n'est pas vide */
    CU_ASSERT_NOT_EQUAL(sdElementQueue.first, NULL);



    /* lance le serveur qui va attendre un second message */
    paramAndReturn.lenght = 2;
    pthread_create(&identifier, NULL, server, &paramAndReturn);
    
    sleep(1);

    /* depile le second element */
    result = popSDElementQueue(address, &paramAndReturn.port, &protocol, &sdElementQueue, &msgID);

    /* attend que le serveur est recu le second message*/
    pthread_join(identifier, NULL);

    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    /* Test la forme du message envoye par sendMessage()*/
    CU_ASSERT(regex_compare(regex2, paramAndReturn.value[1]));
    /* Verifie que la queue est vide */
    CU_ASSERT_EQUAL(sdElementQueue.first, NULL);

    free(paramAndReturn.value[0]);
    free(paramAndReturn.value[1]);
    free(paramAndReturn.value);
}

void testSendMessage_Success()
{
    const char* address = "127.0.0.1";
    unsigned char protocol = 0;
    const char* beforeMsgID = "ABC";
    unsigned short msgID = 123;
    const char* afterMsgID = "DEF";
    time_t collectTime = 1000;
    const char* afterOffset = "GHI";
    pthread_t identifier;

    char* regex = "^<118>1 [0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{6}Z ABC123 DEF[0-9]+GHI\\]\n$";

    int result = 0;
    ParamAndReturn paramAndReturn = {calloc(1, sizeof (char*)), g_main_loop_new(NULL, FALSE), 0, 1, 0};

    /* lance le serveur qui va attendre un message */
    pthread_create(&identifier, NULL, server, &paramAndReturn);

    sleep(1);

    /* envoi un message */
    result = sendMessage(address, &paramAndReturn.port, &protocol, beforeMsgID, &msgID, afterMsgID, collectTime, afterOffset);

    /* attend que le serveur est recu le message*/
    pthread_join(identifier, NULL);

    /* Test que la methode se soit bien executee */
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    /* Test la forme du message envoye par sendMessage()*/
    CU_ASSERT(regex_compare(regex, paramAndReturn.value[0]));

    free(paramAndReturn.value[0]);
    free(paramAndReturn.value);
}

void testSendMessage_UnavailableHost()
{
    const char* address = "127.0.0.1";
    unsigned char protocol = 0;
    const char* beforeMsgID = "ABC";
    unsigned short msgID = 123;
    const char* afterMsgID = "DEF";
    time_t collectTime = 1380036380;
    const char* afterOffset = "GHI";
    guint16 port = 1900;
    int result = 0;

    char* regex = "^.*[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\+[0-9]{4} 127.0.0.1 ea-probe 5 - Could not connect to 127.0.0.1: Connection refused: 127.0.0.1:1900\n$";

    LogParams logParams = {
        "127.0.0.1",
        "ea-probe",
        5,
        getOuputFilePath()
    };

    g_message_defineHandler(&logParams);

    readStdout_init();

    /* envoi un message */
    result = sendMessage(address, &port, &protocol, beforeMsgID, &msgID, afterMsgID, collectTime, afterOffset);

    /* Test que l'envoi n'est pu etre effectue */
    CU_ASSERT_EQUAL(result, EXIT_FAILURE);
    /* Test forme de la sortie */
    CU_ASSERT(outputFile_compare(regex));

    readStdout_end();
}

void testHeartBeat()
{
    /* Creation d'une queue d'envoi vide */
    unsigned int probeID = 1;
    unsigned char transportMsgVersion = 2;
    SDElementQueue sdElementQueue = {
            PTHREAD_MUTEX_INITIALIZER,
            "127.0.0.1",
            "ea-probe",
            "abcd1234",
            &probeID,
            &transportMsgVersion,
            3,
            NULL
    };
    
    /* Test if first call returns TRUE */
    CU_ASSERT_TRUE(heartBeat(&sdElementQueue));

    /* Test if first element is not NULL */
    CU_ASSERT_NOT_EQUAL(sdElementQueue.first, NULL);

    /* Test if non-void Queue returns FALSE */
    CU_ASSERT_FALSE(heartBeat(&sdElementQueue));

    sdElementQueue.first = NULL;
    sleep(HEARTBEAT + 1); /* HEARTBEAT or HEARTBEAT + 1 ? */
    
    /* Test if void Queue after an HEARTBEAT returns TRUE */
    CU_ASSERT_TRUE(heartBeat(&sdElementQueue));
}

void testSender()
{
    const char* address = "127.0.0.1";
    unsigned char protocol = 0;
    pthread_t sender_identifier, server_identifier;
    int signum = 0;

    char* regex = "^<118>1 [0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{6}Z ABC1 DEF[0-9]+GHI\\]\n$";
    char* regex2 = "^<118>1 [0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{6}Z JKL2 MNO[0-9]+PQR\\]\n$";

    ParamAndReturn paramAndReturn = {calloc(2, sizeof (char*)), g_main_loop_new(NULL, FALSE), 0, 2, 0};

    /* Creer une queue d'envoi vide */
    unsigned int probeID = 1;
    unsigned char transportMsgVersion = 2;
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        "127.0.0.1",
        "ea-probe",
        "abcd1234",
        &probeID,
        &transportMsgVersion,
        3,
        NULL
    };
    SenderParams senderParams = {
        &sdElementQueue,
        address,
        &paramAndReturn.port,
        0,
        &protocol,
        &signum
    };
    SDElementQueueElement* sdElementQueueElement = calloc(1, sizeof (SDElementQueueElement));
    SDElementQueueElement* sdElementQueueElement2 = calloc(1, sizeof (SDElementQueueElement));
    strcpy(sdElementQueueElement->beforeMsgID, "ABC");
    strcpy(sdElementQueueElement->afterMsgID, "DEF");
    sdElementQueueElement->afterOffset = strdup("GHI");
    sdElementQueueElement->time = 1000;
    sdElementQueue.first = sdElementQueueElement;
    strcpy(sdElementQueueElement2->beforeMsgID, "JKL");
    strcpy(sdElementQueueElement2->afterMsgID, "MNO");
    sdElementQueueElement2->afterOffset = strdup("PQR");
    sdElementQueueElement2->time = 1000;
    sdElementQueueElement->next = sdElementQueueElement2;


    /* lance le serveur qui va attendre la reception de deux messages */
    pthread_create(&server_identifier, NULL, server, &paramAndReturn);

    sleep(1);

    /* lance sender qui va depiler et envoyer les deux elements */
    CU_ASSERT_FALSE(pthread_create(&sender_identifier, NULL, sender, (void*) &senderParams));

    /* le serveur s'arrete apres avoir recu deux messages */
    pthread_join(server_identifier, NULL);

    /* Test la forme du message envoye par sendMessage()*/
    CU_ASSERT(regex_compare(regex, paramAndReturn.value[0]));

    /* Test la forme du message envoye par sendMessage()*/
    CU_ASSERT(regex_compare(regex2, paramAndReturn.value[1]));

    /* Verifie que la queue est vide */
    CU_ASSERT_EQUAL(sdElementQueue.first, NULL);

    free(paramAndReturn.value[0]);
    free(paramAndReturn.value[1]);
    free(paramAndReturn.value);

    signum = SIGTERM;
    sleep(1);
    /* Test arret du thread apres reception d'un signal */
    CU_ASSERT_EQUAL(pthread_cancel(sender_identifier), ESRCH);
}

int main()
{
    CU_pSuite pSuite = NULL;

    g_type_init();

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("sendercunittest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testSendMessage_Success", testSendMessage_Success)) ||
        (NULL == CU_add_test(pSuite, "testSendMessage_UnavailableHost", testSendMessage_UnavailableHost)) ||
        (NULL == CU_add_test(pSuite, "testPopSDElementQueue", testPopSDElementQueue)) ||
        (NULL == CU_add_test(pSuite, "testSender", testSender)) ||
        (NULL == CU_add_test(pSuite, "testHeartBeat", testHeartBeat)))
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
    CU_set_output_filename("cunit-result/sender");
    CU_list_tests_to_file();
    CU_automated_run_tests();
#endif

    CU_cleanup_registry();
    return CU_get_error();
}
