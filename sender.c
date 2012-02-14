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
 * @param *to      Pointer of Connection params
 * @return Socket
 */
static int init_connection(const char *address, SOCKADDR_IN *to)
{
   /* UDP so SOCK_DGRAM */
   SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
   Hostent *hostinfo; // Engines FQDN info

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
   to->sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   to->sin_port = htons(PORT);
   to->sin_family = AF_INET;

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
 * Send the message to ECHOES Alert Engine
 * @param sock    Socket
 * @param *to     Pointer of Connection params
 * @param *buffer Message
 */
static void send_server(SOCKET sock, SOCKADDR_IN *to, const char *buffer)
{
   if(sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *) to, sizeof *to) < 0)
   {
      perror("sendto()");
      exit(errno);
   }
}

/**
 * Main function of Sender Module
 * @return Exit status
 */
int sender() {
    SOCKADDR_IN to = {0}; // Emission info
    int sock; // Socket
    char message[] = "Hello World !"; // Message to send
 
    init();

    // Creating the Socket
    sock = init_connection(SRV_FQDN, &to);

    // Sending data
    send_server(sock, &to, message);
    
    // Closing the socket
    end_connection(sock);

    end();
    
    return (EXIT_SUCCESS);
}

