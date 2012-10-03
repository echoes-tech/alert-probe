/* 
 * Addon File
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

#include "addon/addonFile.h"

int addonFileRegex(
                   CollectQueue *collectQueue,
                   const char *line,
                   unsigned int lineNum,
                   void *params,
                   unsigned short lotNum,
                   IDList *idList,
                   time_t *now
                   )
{
    SearchInfoParams2_1 *searchInfoParams = (SearchInfoParams2_1*)params;
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

                    /* Cleanup */
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
        else if (searchInfoParams->match == REG_NOMATCH)
        {
            return(EXIT_SUCCESS);
        }
        else
        {
            char *text;
            size_t size;

            size = regerror(searchInfoParams->err, &searchInfoParams->preg, NULL, 0);
            text = malloc(sizeof (*text) * size);
            if (text)
            {
                regerror(searchInfoParams->err, &searchInfoParams->preg, text, size);
                fprintf(stderr, "%s\n", text);
                free(text);
            }
            else
            {
                fprintf(stderr, "Insufficient memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return(EXIT_SUCCESS);
}

int addonFileLocation(
                      CollectQueue *collectQueue,
                      const char *line,
                      unsigned int lineNum,
                      void *params,
                      unsigned short lotNum,
                      IDList *idList,
                      time_t *now
                      )
{
    SearchInfoParams2_2 *searchInfoParams = (SearchInfoParams2_2*)params;

    char *res = NULL;
    char *values[1];

    if (lineNum == searchInfoParams->line)
    {
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
    }

    return(EXIT_SUCCESS);
}

void *addonFile(void *arg)
{
    AddonParamsInfo *addonParamsInfo = (AddonParamsInfo*) arg;
    SrcInfoParams2 *srcInfoParams = (SrcInfoParams2*)addonParamsInfo->params;

    time_t now, temp;
    FILE* file = NULL;
    char line[MAX_SIZE] = "";
    unsigned int n = 0;

#ifndef NDEBUG
    printf("Dans le thread addonFile.\n");
#endif

    /* What time is it ? */
    time(&now);
    
    /* Method to know when start the loop */
    temp =  ((int)(now / *addonParamsInfo->period) * *addonParamsInfo->period) + *addonParamsInfo->period;
    
    /* Diff between now and the start of the loop */
    SLEEP(difftime(temp, now));
    while(1)
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
        file = fopen(srcInfoParams->path, "r");

        if (file != NULL)
        {
            /* What time is it ? */
            time(&now);

            /* Reading file line by line */
            while (fgets(line, MAX_SIZE, file) != NULL)
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
                            addonFileRegex(
                                           addonParamsInfo->collectQueue,
                                           line,
                                           n,
                                           addonTypeParamsInfo->params,
                                           addonParamsInfo->lotNum,
                                           &addonTypeParamsInfo->IDList,
                                           &now
                                           );
                            break;
                        case 2:
                            addonFileLocation(
                                              addonParamsInfo->collectQueue,
                                              line,
                                              n,
                                              addonTypeParamsInfo->params,
                                              addonParamsInfo->lotNum,
                                              &addonTypeParamsInfo->IDList,
                                              &now
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
            
                ++n;
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

        SLEEP(*addonParamsInfo->period);
    }

    pthread_exit(NULL);
}

