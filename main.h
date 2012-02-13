/* 
 * File:   main.h
 * Author: Florent Poinsaut <florent.poinsaut@echoes-tech.com>
 *
 * Created on 13 février 2012, 16:31
 */

#ifndef MAIN_H
#define	MAIN_H

// Source : http://broux.developpez.com/articles/c/sockets/

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

// Le nom de la sonde
#define NAME "ECHOES Alert - Sonde"
// La version de la sonde
#define VERSION "0.1.0"
// L'adresse IP ou idéalement le FQDN du moteur
#define SRV_FQDN "127.0.0.1"
//#define SRV_FQDN "alert.echoes-tech.com"
// Le port d'écoute du moteur
#define PORT 8888

typedef struct hostent Hostent;

static void init(void);
static void end(void);
static int init_connection(const char *address, SOCKADDR_IN *to);
static void end_connection(int sock);
static void send_server(SOCKET sock, SOCKADDR_IN *to, const char *buffer);

#endif	/* MAIN_H */

