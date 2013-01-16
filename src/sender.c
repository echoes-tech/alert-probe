/* 
 * Sender Module
 * @author ECHOES Technologies (FPO)
 * @date 14/02/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "sender.h"

static void init(void)
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (err < 0)
    {
        puts("WSAStartup failed !");
        return (EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}

static int initConnection(
                          const char *address,
                          unsigned short *port,
                          unsigned char *protocol,
                          SOCKADDR_IN *sin,
                          SOCKET *sock
                          )
{
    /* Get info of Engines FQDN */
    Hostent *hostinfo = gethostbyname(address);
    /* Whether no info, return an error by stopping probe */
    if (hostinfo == NULL)
    {
        g_warning("Unknown host %s.", address);
        return EXIT_FAILURE;
    }

    if (*protocol == 1)
    {
        /* UDP so SOCK_DGRAM */
        *sock = socket(AF_INET, SOCK_DGRAM, 0);
    }
    else
    {
        /* TCP so SOCK_STREAM */
        *sock = socket(AF_INET, SOCK_STREAM, 0);
    }

    if (*sock == INVALID_SOCKET)
    {
        g_critical("Critical: %s", strerror(errno));
        return errno;
    }

    /* Emission info */
    sin->sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    sin->sin_port = htons(*port);
    sin->sin_family = AF_INET;

    if (*protocol == 0)
    {
        /* Open a connection */
        if (connect(*sock, (SOCKADDR *) sin, sizeof (SOCKADDR)) == SOCKET_ERROR)
        {
            g_critical("Critical: %s", strerror(errno));
            return errno;
        }
    }

    return EXIT_SUCCESS;
}

static int endConnection(SOCKET *sock)
{
    if (closesocket(*sock))
    {
        g_critical("Critical: %s", strerror(errno));
        return errno;
    }
    return EXIT_SUCCESS;
}

int sendMessage(
                const char *address,
                unsigned short *port,
                unsigned char *protocol,
                const char *beforeMsgID,
                unsigned short *msgID,
                const char *afterMsgID,
                time_t collectTime,
                const char *afterOffset
                )
{
    
    SOCKADDR_IN sin = {0}; /* Emission info */
    SOCKET sock; /* Socket */

    time_t now;
    GTimeVal g_time;
    char completMsg[480], *timeVal = NULL;

    /* Adding PRI, VERSION and TIMESTAMP */
    time(&now);
    g_get_current_time(&g_time);
    timeVal = g_time_val_to_iso8601(&g_time);

    /* Caution: keep the Line Feed to work with TCP */
    snprintf(
             completMsg,
             480,
             "<118>1 %s %s%d %s%d%s]\n",
             timeVal,
             beforeMsgID,
             *msgID,
             afterMsgID,
             (int)difftime (now, collectTime),
             afterOffset);

    free(timeVal);
    timeVal = NULL;

#ifndef NDEBUG
    printf("%s", completMsg);
#endif
    
    /* TCP/TLS or UDP */
    if (*protocol == 0)
    {
        GError *error = NULL;

        /* Create a new connection */
        GSocketConnection *connection = NULL;
        GSocketClient *client = g_socket_client_new();
        GTlsCertificateFlags flags;

        GOutputStream *ostream = NULL;

        gchar uri[265] = "simple://";
        strcat(uri, address);

        /* Following flags are used to allow self-signed certificates */
        flags = g_socket_client_get_tls_validation_flags (client);
        flags &= ~(G_TLS_CERTIFICATE_UNKNOWN_CA);
        g_socket_client_set_tls_validation_flags (client, flags);

        /* Set TLS */
        g_socket_client_set_tls(client, TRUE);

        /* Connect to the host */
        connection = g_socket_client_connect_to_uri(
                                                    client,
                                                    uri,
                                                    (guint16) *port,
                                                    NULL,
                                                    &error
                                                    );

        /* don't forget to check for errors */
        if (error != NULL)
        {
            g_object_unref(client);
            g_warning("%s: %s:%u", error->message, address, *port);
            return EXIT_FAILURE;
        }

        /* use the connection */
        ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));

        g_output_stream_write(ostream,
                              completMsg,
                              strlen(completMsg),
                              NULL,
                              &error);

        g_object_unref(connection);
        g_object_unref(client);
        
        /* don't forget to check for errors */
        if (error != NULL)
        {
            g_warning("%s: %s:%u", error->message, address, *port);
            return EXIT_FAILURE;
        }
    }
    else
    {
        /* Init just for Win32 */
        init();

        /* Creating the Socket */
        if (initConnection(address, port, protocol, &sin, &sock))
        {
            g_warning("%s: %s:%u", strerror(errno), address, *port);
            return errno;
        }

        /* Sending data */
        if (sendto(sock, completMsg, strlen(completMsg), 0, (SOCKADDR *) & sin, sizeof sin) < 0)
        {
            g_warning("%s: %s:%u", strerror(errno), address, *port);
            return errno;
        }

        /* Closing the socket */
        if (endConnection(&sock))
        {
            g_warning("%s: %s:%u", strerror(errno), address, *port);
            return errno;
        }

        /* End just for Win32 */
        end();
    }

    return EXIT_SUCCESS;
}

int popSDElementQueue(
                      const char *address,
                      unsigned short *port,
                      unsigned char *protocol,
                      SDElementQueue *sdElementQueue,
                      unsigned short *msgID
                      )
{
    if (sdElementQueue == NULL)
        g_error("Error: Queue of collected data unavailable");

    /* Debut de la zone protegee. */
    pthread_mutex_lock (& sdElementQueue->mutex);
    
    /* On vérifie s'il y a quelque chose à défiler */
    if (sdElementQueue->first != NULL)
    {
        SDElementQueueElement *popedElement = sdElementQueue->first;

        sendMessage(
                    address,
                    port,
                    protocol,
                    popedElement->beforeMsgID,
                    msgID,
                    popedElement->afterMsgID,
                    popedElement->time,
                    popedElement->afterOffset
                    );
        sdElementQueue->first = popedElement->next;
        free(popedElement->afterOffset);
        popedElement->afterOffset = NULL;
        free(popedElement);
        popedElement = NULL;
    }

    /* Fin de la zone protegee. */
    pthread_mutex_unlock (& sdElementQueue->mutex);

    return EXIT_SUCCESS;
}

void *sender(void *arg)
{
    SenderParams *senderParams = (SenderParams*) arg;

    while (1)
    {
        while (senderParams->sdElementQueue->first != NULL)
        {
            senderParams->msgID++;
            popSDElementQueue(
                              senderParams->address,
                              senderParams->port,
                              senderParams->protocol,
                              senderParams->sdElementQueue,
                              &senderParams->msgID
                              );
            if (senderParams->msgID == 65535)
            {
                senderParams->msgID = 0;
            }
        }
        SLEEP(1);
    }

#ifndef NDEBUG
    printf("Fin de l'envoi des messages\n");
#endif
    
    pthread_exit(NULL);
}

