/* 
 * File:   addonList.h
 * Author: fpo
 *
 * Created on 8 août 2012, 13:43
 */

#ifndef ADDONLIST_H
#define	ADDONLIST_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

/* ID Informations */
typedef struct IDInfo IDInfo;
struct IDInfo
{
    unsigned int *idPlg, *idAsset, *idSrc, *idSearch;
    IDInfo *nxt;
};
/* IDs List */
typedef IDInfo* IDList;

/* AddonTypeParams Informations */
typedef struct AddonTypeParamsInfo AddonTypeParamsInfo;
struct AddonTypeParamsInfo
{
    unsigned int valueNum;
    void *params;
    IDList IDList;
    AddonTypeParamsInfo *nxt;
};
/* AddonTypeParams List */
typedef AddonTypeParamsInfo* AddonTypeParamsList;

/* AddonType Informations */
typedef struct AddonTypeInfo AddonTypeInfo;
struct AddonTypeInfo
{
    unsigned int *idType;
    AddonTypeParamsList addonTypeParamsList;
    AddonTypeInfo *nxt;
};
/* AddonType List */
typedef AddonTypeInfo* AddonTypeList;

/* AddonParams Informations */
typedef struct AddonParamsInfo AddonParamsInfo;
struct AddonParamsInfo
{
    void *params;
    unsigned int *period, *staticValues;
    CollectQueue *collectQueue;
    AddonTypeList addonTypeList;
    AddonParamsInfo *nxt;
};
/* AddonParams List */
typedef AddonParamsInfo* AddonParamsList;

/* Addon Informations */
typedef struct AddonInfo AddonInfo;
struct AddonInfo
{
    unsigned int *idAddon;
    AddonParamsList addonParamsList;
    AddonInfo *nxt;
};
/* Addons List */
typedef AddonInfo* AddonList;

/**
 * @return true : Pointer of addonParamsInfo; false : NULL
 */
AddonParamsInfo* isAddonParamsOnList(
                                     const AddonParamsList *addonParamsList,
                                     const unsigned int *idAddon,
                                     void *params,
                                     const unsigned int *period,
                                     const unsigned int *staticValues
                                     );

/**
 * @return true : Pointer of addonInfo; false : NULL
 */
AddonInfo* isAddonOnList(const AddonList *addonList, const unsigned int *idAddon);

/**
 * Main function of Addon list Manager
 * @param *addonList Addon list
 * @param *idAddon   Pointer of Addon Identifier
 * @param *srcParams Pointer of Source Parameters
 * @param *period    Pointer of Period
 * @param *staticValues
 * @param *collectQueue
 * @param *idType
 * @param *searchParams
 * @param *idPlg
 * @param *idAsset
 * @param *idSrc
 * @param *idSearch
 * @return Exit status
 */
int pushAddonList(
                  AddonList *addonList,
                  unsigned int *idAddon,
                  void *srcParams,
                  unsigned int *period,
                  unsigned int *staticValues,
                  CollectQueue *collectQueue,
                  unsigned int *idType,
                  void *searchParams,
                  unsigned int *idPlg,
                  unsigned int *idAsset,
                  unsigned int *idSrc,
                  unsigned int *idSearch
                  );

#include "plugin.h"

#ifdef	__cplusplus
}
#endif

#endif	/* ADDONLIST_H */

