/* 
 * Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */

#include "addon/addon.h"

int pushCollectQueue(
                     CollectQueue *collectQueue,
                     const unsigned int idPlg,
                     const unsigned int idAsset,
                     const unsigned int idSrc,
                     const unsigned int idSearch,
                     const unsigned int valueNum,
                     const unsigned short lotNum,
                     const char *value,
                     time_t time
                     )
{
    CollectQueueElement *new = calloc(1, sizeof(CollectQueueElement));
    if (collectQueue == NULL || new == NULL)
    {
        exit(EXIT_FAILURE);
    }

    new->idPlg = idPlg;
    new->idAsset = idAsset;
    new->idSrc = idSrc;
    new->idSearch = idSearch;
    new->valueNum = valueNum;
    new->lotNum = lotNum;
    strcpy(new->value, value);
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
    pthread_mutex_unlock (& collectQueue->mutex);

    return (EXIT_SUCCESS);
}

void *addon(void *arg)
{
    AddonsMgrParams *addonsMgrParams = (AddonsMgrParams*) arg;
    
    /* Thread Counter + Position on the threads table */
    unsigned int i, numThread = 0;

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
            case 2:
                if (pthread_create(&addonsMgrParams->addonsThreads[numThread], NULL, addonFile, (void*) addonParamsInfo))
                {
                    perror("pthread_create");
                    pthread_exit(NULL);
                }
                break;
            case 3:
                if (pthread_create(&addonsMgrParams->addonsThreads[numThread], NULL, addonLog, (void*) addonParamsInfo))
                {
                    perror("pthread_create");
                    pthread_exit(NULL);
                }
                break;
            default:
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

    pthread_exit(NULL);

}

