/* 
 * Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */

#include "addon.h"

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
            AddonLocationFileParams *params = (AddonLocationFileParams*) loopParams->params;
            
            // Method to know when start the loop
            time_t temp =  ((int)(now / params->period) * params->period) + params->period;

            // Diff between now and the start of the loop
            SLEEP(difftime(temp, now));
            while(1)
            {
                pthread_t alft;

                printf("Création du thread addonLocationFile.\n");

                if (pthread_create(&alft, NULL, addonLocationFile, (void*) loopParams->params))
                {
                    perror("pthread_create");
                    pthread_exit(NULL);
                }

                SLEEP(params->period);
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

        //TODO: envoyer le résultat
        printf("time: %f, res: %s, ids: %d-%d-%d-%d.\n", (double) now, res, params->idPlg, params->idAsset, params->idSrc, params->idSearch);
    }
    else
    {
        perror("fopen()");
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

int addon(unsigned int *nbThreads, PlgList *plgList, pthread_t addonsThreads[])
//int addon(unsigned int *nbThreads, PlgList *plgList, pthread_t addonsThreads[], LoopParams loopParams[], AddonLocationFileParams alp[])
{
    // The position on the threads table
    unsigned int numThread = 0;
    char path[255], regex[255] = "";

    PlgInfo *plgInfo = *plgList;
    // Tant que l'on n'est pas au bout de la liste
    while (plgInfo != NULL)
    {
        SrcInfo *srcInfo = plgInfo->srcList;
        while (srcInfo != NULL)
        {
            if (srcInfo->params == NULL)
            {
                printf("Invalid JSON Node\n");
                return (EXIT_FAILURE);
            }
            SearchInfo *searchInfo = srcInfo->searchList;
            while (searchInfo != NULL)
            {
                switch (srcInfo->idAddon)
                {
                case 2:
                case 3:
                    switch (searchInfo->idType)
                    {
                    case 1:
                    {
                        SrcInfoParams3 *srcInfoParams3 = (SrcInfoParams3*)srcInfo->params;
                        SearchInfoParams2_1 *searchInfoParams2_1 = (SearchInfoParams2_1*)searchInfo->params;

/*
                        // On affiche
                        printf("idPlg: %d\n", plgInfo->idPlg);
                        printf("idAsset: %d\n", plgInfo->idAsset);
                        printf("idSrc: %d\n", srcInfo->idSrc);
                        printf("idAddon: %d\n", srcInfo->idAddon);
                        printf("path: %s\n", srcInfoParams3->path);
                        printf("idSearch: %d\n", searchInfo->idSearch);
                        printf("idType: %d\n", searchInfo->idType);
                        printf("period: %d\n", searchInfo->period);
                        printf("staticValues: %d\n", searchInfo->staticValues);
                        printf("regex: %s\n", searchInfoParams2_1);
*/
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
                            searchInfoParams2_2->line, // line
                            searchInfoParams2_2->firstChar, // firstChar
                            searchInfoParams2_2->length, //length
                            "" // path
                        };
                        
/*
                        alfp[numThread]->idPlg = plgInfo.idPlg;
                        alfp[numThread]->idAsset = plgInfo.idAsset;
                        alfp[numThread]->idSrc = srcInfo.idSrc;
                        alfp[numThread]->idSearch = searchInfo.idSearch;
                        alfp[numThread]->period = searchInfo.period;
                        alfp[numThread]->staticValues = searchInfo.staticValues;
*/
                        SrcInfoParams2 *srcInfoParams2 = (SrcInfoParams2*)srcInfo->params;
                        strcpy(alfp.path, srcInfoParams2->path);

/*
                        // On affiche
                        printf("idPlg: %d\n", alfp.idPlg);
                        printf("idAsset: %d\n", alfp.idAsset);
                        printf("idSrc: %d\n", alfp.idSrc);
                        printf("idAddon: %d\n", srcInfo->idAddon);
                        printf("path: %s\n", alfp.path);
                        printf("idSearch: %d\n", alfp.idSearch);
                        printf("idType: %d\n", searchInfo->idType);
                        printf("period: %d\n", alfp.period);
                        printf("staticValues: %d\n", alfp.staticValues);
                        printf("line: %i\n",alfp.line);
                        printf("firstChar: %i\n", alfp.firstChar);
                        printf("length: %i\n", alfp.length);
*/

                        LoopParams loopParams = {srcInfo->idAddon, searchInfo->idType, (void*)&alfp};

/*
                        if (pthread_create(&addonsThreads[numThread], NULL, addonLoop, (void*)&loopParams))
                        {
                            perror("pthread_create");
                            return EXIT_FAILURE;
                        }
                        if (pthread_join(addonsThreads[numThread], NULL))
                        {

                            perror("pthread_join");
                            return EXIT_FAILURE;

                        } 
*/

                        break;
                    }
                    default:
                        break;
                    }
                    break;
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

    return (EXIT_SUCCESS);
}

