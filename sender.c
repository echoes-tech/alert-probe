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

#include "sender.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

/**
 * Open the socket
 * @param *address Engines FQDN 
 * @param *sin      Pointer of Connection params
 * @return Socket
 */
static int init_connection(const char *address, SOCKADDR_IN *sin)
{
    SOCKET sock;
    Hostent *hostinfo; // Engines FQDN info

    if (PROTO == "UDP") {
        /* UDP so SOCK_DGRAM */
        sock = socket(AF_INET, SOCK_DGRAM, 0);
    } else {
        sock = socket(AF_INET, SOCK_STREAM, 0);
    }

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   // Get info of Engines FQDN
   hostinfo = gethostbyname(address);
   // Whether no info, return an error by stopping probe
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

    // Emission info
   sin->sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin->sin_port = htons(PORT);
   sin->sin_family = AF_INET;
   
    if (PROTO != "UDP")
    {
        // Open a connection
        if(connect(sock,(SOCKADDR *) sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            perror("connect()");
            exit(errno);
        }
    }

   return sock;
}

/**
 * Close the socket
 * @param sock Socket
 */
static void end_connection(int sock)
{
   closesocket(sock);
}

/**
 * Send the message in UDP to ECHOES Alert Engine
 * @param sock    Socket
 * @param *sin    Pointer of Connection params
 * @param *buffer Message
 */
static void send_server_udp(SOCKET sock, SOCKADDR_IN *sin, const char *buffer)
{
   if(sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *) sin, sizeof *sin) < 0)
   {
      perror("sendto()");
      exit(errno);
   }
}

/**
 * Send the message in TCP to ECHOES Alert Engine
 * @param sock    Socket
 * @param *buffer Message
 */
static void send_server_tcp(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

/**
 * Main function of Sender Module
 * @return Exit status
 */
int sender() {
    SOCKADDR_IN sin = {0}; // Emission info
    int sock; // Socket
    char message[] = "Hello World !"; // Message to send
 
    init();

    // Creating the Socket
    sock = init_connection(SRV_FQDN, &sin);

    // Sending data
    if (PROTO == "UDP")
    {
        send_server_udp(sock, &sin, message);
    }
    else
    {
         send_server_tcp(sock, message);
    }
    
    // Closing the socket
    end_connection(sock);

    end();
    
    return (EXIT_SUCCESS);
}

