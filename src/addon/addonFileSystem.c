/* 
 * Addon FileSystem
 * @author ECHOES Technologies (FPO)
 * @date 05/08/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "addon/addonFileSystem.h"

int addonFileSystemStatsvfs(
                            CollectQueue *collectQueue,
                            void *params,
                            unsigned short lotNum,
                            IDList *idList,
                            time_t *now
                            )
{
    SearchInfoParams1_6 *searchInfoParams = (SearchInfoParams1_6*)params;

    char *values[11] = { "" };

    struct statvfs info;
    
    if (statvfs(searchInfoParams->path, &info) == 0)
    {
        values[0] = g_strdup_printf("%lu", info.f_bsize);    /* file system block size */
        values[1] = g_strdup_printf("%lu", info.f_frsize);   /* fragment size */
        values[2] = g_strdup_printf("%lu", info.f_blocks);   /* size of fs in f_frsize units */
        values[3] = g_strdup_printf("%lu", info.f_bfree);    /* # free blocks */
        values[4] = g_strdup_printf("%lu", info.f_bavail);   /* # free blocks for unprivileged users */
        values[5] = g_strdup_printf("%lu", info.f_files);    /* # inodes */
        values[6] = g_strdup_printf("%lu", info.f_ffree);    /* # free inodes */
        values[7] = g_strdup_printf("%lu", info.f_favail);   /* # free inodes for unprivileged users */
        values[8] = g_strdup_printf("%lu", info.f_fsid);     /* file system ID */
        values[9] = g_strdup_printf("%lu", info.f_flag);     /* mount flags */
        values[10] = g_strdup_printf("%lu", info.f_namemax); /* maximum filename length */     

        if (pushCollectQueue(
                             collectQueue,
                             idList,
                             lotNum,
                             1,
                             11,
                             values,
                             *now
                             ))
        {
            return EXIT_FAILURE;
        }

    }
    else
    {
        g_critical("Critical: %s: %s", strerror(errno), searchInfoParams->path);
        return errno;
    }

    return EXIT_SUCCESS;
}

void *addonFileSystem(void *arg)
{
    AddonParamsInfo *addonParamsInfo = (AddonParamsInfo*) arg;

    time_t now;

#ifndef NDEBUG
    printf("Dans le thread addonFileSystem.\n");
#endif

    while(TRUE)
    {
        AddonTypeInfo *addonTypeInfo = addonParamsInfo->addonTypeList;

        addonSleep(*addonParamsInfo->period);

        addonParamsInfo->lotNum = increaseLotNum(
                                                 addonParamsInfo->mutex,
                                                 addonParamsInfo->lotNumPtr
                                                 );
        /* What time is it ? */
        time(&now);

        /* Tant que l'on n'est pas au bout de la liste */
        while (addonTypeInfo != NULL)
        {
            AddonTypeParamsInfo *addonTypeParamsInfo = addonTypeInfo->addonTypeParamsList;

            /* Tant que l'on n'est pas au bout de la liste */
            while (addonTypeParamsInfo != NULL)
            {
                switch (*addonTypeInfo->idType)
                {
                case 6:
                    addonFileSystemStatsvfs(
                                    addonParamsInfo->collectQueue,
                                    addonTypeParamsInfo->params,
                                    addonParamsInfo->lotNum,
                                    &addonTypeParamsInfo->IDList,
                                    &now
                                    );
                    break;
                default:
                    g_warning("Warning: idType %d does'nt exist for the FileSystem addon.", *addonTypeInfo->idType);
                    break;
                }

                /* On avance d'une case */
                addonTypeParamsInfo = addonTypeParamsInfo->nxt;
            }

            /* On avance d'une case */
            addonTypeInfo = addonTypeInfo->nxt;
        }

    }

    pthread_exit(NULL);
}

