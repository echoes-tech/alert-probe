/* 
 * Header of Format Module
 * @author ECHOES Technologies (FPO)
 * @date 24/04/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
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

