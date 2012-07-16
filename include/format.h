/* 
 * Header of Format Module
 * @author ECHOES Technologies (FPO)
 * @date 24/04/2012
 */


#ifndef FORMAT_H
#define	FORMAT_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "addon.h"

// Element of SD-Element queue
typedef struct SDElementQueueElement SDElementQueueElement;
struct SDElementQueueElement
{
    char sdElement[10000];
    SDElementQueueElement *next;
};
// SD-Element queue
typedef struct SDElementQueue SDElementQueue;
struct SDElementQueue
{
    pthread_mutex_t mutex;
    char hostname[255], appName[255];
    unsigned int *probeID, *transportMsgVersion;
    pid_t pID;
    SDElementQueueElement *first;
};

typedef struct FormatParams FormatParams;
struct FormatParams
{
    CollectQueue *collectQueue;
    SDElementQueue *sdElementQueue;
};

char *base64(const unsigned char *input, int length);

/**
 * Group collected data by time in Structured-Element.
 * @param collectQueue   Data collected Queue
 * @param sdElementQueue SD-Element Queue
 * @return Exit status
 */
 int collectedData2sdElement(CollectQueue collectQueue, SDElementQueue sdElementQueue);
 
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

