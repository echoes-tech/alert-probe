/* 
 * File:   sender.c
 * Author: Florent Poinsaut <florent.poinsaut@echoes-tech.com>
 *
 * Created on 14 février 2012, 09:41
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "sender.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      return(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

static int init_connection(const char *address, int *port, int *protocol, SOCKADDR_IN *sin, SOCKET *sock)
{
    Hostent *hostinfo; // Engines FQDN info

    if (*protocol == 1)
    {
        /* UDP so SOCK_DGRAM */
        *sock = socket(AF_INET, SOCK_DGRAM, 0);
    }
    else
    {
        *sock = socket(AF_INET, SOCK_STREAM, 0);
    }

   if(*sock == INVALID_SOCKET)
   {
      perror("socket()");
      return(errno);
   }

   // Get info of Engines FQDN
   hostinfo = gethostbyname(address);
   // Whether no info, return an error by stopping probe
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      return(EXIT_FAILURE);
   }

   // Emission info
   sin->sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin->sin_port = htons(*port);
   sin->sin_family = AF_INET;
   
    if (*protocol != 1)
    {
        // Open a connection
        if(connect(*sock,(SOCKADDR *) sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            perror("connect()");
            return(errno);
        }
    }

   return (EXIT_SUCCESS);
}

static int end_connection(SOCKET *sock) {
    if (closesocket(*sock))
    {
        perror("closesocket()");
        return (errno);
    }
   return (EXIT_SUCCESS);
}

int sender(const char *address, int port, int protocol) {
    SOCKADDR_IN sin = {0}; // Emission info
    SOCKET sock; // Socket
    char message[] = "Hello World !"; // Message to send
 
    // Init just for Win32
    init();

    // Creating the Socket
    if(init_connection(address, &port, &protocol, &sin, &sock))
    {
        perror("init_connection()");
        return (errno);
    }

    // Sending data
    if (protocol == 1)
    {
        if(sendto(sock, message, strlen(message), 0, (SOCKADDR *) &sin, sizeof sin) < 0)            
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
    if (end_connection(&sock))
    {
        perror("send()");
        return (errno);
    }

    // End just for Win32
    end();
    
    return (EXIT_SUCCESS);
}

