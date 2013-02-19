/* 
 * Header of Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ADDON_H
#define	ADDON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "plugin.h"
    
#include "addon/addonFile.h"
#include "addon/addonLog.h"
#include "addon/addonMySQL.h"
#include "addon/addonSNMP.h"

/* Portable sleep */
#if defined (WIN32) || defined (WIN64)

#include <windows.h>
#define SLEEP(time) Sleep(time*1000)

#else

#include <unistd.h>
#define SLEEP(time) sleep(time)

#endif

typedef struct AddonsMgrParams AddonsMgrParams;
struct AddonsMgrParams
{
    AddonList addonsList;
    pthread_t* addonsThreads;
    unsigned short lotNum;
    pthread_mutex_t mutex;
    CollectQueue collectQueue;
};

#define ADDON_PARAMS_INITIALIZER {NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER, COLLECT_QUEUE_INITIALIZER}

/**
 * Method to know when continue the collect
 * @param period
 */
void addonSleep(unsigned int period);

/**
 * Increase lot number
 * @param mutexPtr  Pointer of mutex
 * @param lotNumPtr Pointer of lot number
 * @return Current lot number
 */
unsigned short increaseLotNum(
                              pthread_mutex_t *mutexPtr,
                              unsigned short *lotNumPtr
                              );

/**
 * Push a new element to CollectQueue
 * @param collectQueue 
 * @param idList
 * @param lotNum
 * @param lineNum
 * @param valuesLength
 * @param values
 * @param time
 * @return Exit status
 */
int pushCollectQueue(
                     CollectQueue *collectQueue,
                     IDList *idList,
                     const unsigned short lotNum,
                     const unsigned int lineNum,
                     const unsigned int valuesLength,
                     char **values,
                     time_t time
                     );

/**
 * Thread - Main function of Addons Manager.
 * @param arg (void*)AddonsMgrParams*
 */
void *addon(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* ADDON_H */

