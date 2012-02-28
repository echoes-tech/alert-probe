/* 
 * Sender Module
 * @author ECHOES Technologies (FPO)
 * @date 14/02/2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

static int initConnection(const char *address, int *port, int *protocol, SOCKADDR_IN *sin, SOCKET *sock)
{
    Hostent *hostinfo; // Engines FQDN info

    if (*protocol == 1)
    {
        // UDP so SOCK_DGRAM
        *sock = socket(AF_INET, SOCK_DGRAM, 0);
    }
    else
    {
        // UDP so SOCK_STREAM
        *sock = socket(AF_INET, SOCK_STREAM, 0);
    }

    if (*sock == INVALID_SOCKET)
    {
        perror("socket()");
        return (errno);
    }

    // Get info of Engines FQDN
    hostinfo = gethostbyname(address);
    // Whether no info, return an error by stopping probe
    if (hostinfo == NULL)
    {
        fprintf(stderr, "Unknown host %s.\n", address);
        return (EXIT_FAILURE);
    }

    // Emission info
    sin->sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    sin->sin_port = htons(*port);
    sin->sin_family = AF_INET;

    if (*protocol != 1)
    {
        // Open a connection
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

int sender(const char *address, int *port, int *protocol)
{
    SOCKADDR_IN sin = {0}; // Emission info
    SOCKET sock; // Socket
    char message[] = "Hello World !"; // Message to send

    // Init just for Win32
    init();

    // Creating the Socket
    if (initConnection(address, port, protocol, &sin, &sock))
    {
        perror("initConnection()");
        return (errno);
    }

    // Sending data
    if (*protocol == 1)
    {
        if (sendto(sock, message, strlen(message), 0, (SOCKADDR *) & sin, sizeof sin) < 0)
        {
            perror("sendto()");
            return (errno);
        }
    }
    else
    {
        if (send(sock, message, strlen(message), 0) < 0)
        {
            perror("send()");
            return (errno);
        }
    }

    // Closing the socket
    if (endConnection(&sock))
    {
        perror("send()");
        return (errno);
    }

    // End just for Win32
    end();

    return (EXIT_SUCCESS);
}

