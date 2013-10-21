/* 
 * Header of Addon List
 * @author ECHOES Technologies (FPO)
 * @date 08/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ADDONLIST_H
#define	ADDONLIST_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <addonLib.h>

#define COLLECT_QUEUE_INITIALIZER {PTHREAD_MUTEX_INITIALIZER, NULL}

/**
 * @param addonParamsList
 * @param idAddon
 * @param params
 * @param period
 * @param staticValues
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
 * 
 * @param addonList
 * @param idAddon
* @return true : Pointer of addonInfo; false : NULL
*/
AddonInfo* isAddonOnList(
                         const AddonList *addonList,
                         const unsigned int *idAddon
                         );

/**
 * Main function of Addon list Manager
 * @param *addonList Addon list
 * @param *idAddon   Pointer of Addon Identifier
 * @param *srcParams Pointer of Source Parameters
 * @param *period    Pointer of Period
 * @param *staticValues
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

