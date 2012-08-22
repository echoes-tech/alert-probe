/* 
 * Header of Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
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
 * Thread - Main function of Addons Manager.
 * @param arg (void*)AddonsMgrParams*
 */
void *addon(void *arg);

#include "addonFile.h"
#include "addonLog.h"

#ifdef	__cplusplus
}
#endif

#endif	/* ADDON_H */

