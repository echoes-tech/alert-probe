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

// Portable sleep
#if defined (WIN32) || defined (WIN64)

#include <windows.h>
#define SLEEP(time) Sleep(time*1000)

#else

#include <unistd.h>
#define SLEEP(time) sleep(time)

#endif

typedef struct LoopParams LoopParams;
struct LoopParams
{
    unsigned int idAddon, idType;
    void *params;
};

typedef struct AddonLocationFileParams AddonLocationFileParams;
struct AddonLocationFileParams
{
    unsigned int idPlg, idAsset, idSrc, idSearch, period, staticValues, line, firstChar, length;
    char path[255];
};

/**
 * Thread - Loop for addon.
 * @param arg Pointer of Loop Parameters 
 */
void *addonLoop(void *arg);

/**
 * Addon Location File.
 * @param *alfp Pointer of addon location file parameters
 */
void *addonLocationFile(void *arg);

/**
 * Main function of Addons Manager.
 * @param *nbThreads      Pointer of Number of threads
 * @param *plgList        Pointer of Plugins list with informations
 * @param *addonThreads[] Table of addons threads
 * @return Exit status
 */
int addon(unsigned int *nbThreads, PlgList *plgList, pthread_t addonsThreads[]);

#endif	/* ADDON_H */

