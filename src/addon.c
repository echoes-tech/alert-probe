/* 
 * Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */

#include "addon.h"

int pushCollectQueue(CollectQueue *collectQueue, unsigned int idPlg, unsigned int idAsset, unsigned int idSrc, unsigned int idSearch, unsigned int numSubSearch, const char *value, time_t time)
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
    new->numSubSearch = numSubSearch;
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
}

void *addonLoop(void *arg)
{
    LoopParams *loopParams = (LoopParams*) arg;

    time_t now;

    // What time is it ?
    time(&now);

    switch(loopParams->idAddon)
    {
    case 1:
        switch(loopParams->idType)
        {
        case 1:
            break;
        default:
            break;        
        }
        break;
    case 2:
        switch(loopParams->idType)
        {
        case 1:
            break;
        case 2:
        {
            AddonLocationFileParams *paramsTmp = (AddonLocationFileParams*) loopParams->params;
            AddonLocationFileParams alfp = {
                paramsTmp->idPlg,
                paramsTmp->idAsset,
                paramsTmp->idSrc,
                paramsTmp->idSearch,
                paramsTmp->period,
                paramsTmp->staticValues,
                paramsTmp->line,
                paramsTmp->firstChar,
                paramsTmp->length,
                "", // path
                paramsTmp->collectQueue
            };
            
            strcpy(alfp.path, paramsTmp->path);
            
            pthread_mutex_lock (&loopParams->mutex); /* On verrouille le mutex */
			pthread_cond_signal (&loopParams->condition); /* On délivre le signal : condition remplie */
			pthread_mutex_unlock (&loopParams->mutex); /* On déverrouille le mutex */

            // Method to know when start the loop
            time_t temp =  ((int)(now / alfp.period) * alfp.period) + alfp.period;

            // Diff between now and the start of the loop
            SLEEP(difftime(temp, now));
            while(1)
            {
                pthread_t alft;

                printf("Création du thread addonLocationFile.\n");

                if (pthread_create(&alft, NULL, addonLocationFile, (void*) &alfp))
                {
                    perror("pthread_create");
                    pthread_exit(NULL);
                }

                SLEEP(alfp.period);
            }
            break;
        }
        default:
            break;        
        }
        break;
    case 3:
        switch(loopParams->idType)
        {
        case 1:
            break;
        case 2:
        {
            AddonLocationLogParams *paramsTmp = (AddonLocationLogParams*) loopParams->params;
            AddonLocationLogParams allp = {
                paramsTmp->idPlg,
                paramsTmp->idAsset,
                paramsTmp->idSrc,
                paramsTmp->idSearch,
                paramsTmp->period,
                paramsTmp->staticValues,
                0, // nbLine
                paramsTmp->firstChar,
                paramsTmp->length,
                "", // path
                paramsTmp->collectQueue
            };

            strcpy(allp.path, paramsTmp->path);
            
            pthread_mutex_lock (&loopParams->mutex); /* On verrouille le mutex */
			pthread_cond_signal (&loopParams->condition); /* On délivre le signal : condition remplie */
			pthread_mutex_unlock (&loopParams->mutex); /* On déverrouille le mutex */

            // Method to know when start the loop
            time_t temp =  ((int)(now / allp.period) * allp.period) + allp.period;

            // Diff between now and the start of the loop
            SLEEP(difftime(temp, now));
            while(1)
            {
                pthread_t allt;

                printf("Création du thread addonLocationLog.\n");

                if (pthread_create(&allt, NULL, addonLocationLog, (void*) &allp))
                {
                    perror("pthread_create");
                    pthread_exit(NULL);
                }

                SLEEP(allp.period);
            }
            break;
        }
        default:
            break;        
        }
        break;
    default:
        break;
    }

    pthread_exit(NULL);
}

void *addonLocationFile(void *arg)
{
    printf("Dans le thread addonLocationFile.\n");
    
    AddonLocationFileParams *params = (AddonLocationFileParams*) arg;
    
    FILE* file = NULL;
    char line[MAX_SIZE] = "", res[MAX_SIZE]= "";
    unsigned int n = 0, i = 0;
    
    time_t now;

    // What time is it ?
    time(&now);

    // Opening file
    file = fopen(params->path, "r");

    if (file != NULL)
    {
        // Reading file line by line
        while (n < params->line && fgets(line, MAX_SIZE, file) != NULL)
        {
            ++n;
        }

        //TODO: Gérer le fait que le numéro de ligne demandée soit supérieur au nombre de ligne du fichier

        // Closing file
        fclose(file);

        for(i=0; i < params->length; ++i)
        {
            res[i] = line[params->firstChar + i - 1];
        }
        
        if (pushCollectQueue(params->collectQueue, params->idPlg, params->idAsset, params->idSrc, params->idSearch, 0, res, now))
        {
            perror("pushCollectQueue()");
            return (errno);
        }
    }
    else
    {
        perror("fopen()");
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

void *addonLocationLog(void *arg)
{
    printf("Dans le thread addonLocationLog.\n");
    
    AddonLocationLogParams *params = (AddonLocationLogParams*) arg;
    
    FILE* file = NULL;
    char line[MAX_SIZE] = "", res[MAX_SIZE]= "";
    unsigned int n = 0, i = 0, j = 0;
    
    //TODO: faire un check du protocole file://, socket://, etc.
    char *path;
    path = params->path + 7;
    
    time_t now;

    // What time is it ?
    time(&now);

    // Opening file
    file = fopen(path, "r");

    //TODO: Gérer la remise à zéro du fichier dans le labs de temps de la période
    
    if (file != NULL)
    {
        if (params->nbLine == 0)
        {
            // Reading file line by line
            while (fgets(line, MAX_SIZE, file) != NULL)
            {
                for(i=0; i < params->length; ++i)
                {
                    res[i] = line[params->firstChar + i - 1];
                }
                if (pushCollectQueue(params->collectQueue, params->idPlg, params->idAsset, params->idSrc, params->idSearch, 0, res, now))
                {
                    perror("pushCollectQueue()");
                    return (errno);
                }
                n++;
            }
        }
        else
        {
            // Reading file line by line
            while (fgets(line, MAX_SIZE, file) != NULL)
            {
                n++;
            }
        }
        
        if (n > params->nbLine)
        {
            // Reading file line by line
            while (j < params->nbLine && fgets(line, MAX_SIZE, file) != NULL)
            {
                j++;
            }
            // Reading file line by line
            while (fgets(line, MAX_SIZE, file) != NULL)
            {
                for(i=0; i < params->length; ++i)
                {
                    res[i] = line[params->firstChar + i - 1];
                }
                if (pushCollectQueue(params->collectQueue, params->idPlg, params->idAsset, params->idSrc, params->idSearch, 0, res, now))
                {
                    perror("pushCollectQueue()");
                    return (errno);
                }
            }
        }
        else if (n < params->nbLine)
        {
            // Reading file line by line
            while (fgets(line, MAX_SIZE, file) != NULL)
            {
                for(i=0; i < params->length; ++i)
                {
                    res[i] = line[params->firstChar + i - 1];
                }
                //TODO: envoyer le résultat
                printf("time: %f, res: %s, ids: %d-%d-%d-%d.\n", (double) now, res, params->idPlg, params->idAsset, params->idSrc, params->idSearch);
                n++;
            }
        }

        params->nbLine = n;
        
        // Closing file
        fclose(file);
    }
    else
    {
        perror("fopen()");
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

//int addon(unsigned int *nbThreads, PlgList *plgList, pthread_t addonsThreads[])
void *addon(void *arg)
{
    AddonsMgrParams *addonParams = (AddonsMgrParams*) arg;
    
    // The position on the threads table
    unsigned int numThread = 0;

    PlgInfo *plgInfo = addonParams->plgList;
    // Tant que l'on n'est pas au bout de la liste
    while (plgInfo != NULL)
    {
        SrcInfo *srcInfo = plgInfo->srcList;
        while (srcInfo != NULL)
        {
            if (srcInfo->params == NULL)
            {
                printf("Invalid JSON Node\n");
                pthread_exit(NULL);
            }
            SearchInfo *searchInfo = srcInfo->searchList;
            while (searchInfo != NULL)
            {
                switch (srcInfo->idAddon)
                {
                case 2:
                {
                    SrcInfoParams2 *srcInfoParams2 = (SrcInfoParams2*)srcInfo->params;
                    switch (searchInfo->idType)
                    {
                    case 1:
                    {
                        SearchInfoParams2_1 *searchInfoParams2_1 = (SearchInfoParams2_1*)searchInfo->params;

                        break;
                    }
                    case 2:
                    {
                        
                        SearchInfoParams2_2 *searchInfoParams2_2 = (SearchInfoParams2_2*)searchInfo->params;
                        
                        AddonLocationFileParams alfp =
                        {
                            plgInfo->idPlg,
                            plgInfo->idAsset,
                            srcInfo->idSrc,
                            searchInfo->idSearch,
                            searchInfo->period,
                            searchInfo->staticValues,
                            searchInfoParams2_2->line,
                            searchInfoParams2_2->firstChar,
                            searchInfoParams2_2->length,
                            "", // path
                            &addonParams->collectQueue
                        };
                        strcpy(alfp.path, srcInfoParams2->path);

                        addonParams->loopsParams[numThread].idAddon = srcInfo->idAddon;
                        addonParams->loopsParams[numThread].idType = searchInfo->idType;
                        addonParams->loopsParams[numThread].params = (void*)&alfp;

                        if (pthread_create(&addonParams->addonsThreads[numThread], NULL, addonLoop, (void*)&addonParams->loopsParams[numThread]))
                        {
                            perror("pthread_create");
                            pthread_exit(NULL);
                        }
                        
                        pthread_mutex_lock(&addonParams->loopsParams[numThread].mutex); /* On verrouille le mutex */
                        pthread_cond_wait (&addonParams->loopsParams[numThread].condition, &addonParams->loopsParams[numThread].mutex); /* On attend que la condition soit remplie */
                        pthread_mutex_unlock(&addonParams->loopsParams[numThread].mutex); /* On déverrouille le mutex */
                        
                        pthread_mutex_destroy(&addonParams->loopsParams[numThread].mutex);
                        break;
                    }
                    default:
                        break;
                    }
                    break;
                }
                case 3:
                {
                    SrcInfoParams3 *srcInfoParams3 = (SrcInfoParams3*)srcInfo->params;
                    switch (searchInfo->idType)
                    {
                    case 1:
                    {
                        SearchInfoParams3_1 *searchInfoParams3_1 = (SearchInfoParams3_1*)searchInfo->params;

                        break;
                    }
                    case 2:
                    {
                        
                        SearchInfoParams3_2 *searchInfoParams3_2 = (SearchInfoParams3_2*)searchInfo->params;
                        
                        AddonLocationLogParams allp =
                        {
                            plgInfo->idPlg,
                            plgInfo->idAsset,
                            srcInfo->idSrc,
                            searchInfo->idSearch,
                            searchInfo->period,
                            searchInfo->staticValues,
                            0, // nbLine
                            searchInfoParams3_2->firstChar,
                            searchInfoParams3_2->length,
                            "", // path
                            &addonParams->collectQueue
                        };
                        strcpy(allp.path, srcInfoParams3->path);

                        addonParams->loopsParams[numThread].idAddon = srcInfo->idAddon;
                        addonParams->loopsParams[numThread].idType = searchInfo->idType;
                        addonParams->loopsParams[numThread].params = (void*)&allp;

                        if (pthread_create(&addonParams->addonsThreads[numThread], NULL, addonLoop, (void*) &addonParams->loopsParams[numThread]))
                        {
                            perror("pthread_create");
                            pthread_exit(NULL);
                        }

                        pthread_mutex_lock(&addonParams->loopsParams[numThread].mutex); /* On verrouille le mutex */
                        pthread_cond_wait (&addonParams->loopsParams[numThread].condition, &addonParams->loopsParams[numThread].mutex); /* On attend que la condition soit remplie */
                        pthread_mutex_unlock(&addonParams->loopsParams[numThread].mutex); /* On déverrouille le mutex */
                        
                        pthread_mutex_destroy(&addonParams->loopsParams[numThread].mutex);
                        break;
                    }
                    default:
                        break;
                    }
                    break;
                }
                default:
                    break;
                }

                // Increment the iterator
                numThread++;
                
                searchInfo = searchInfo->nxt;
            }

            srcInfo = srcInfo->nxt;
        }

        // On avance d'une case
        plgInfo = plgInfo->nxt;
    }

    printf("Fin du chargement des addons\n");
    
    /* Attente de la fin des threads */
    unsigned int i;
    for (i = 0; i < numThread; i++)
    {
        pthread_join(addonParams->addonsThreads[i], NULL);
    }
    
    pthread_exit(NULL);
}

