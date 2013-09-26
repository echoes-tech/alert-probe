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

#include "addon/addon.h"

void addonSleep(unsigned int period)
{
    time_t now, temp;
    
    /* What time is it ? */
    time(&now);
    
    /* Method to know when continue the collect */
    temp = ((int)(now / period) * period) + period;

    /* Diff between now and the start of the loop */
    SLEEP(difftime(temp, now));
}

unsigned short increaseLotNum(
                              pthread_mutex_t *mutexPtr,
                              unsigned short *lotNumPtr
                              )
{
    unsigned short res;
    
    /* Start of protected area */
    pthread_mutex_lock(mutexPtr);

    ++*lotNumPtr;
    res = *lotNumPtr;

    if (*lotNumPtr == 65535)
    {
        *lotNumPtr = 0;
    }
    
    /* End of protected area */
    pthread_mutex_unlock(mutexPtr);
    
    return res;
}

int pushCollectQueue(
                     CollectQueue *collectQueue,
                     IDList *idList,
                     const unsigned short lotNum,
                     const unsigned int lineNum,
                     const unsigned int valuesLength,
                     char **values,
                     time_t time
                     )
{
    IDInfo *idInfo;

    gushort i = 0;

    idInfo = *idList;
    /* Tant que l'on n'est pas au bout de la liste */
    while (idInfo != NULL)
    {
        CollectQueueElement *new = calloc(1, sizeof(CollectQueueElement));
        if (collectQueue == NULL || new == NULL)
            g_error("Error: Queue of collected data unavailable");

        new->idPlg = *idInfo->idPlg;
        new->idAsset = *idInfo->idAsset;
        new->idSrc = *idInfo->idSrc;
        new->idSearch = *idInfo->idSearch;
        new->lotNum = lotNum;
        new->lineNum = lineNum;
        new->valuesLength = valuesLength;
        new->values = calloc(new->valuesLength, sizeof (char*));
        for(i = 0; i < valuesLength; ++i)
        {
            new->values[i] = strdup(values[i]);
        }
        new->time = time;

        /* Debut de la zone protegee. */
        pthread_mutex_lock (& collectQueue->mutex);

        if (collectQueue->first != NULL) /* La file n'est pas vide */
        {
            /* On se positionne à la fin de la file */
            CollectQueueElement *lastElement = collectQueue->first;
            while (lastElement->next != NULL)
            {
                lastElement = lastElement->next;
            }
            lastElement->next = new;
        }
        else /* La file est vide, notre élément est le premier */
        {
            collectQueue->first = new;
        }

        /* Fin de la zone protegee. */
        pthread_mutex_unlock(& collectQueue->mutex);

        /* On avance d'une case */
        idInfo = idInfo->nxt;
    }
    for(i = 0; i < valuesLength; ++i)
        {
            free(values[i]);
            values[i] = NULL;
        }
    return EXIT_SUCCESS;
}

void *addon(void *arg)
{
    AddonsMgrParams *addonsMgrParams = (AddonsMgrParams*) arg;
    
    /* Thread Counter + Position on the threads table */
    unsigned int i = 0, numThread = 0;
    
    gboolean snmpInit = FALSE;

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

            switch (*addonInfo->idAddon)
            {
            case 1:
                if (pthread_create(&addonsMgrParams->addonsThreads[numThread], NULL, addonFileSystem, (void*) addonParamsInfo))
                {
                    g_critical("Critical: %s: addonFileSystem: %u", strerror(errno), numThread);
                    pthread_exit(NULL);
                }
                break;
            case 2:
                if (pthread_create(&addonsMgrParams->addonsThreads[numThread], NULL, addonFile, (void*) addonParamsInfo))
                {
                    g_critical("Critical: %s: addonFile: %u", strerror(errno), numThread);
                    pthread_exit(NULL);
                }
                break;
            case 3:
                if (pthread_create(&addonsMgrParams->addonsThreads[numThread], NULL, addonLog, (void*) addonParamsInfo))
                {
                    g_critical("Critical: %s: addonLog: %u", strerror(errno), numThread);
                    pthread_exit(NULL);
                }
                break;
            case 4:
                if (pthread_create(&addonsMgrParams->addonsThreads[numThread], NULL, addonMySQL, (void*) addonParamsInfo))
                {
                    g_critical("Critical: %s: addonMySQL: %u", strerror(errno), numThread);
                    pthread_exit(NULL);
                }
                break;
            case 5:
                if (snmpInit == FALSE)
                {
                    /* Initialize the SNMP library */
                    init_snmp("addonSNMP");
                    SOCK_STARTUP;

                    snmpInit = TRUE;
                }
                
                if (pthread_create(&addonsMgrParams->addonsThreads[numThread], NULL, addonSNMP, (void*) addonParamsInfo))
                {
                    g_critical("Critical: %s: addonSNMP: %u", strerror(errno), numThread);
                    pthread_exit(NULL);
                }
                break;
            default:
                g_critical("Critical: idAddon %d does'nt exist", *addonInfo->idAddon);
                pthread_exit(NULL);
                break;
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

    if (snmpInit == TRUE)
    {
        SOCK_CLEANUP;
    }
    
    pthread_exit(NULL);

}

