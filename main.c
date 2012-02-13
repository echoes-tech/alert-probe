/* 
 * File:   main.c
 * Author: Florent Poinsaut <florent.poinsaut@echoes-tech.com>
 *
 * Created on 13 février 2012, 16:04
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "main.h"

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

static int init_connection(const char *address, SOCKADDR_IN *to)
{
   /* UDP so SOCK_DGRAM */
   SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
   Hostent *hostinfo; // Info sur le FQDN du moteur

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   // Récupération des info sur le FQDN du moteur
   hostinfo = gethostbyname(address);
   // S'il n'y a aucune info, on renvoi une erreur en arrêtant la sonde
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

    // Renseignement les info d'émission
   to->sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   to->sin_port = htons(PORT);
   to->sin_family = AF_INET;

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static void send_server(SOCKET sock, SOCKADDR_IN *to, const char *buffer)
{
   if(sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *) to, sizeof *to) < 0)
   {
      perror("sendto()");
      exit(errno);
   }
}

/*
 * 
 */
int main(int argc, char** argv) {
    SOCKADDR_IN to = {0}; // Info d'émission
    int sock; // Socket
    char message[] = "Hello World !"; // Message à émettre
    
    // Affichage à l'écran le démarrage de la sonde
    //TODO : ne l'afficher qu'en mode verbose
    printf("---------- %s %s ----------\n", NAME, VERSION);

    // On ferme le socket
    closesocket(sock);
    
    init();

    // Création du Socket
    init_connection(SRV_FQDN, &to);

    // Envoi des données
    send_server(sock, &to, message);
    
    // Fermeture du socket
    end_connection(sock);

    end();
    
    return (EXIT_SUCCESS);
}

