/* 
 * Addon Log
 * @author ECHOES Technologies (FPO)
 * @date 06/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "addon/addonLog.h"

int addonLogRegex(
                  CollectQueue *collectQueue,
                  const char *line,
                  void *params,
                  unsigned short lotNum,
                  unsigned int *valueNum,
                  IDList *idList,
                  time_t *now
                  )
{
    SearchInfoParams3_1 *searchInfoParams = (SearchInfoParams3_1*)params;
    char *res = NULL;

    if (searchInfoParams->pmatch)
    {
        /* TODO: Comprendre le comportement étrange avec \d */
        searchInfoParams->match = regexec(
                                          &searchInfoParams->preg,
                                          line,
                                          searchInfoParams->nmatch,
                                          searchInfoParams->pmatch,
                                          0
                                          );
        if (searchInfoParams->match == 0)
        {
            for (*valueNum = 1; *valueNum < searchInfoParams->nmatch; ++*valueNum)
            {
                int start = searchInfoParams->pmatch[*valueNum].rm_so;
                int end = searchInfoParams->pmatch[*valueNum].rm_eo;
                size_t size = end - start;

                res = malloc(sizeof (*res) * (size  + 1));
                if (res)
                {
                    IDInfo *idInfo;
                    strncpy(res, &line[start], size);
                    res[size] = '\0';
                    idInfo = *idList;
                    /* Tant que l'on n'est pas au bout de la liste */
                    while (idInfo != NULL)
                    {
                        if (pushCollectQueue(collectQueue, *idInfo->idPlg, *idInfo->idAsset, *idInfo->idSrc, *idInfo->idSearch, *valueNum, lotNum, res, *now))
                        {
                            perror("pushCollectQueue()");
                            exit(EXIT_FAILURE);
                        }
                        /* On avance d'une case */
                        idInfo = idInfo->nxt;
                    }
                    free(res);
                }
            }
        }
    }

    return(EXIT_SUCCESS);
}

int addonLogLocation(
                     CollectQueue *collectQueue,
                     const char *line,
                     void *params,
                     unsigned short lotNum,
                     unsigned int *valueNum,
                     IDList *idList,
                     time_t *now
                     )
{
    SearchInfoParams3_2 *searchInfoParams = (SearchInfoParams3_2*)params;

    IDInfo *idInfo;

    char res[MAX_SIZE] = "";
    unsigned int i = 0;

    for (i = 0; i < searchInfoParams->length; ++i)
    {
        res[i] = line[searchInfoParams->firstChar + i - 1];
    }

    idInfo = *idList;
    /* Tant que l'on n'est pas au bout de la liste */
    while (idInfo != NULL)
    {
        if (pushCollectQueue(collectQueue, *idInfo->idPlg, *idInfo->idAsset, *idInfo->idSrc, *idInfo->idSearch, *valueNum, lotNum, res, *now))
        {
            perror("pushCollectQueue()");
            exit(EXIT_FAILURE);
        }
        /* On avance d'une case */
        idInfo = idInfo->nxt;
    }

    return(EXIT_SUCCESS);
}

void whileAddonTypeInfo(AddonParamsInfo *addonParamsInfo, const char *line, time_t *now)
{
    AddonTypeInfo *addonTypeInfo = addonParamsInfo->addonTypeList;

    /* Tant que l'on n'est pas au bout de la liste */
    while (addonTypeInfo != NULL)
    {
        AddonTypeParamsInfo *addonTypeParamsInfo = addonTypeInfo->addonTypeParamsList;

        /* Tant que l'on n'est pas au bout de la liste */
        while (addonTypeParamsInfo != NULL)
        {
            switch (*addonTypeInfo->idType)
            {
            case 1:
                addonLogRegex(
                              addonParamsInfo->collectQueue,
                              line,
                              addonTypeParamsInfo->params,
                              addonParamsInfo->lotNum,
                              &addonTypeParamsInfo->valueNum,
                              &addonTypeParamsInfo->IDList,
                              now
                              );
                break;
            case 2:
                addonLogLocation(
                                 addonParamsInfo->collectQueue,
                                 line,
                                 addonTypeParamsInfo->params,
                                 addonParamsInfo->lotNum,
                                 &addonTypeParamsInfo->valueNum,
                                 &addonTypeParamsInfo->IDList,
                                 now
                                 );
                break;
            default:
                break;
            }

            /* On avance d'une case */
            addonTypeParamsInfo = addonTypeParamsInfo->nxt;
        }

        /* On avance d'une case */
        addonTypeInfo = addonTypeInfo->nxt;
    }
    return;
}

void *addonLog(void *arg)
{
    AddonParamsInfo *addonParamsInfo = (AddonParamsInfo*) arg;
    SrcInfoParams3 *srcInfoParams = (SrcInfoParams3*) addonParamsInfo->params;

    time_t now, temp;
    FILE* file = NULL;
    char line[MAX_SIZE] = "";
    unsigned int i = 0, n = 0;

    /*TODO: faire un check du protocole file://, socket://, etc. */
    const char *path = srcInfoParams->path + 7;

#ifndef NDEBUG
    printf("Dans le thread addonLog.\n");
#endif

    /* What time is it ? */
    time(&now);

    /* Method to know when start the loop */
    temp = ((int) (now / *addonParamsInfo->period) * *addonParamsInfo->period) + *addonParamsInfo->period;

    /* Diff between now and the start of the loop */
    SLEEP(difftime(temp, now));
    while (1)
    {
        /* Debut de la zone protegee. */
        pthread_mutex_lock(addonParamsInfo->mutex);

        ++*addonParamsInfo->lotNumPtr;
        addonParamsInfo->lotNum = *addonParamsInfo->lotNumPtr;

        /* Fin de la zone protegee. */
        pthread_mutex_unlock(addonParamsInfo->mutex);
        
        /* Number of line */
        n = 1;

        /* What time is it ? */
        time(&now);

        /* Opening file */
        file = fopen(path, "r");

        if (file != NULL)
        {
            if (srcInfoParams->nbLine == 0)
            {
                /* Reading file line by line */
                while (fgets(line, MAX_SIZE, file) != NULL)
                {
                    whileAddonTypeInfo(addonParamsInfo, line, &now);
                    ++n;
                }
            }
            else
            {
                /* Count the number of line */
                while (fgets(line, MAX_SIZE, file) != NULL)
                {
                    ++n;
                }

                /* Closing file */
                fclose(file);

                /* Opening file */
                file = fopen(path, "r");

                if (file != NULL)
                {
                    if (n > srcInfoParams->nbLine)
                    {
                        /* Reading file line by line */
                        while (i < srcInfoParams->nbLine && fgets(line, MAX_SIZE, file) != NULL)
                        {
                            ++i;
                        }
                        /* Reading file line by line */
                        while (fgets(line, MAX_SIZE, file) != NULL)
                        {
                            whileAddonTypeInfo(addonParamsInfo, line, &now);
                        }
                    }
                    else if (n < srcInfoParams->nbLine)
                    {
                        /* Reading file line by line */
                        while (fgets(line, MAX_SIZE, file) != NULL)
                        {
                            whileAddonTypeInfo(addonParamsInfo, line, &now);
                        }
                    }
                }
            }

        }
        else
        {
            perror("fopen()");
            pthread_exit(NULL);
        }

        /* Closing file */
        fclose(file);
        
        srcInfoParams->nbLine = n;

        SLEEP(*addonParamsInfo->period);
    }

    pthread_exit(NULL);
}

