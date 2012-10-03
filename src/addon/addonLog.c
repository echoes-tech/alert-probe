/* 
 * Addon Log
 * @author ECHOES Technologies (FPO)
 * @date 06/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "addon/addonLog.h"

int addonLogRegex(
                  CollectQueue *collectQueue,
                  const char *line,
                  unsigned int lineNum,
                  void *params,
                  unsigned short lotNum,
                  IDList *idList,
                  time_t *now
                  )
{
    SearchInfoParams3_1 *searchInfoParams = (SearchInfoParams3_1*)params;
    char *res = NULL;
    char **values = NULL;
    unsigned short i = 0;

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
            values = calloc(searchInfoParams->nmatch, sizeof (char*));

            for (i = 1; i < searchInfoParams->nmatch; ++i)
            {
                int start = searchInfoParams->pmatch[i].rm_so;
                int end = searchInfoParams->pmatch[i].rm_eo;
                size_t size = end - start;

                res = malloc(sizeof (*res) * (size  + 1));
                if (res)
                {
                    strncpy(res, &line[start], size);
                    res[size] = '\0';

                    values[i - 1] = strdup(res);

                    free(res);
                }
            }
            if (pushCollectQueue(
                                 collectQueue,
                                 idList,
                                 lotNum,
                                 lineNum,
                                 (searchInfoParams->nmatch - 1),
                                 values,
                                 *now
                                 ))
            {
                perror("pushCollectQueue()");
                exit(EXIT_FAILURE);
            }

            /* Cleanup */
            free(values);
        }
    }

    return(EXIT_SUCCESS);
}

int addonLogLocation(
                     CollectQueue *collectQueue,
                     const char *line,
                     unsigned int lineNum,
                     void *params,
                     unsigned short lotNum,
                     IDList *idList,
                     time_t *now
                     )
{
    SearchInfoParams3_2 *searchInfoParams = (SearchInfoParams3_2*)params;

    char *res = NULL;
    char *values[1];

    res = calloc(searchInfoParams->length, sizeof (char));
    if (res)
    {
        strncpy(
                res,
                &line[searchInfoParams->firstChar - 1],
                searchInfoParams->length
                );


        values[0] = strdup(res);

        /* Cleanup */
        free(res);
    }

    if (pushCollectQueue(
                         collectQueue,
                         idList,
                         lotNum,
                         lineNum,
                         1,
                         values,
                         *now
                         ))
    {
        perror("pushCollectQueue()");
        exit(EXIT_FAILURE);
    }

    return(EXIT_SUCCESS);
}

void whileAddonTypeInfo(
                        AddonParamsInfo *addonParamsInfo,
                        const char *line,
                        unsigned int lineNum,
                        time_t *now
                        )
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
                              lineNum,
                              addonTypeParamsInfo->params,
                              addonParamsInfo->lotNum,
                              &addonTypeParamsInfo->IDList,
                              now
                              );
                break;
            case 2:
                addonLogLocation(
                                 addonParamsInfo->collectQueue,
                                 line,
                                 lineNum,
                                 addonTypeParamsInfo->params,
                                 addonParamsInfo->lotNum,
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
    unsigned int i = 1, n = 1;

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

        /* Opening file */
        file = fopen(path, "r");

        if (file != NULL)
        {
            /* What time is it ? */
            time(&now);

            if (srcInfoParams->nbLine == 0)
            {
                if (srcInfoParams->lastNLines == 0)
                {
                    /* Reading file line by line */
                    while (fgets(line, MAX_SIZE, file) != NULL)
                    {
                        whileAddonTypeInfo(addonParamsInfo, line, n, &now);
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
                        /* Number of line */
                        i = 1;
                        /* Reading file line by line */
                        while (i < (n - srcInfoParams->lastNLines) && fgets(line, MAX_SIZE, file) != NULL)
                        {
                            ++i;
                        }
                        /* Reading file line by line */
                        while (fgets(line, MAX_SIZE, file) != NULL)
                        {
                            whileAddonTypeInfo(addonParamsInfo, line, i, &now);
                            ++i;
                        }
                    }
                    else
                    {
                        g_warning(
                                  "Error opening file %s: %s",
                                  srcInfoParams->path,
                                  strerror(errno)
                                  );
                    }
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
                        i = 1;
                        /* Reading file line by line */
                        while (i < srcInfoParams->nbLine && fgets(line, MAX_SIZE, file) != NULL)
                        {
                            ++i;
                        }
                        /* Reading file line by line */
                        while (fgets(line, MAX_SIZE, file) != NULL)
                        {
                            whileAddonTypeInfo(addonParamsInfo, line, i, &now);
                            ++i;
                        }
                    }
                    else if (n < srcInfoParams->nbLine)
                    {
                        /* Reading file line by line */
                        while (fgets(line, MAX_SIZE, file) != NULL)
                        {
                            whileAddonTypeInfo(addonParamsInfo, line, i, &now);
                            ++i;
                        }
                    }
                }
                else
                {
                    g_warning(
                              "Error opening file %s: %s",
                              srcInfoParams->path,
                              strerror(errno)
                              );
                }
            }
            
            /* Closing file */
            fclose(file);
        }
        else
        {
            g_warning(
                      "Error opening file %s: %s",
                      srcInfoParams->path,
                      strerror(errno)
                      );
        }
        
        srcInfoParams->nbLine = n;

        SLEEP(*addonParamsInfo->period);
    }

    pthread_exit(NULL);
}

