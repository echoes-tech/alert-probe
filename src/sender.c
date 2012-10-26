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
        fprintf(stderr, "Unknown host %s.\n", address);
        return (EXIT_FAILURE);
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
        perror("socket()");
        return (errno);
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
            perror("connect()");
            return (errno);
        }
    }

    return (EXIT_SUCCESS);
}

static int endConnection(SOCKET *sock)
{
    if (closesocket(*sock))
    {
        perror("closesocket()");
        return (errno);
    }
    return (EXIT_SUCCESS);
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
    char completMsg[480];

    /* Init just for Win32 */
    init();

    /* Creating the Socket */
    if (initConnection(address, port, protocol, &sin, &sock))
    {
        perror("initConnection()");
        return (errno);
    }
    
    /* Adding PRI, VERSION and TIMESTAMP */
    time(&now);
    g_get_current_time(&g_time);
    
    /* Caution: keep the Line Feed to work with TCP */
    snprintf(
        completMsg,
             480,
             "<118>1 %s %s%d %s%d%s]\n",
             g_time_val_to_iso8601(&g_time),
             beforeMsgID,
             *msgID,
             afterMsgID,
             (int)difftime (now, collectTime),
             afterOffset);

#ifndef NDEBUG
    printf("%s", completMsg);
#endif
    
    /* Sending data */
    if (*protocol == 1)
    {
        if (sendto(sock, completMsg, strlen(completMsg), 0, (SOCKADDR *) & sin, sizeof sin) < 0)
        {
            perror("sendto()");
            return (errno);
        }
    }
    else
    {
        if (send(sock, completMsg, strlen(completMsg), 0) < 0)
        {
            perror("send()");
            return (errno);
        }
    }

    /* Closing the socket */
    if (endConnection(&sock))
    {
        perror("send()");
        return (errno);
    }

    /* End just for Win32 */
    end();

    return (EXIT_SUCCESS);
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
    {
        exit(EXIT_FAILURE);
    }

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
        free(popedElement);
    }

    /* Fin de la zone protegee. */
    pthread_mutex_unlock (& sdElementQueue->mutex);

    return (EXIT_SUCCESS);
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

