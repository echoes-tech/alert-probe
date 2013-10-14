/* 
 * Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "addon.h"

void *addon(void *arg)
{
    AddonsMgrParams *addonsMgrParams = (AddonsMgrParams*) arg;

    /* Thread Counter + Position on the threads table */
    unsigned int i = 0, numThread = 0;

    gboolean snmpInit = FALSE;

    char *libPath = NULL;
    void *libHandle = NULL;
    void *(*addon)(void* arg);

    AddonInfo *addonInfo = addonsMgrParams->addonsList;
    /* Tant que l'on n'est pas au bout de la liste */
    while (addonInfo != NULL)
    {
        AddonParamsInfo *addonParamsInfo = addonInfo->addonParamsList;

        /* Tant que l'on n'est pas au bout de la liste */
        while (addonParamsInfo != NULL)
        {
            addonParamsInfo->collectQueue = &addonsMgrParams->collectQueue;
            addonParamsInfo->lotNumPtr = &addonsMgrParams->lotNum;
            addonParamsInfo->mutex = &addonsMgrParams->mutex;
            addonParamsInfo->signum = addonsMgrParams->signum;
            addonParamsInfo->snmpInit = &snmpInit;

            switch (*addonInfo->idAddon)
            {
#ifdef NDEBUG
                case 1:
                /* addonFileSystem */
                libPath = strdup("libaddonfilesystem.so");
                break;
            case 2:
                /* addonFile */
                libPath = strdup("libaddonfile.so");
                break;
            case 3:
                /* addonLog */
                libPath = strdup("libaddonlog.so");
                break;
            case 5:
                /* addonSNMP */
                libPath = strdup("libaddonsnmp.so");
                break;
#else
            case 1:
                /* addonFileSystem */
                libPath = strdup("../addonfilesystem/dist/Debug/GNU-Linux-x86/libaddonfilesystem.so");
                break;
            case 2:
                /* addonFile */
                libPath = strdup("../addonfile/dist/Debug/GNU-Linux-x86/libaddonfile.so");
                break;
            case 3:
                /* addonLog */
                libPath = strdup("../addonlog/dist/Debug/GNU-Linux-x86/libaddonlog.so");
                break;
            case 5:
                /* addonSNMP */
                libPath = strdup("../addonsnmp/dist/Debug/GNU-Linux-x86/libaddonsnmp.so");
                break;
#endif
            default:
                g_critical("Critical: idAddon %d does'nt exist", *addonInfo->idAddon);
                pthread_exit(NULL);
            }

            libHandle = dlopen(libPath, RTLD_NOW);
            if (libHandle)
            {
                addon = dlsym(libHandle, "addon");
                if (addon)
                {
                    if (pthread_create(&addonsMgrParams->addonsThreads[numThread], NULL, addon, (void*) addonParamsInfo))
                    {
                        g_critical("Critical: %s: addon: %u", strerror(errno), numThread);
                        pthread_exit(NULL);
                    }
                }
                else
                {
                    g_critical("Critical: Cannot load function: %s", dlerror());
                }
                free(libPath);
            }
            else
            {
                g_critical("Critical: Cannot load library: %s", dlerror());
            }
            
            ++numThread;

            /* On avance d'une case */
            addonParamsInfo = addonParamsInfo->nxt;
        }

        /* On avance d'une case */
        addonInfo = addonInfo->nxt;
    }

#ifndef NDEBUG
    printf("Fin du chargement des addons\n");
#endif

    /* Attente de la fin des threads */
    for (i = 0; i < numThread; i++)
    {
        pthread_join(addonsMgrParams->addonsThreads[i], NULL);
    }

    pthread_exit(NULL);

}

