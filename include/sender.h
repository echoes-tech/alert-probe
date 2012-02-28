/* 
 * Header of Sender Module
 * @author ECHOES Technologies (FPO)
 * @date 14/02/2012
 */

#ifndef SENDER_H
#define	SENDER_H

// Source : http://broux.developpez.com/articles/c/sockets/

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // close
#include <netdb.h> // gethostbyname
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

typedef struct hostent Hostent;

/**
 * Main function of Sender Module
 * @param *address Engines FQDN
 * @param *port     Pointer of Emission port
 * @param *protocol Pointer of Emission protocol (O: TCP, 1: UDP)
 * @return Exit status
 */
int sender(const char *address, int *port, int *protocol);
static void init(void);
static void end(void);
/**
 * Open the socket
 * @param *address  Engines FQDN
 * @param *port     Pointer of Emission port
 * @param *protocol Pointer of Emission protocol (O: TCP, 1: UDP)
 * @param *sin      Pointer of Connection params
 * @param *sock     Pointer of Socket
 * @return Exit status
 */
static int initConnection(const char *address, int *port, int *protocol, SOCKADDR_IN *sin, SOCKET *sock);
/**
 * Close the socket
 * @param *sock Pointer of Socket
 */
static int endConnection(SOCKET *sock);

#endif	/* SENDER_H */

