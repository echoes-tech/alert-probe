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

#ifndef SENDER_H
#define	SENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <gio/gio.h>

#include "format.h"

/* Source : http://broux.developpez.com/articles/c/sockets/ */

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define HEARTBEAT 10
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

typedef struct hostent Hostent;

typedef struct SenderParams SenderParams;
struct SenderParams
{
    SDElementQueue *sdElementQueue;
    const char *address;
    unsigned short *port, msgID;
    unsigned char *protocol;
    int *signum;
};

/* TODO: changer de nom et dégager "static" */
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
static int initConnection(
                          const char *address,
                          unsigned short *port,
                          unsigned char *protocol,
                          SOCKADDR_IN *sin,
                          SOCKET *sock
                          );

/**
 * Close the socket
 * @param *sock Pointer of Socket
 */
static int endConnection(SOCKET *sock);

/**
 * Send Message
 * @param *address     Engines FQDN
 * @param *port        Pointer of Emission port
 * @param *protocol    Pointer of Emission protocol (O: TCP, 1: UDP)
 * @param *beforeMsgID Message before Message Identifier
 * @param *msgID       Pointer of Message Identifier
 * @param *afterMsgID  Message after Message Identier
 * @patam collectTime         Time of collect
 * @param *afterOffset Message after Offset
 * @return Exit status
 */
int sendMessage(
                const char *address,
                unsigned short *port,
                unsigned char *protocol,
                const char *beforeMsgID,
                unsigned short *msgID,
                const char *afterMsgID,
                time_t collectTime,
                const char *afterOffset
                );

/**
 * Extract a message from SD-Element Queue
 * @param *address        Engines FQDN
 * @param *port           Pointer of Emission port
 * @param *protocol       Pointer of Emission protocol (O: TCP, 1: UDP)
 * @param *sdElementQueue Pointer of SD-Element Queue
 * @param *msgID    Pointer of Message Identifier
 * @return Exit status
 */
int popSDElementQueue(
                      const char *address,
                      unsigned short *port,
                      unsigned char *protocol,
                      SDElementQueue *sdElementQueue,
                      unsigned short *msgID
                      );

/*
 * Test if an hearbeat should be sent, push ElementQueue if needed
 * @param *sdElementQueue Pointer of SD-Element Queue
 * @return Exit status
 */
int  heartBeat(SDElementQueue *sdElementQueue);

/**
 * Thread - Main function of Sender Module
 * @param *sdElementQueue Pointer of SD-Element Queue
 * @param *address        Engines FQDN
 * @param *port           Pointer of Emission port
 * @param *protocol       Pointer of Emission protocol (O: TCP, 1: UDP)
 */
void *sender(void *arg);

#endif	/* SENDER_H */

