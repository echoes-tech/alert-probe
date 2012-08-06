/* 
 * Header of Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */


#ifndef ADDON_H
#define	ADDON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "plugin.h"

/* Portable sleep */
#if defined (WIN32) || defined (WIN64)

#include <windows.h>
#define SLEEP(time) Sleep(time*1000)

#else

#include <unistd.h>
#define SLEEP(time) sleep(time)

#endif

/* Element of collected data queue */
typedef struct CollectQueueElement CollectQueueElement;
struct CollectQueueElement
{
    unsigned int idPlg, idAsset, idSrc, idSearch, valueNum;
    char value[1000];
    time_t time;
    CollectQueueElement *next;
};
/* Collected data queue */
typedef struct CollectQueue CollectQueue;
struct CollectQueue
{
    pthread_mutex_t mutex;
    CollectQueueElement *first;
};

#define COLLECT_QUEUE_INITIALIZER {PTHREAD_MUTEX_INITIALIZER, NULL}

typedef struct LoopParams LoopParams;
struct LoopParams
{
    pthread_cond_t condition;
    pthread_mutex_t mutex;
    unsigned int idAddon, idType;
    void *params;
};

typedef struct AddonLocationLogParams AddonLocationLogParams;
struct AddonLocationLogParams
{
    const unsigned int idPlg, idAsset, idSrc, idSearch, period, staticValues, firstChar, length;
    unsigned int nbLine;
    const char *path;
    CollectQueue *collectQueue;
};

typedef struct AddonLocationFileParams AddonLocationFileParams;
struct AddonLocationFileParams
{
    const unsigned int idPlg, idAsset, idSrc, idSearch, period, staticValues, line, firstChar, length;
    const char *path;
    CollectQueue *collectQueue;
};

typedef struct AddonRegexFileParams AddonRegexFileParams;
struct AddonRegexFileParams
{
    const unsigned int idPlg, idAsset, idSrc, idSearch, period, staticValues;
    unsigned int valueNum;
    const char *path;
    regex_t preg;
    int match;
    size_t nmatch;
    regmatch_t *pmatch;
    CollectQueue *collectQueue;
};

typedef struct AddonsMgrParams AddonsMgrParams;
struct AddonsMgrParams
{
    PlgList plgList;
    pthread_t* addonsThreads;
    LoopParams* loopsParams;
    CollectQueue collectQueue;
};

#define ADDON_PARAMS_INITIALIZER {NULL, NULL, NULL, COLLECT_QUEUE_INITIALIZER}

/**
 * Push a new Element to CollectQueue
 * @param *collectQueue Pointer of Collected Data Queue
 * @param idPlg         Plugin Identifier
 * @param idAsset       Asset Identifier
 * @param idSrc         Source Identifier
 * @param idSearch      Search Identifier
 * @param valueNum      Value Number
 * @param value         Message Value
 * @param time          Time of collect
 * @return Exit status
 */
int pushCollectQueue(CollectQueue *collectQueue, const unsigned int idPlg, const unsigned int idAsset, const unsigned int idSrc, const unsigned int idSearch, const unsigned int valueNum, const char *value, time_t time);

/**
 * Thread - Loop for addon.
 * @param arg Pointer of Loop Parameters 
 */
void *addonLoop(void *arg);

/**
 * Thread - Addon Location File.
 * @param *alfp Pointer of addon location file parameters
 */
void *addonLocationFile(void *arg);

/**
 * Thread - Addon Regex File.
 * @param *arfp Pointer of addon regex file parameters
 */
void *addonRegexFile(void *arg);

/**
 * Thread - Addon Location Log.
 * @param *allp Pointer of addon location log parameters
 */
void *addonLocationLog(void *arg);

/**
 * Thread - Main function of Addons Manager.
 * @param *nbThreads      Pointer of Number of threads
 * @param *plgList        Pointer of Plugins list with informations
 * @param *addonThreads[] Table of addons threads
 */
void *addon(void *arg);

#endif	/* ADDON_H */

