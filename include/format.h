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

#ifndef FORMAT_H
#define	FORMAT_H

#include "addonLib.h"

/* Element of SD-Element queue */
typedef struct SDElementQueueElement SDElementQueueElement;
struct SDElementQueueElement
{
    char beforeMsgID[10000], afterMsgID[10000], *afterOffset;
    time_t time;
    SDElementQueueElement *next;
};
/* SD-Element queue */
typedef struct SDElementQueue SDElementQueue;
struct SDElementQueue
{
    pthread_mutex_t mutex;
    const char *hostname, appName[19];
    char *token;
    unsigned int *probeID;
    unsigned char *transportMsgVersion;
    pid_t pID;
    SDElementQueueElement *first;
};

typedef struct FormatParams FormatParams;
struct FormatParams
{
    CollectQueue *collectQueue;
    SDElementQueue *sdElementQueue;
    int *signum;
};

/**
 */
int pushSDElementQueue(
                       SDElementQueue *sdElementQueue,
                       char *afterOffset,
                       time_t time
                       );

/**
 * Group collected data by time in Structured-Element.
 * @param collectQueue   Pointer of Data collected Queue
 * @param sdElementQueue Pointer of SD-Element Queue
 * @return Exit status
 */
 int popCollectQueue(CollectQueue *collectQueue, SDElementQueue *sdElementQueue);
 
/**
 * Create the message to send
 * @param sdElementQueue   SD-Element Queue
 * @param probeID             Probe Identifier
 * @param transportMsgVersion Message Version
 * @return Exit status
 */
 int msgCreator(SDElementQueue sdElementQueue, unsigned int probeID, unsigned int transportMsgVersion);

/**
 * Main function of Format Module.
 * @param *probeID            Pointer of Probe Identifier
 * @param transportMsgVersion Pointer of Message Version
 * @param collectQueue        Pointer of Data collected Queue
 */
void *format(void *arg);

#endif	/* FORMAT_H */

