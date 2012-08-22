/* 
 * Header of Format Module
 * @author ECHOES Technologies (FPO)
 * @date 24/04/2012
 */

#ifndef FORMAT_H
#define	FORMAT_H

#include "addon/addon.h"

/* Element of SD-Element queue */
typedef struct SDElementQueueElement SDElementQueueElement;
struct SDElementQueueElement
{
    char beforeMsgID[10000], afterMsgID[10000], afterOffset[10000];
    time_t time;
    SDElementQueueElement *next;
};
/* SD-Element queue */
typedef struct SDElementQueue SDElementQueue;
struct SDElementQueue
{
    pthread_mutex_t mutex;
    char *hostname, appName[19];
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
};

/**
 */
int pushSDElementQueue(
                       SDElementQueue *sdElementQueue,
                       unsigned int idPlg,
                       unsigned int idAsset,
                       unsigned int idSrc,
                       unsigned int idSearch,
                       unsigned int valueNum,
                       unsigned short lotNum,
                       const gchar *b64Value,
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

