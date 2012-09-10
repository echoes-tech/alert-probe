/* 
 * Addon File
 * @author ECHOES Technologies (FPO)
 * @date 06/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT COMPANY AUTHORIZATION.
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
                   unsigned int *valueNum,
                   IDList *idList,
                   time_t *now
                   )
{
    SearchInfoParams2_1 *searchInfoParams = (SearchInfoParams2_1*)params;
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
                        if (pushCollectQueue(collectQueue, *idInfo->idPlg, *idInfo->idAsset, *idInfo->idSrc, *idInfo->idSearch, *valueNum, lotNum, lineNum, res, *now))
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
                      unsigned int *valueNum,
                      IDList *idList,
                      time_t *now
                      )
{
    SearchInfoParams2_2 *searchInfoParams = (SearchInfoParams2_2*)params;

    char res[MAX_SIZE]= "";
    unsigned int i = 0;

    if (lineNum == searchInfoParams->line)
    {
        IDInfo *idInfo;
        for (i = 0; i < searchInfoParams->length; ++i)
        {
            res[i] = line[searchInfoParams->firstChar + i - 1];
        }

        idInfo = *idList;
        /* Tant que l'on n'est pas au bout de la liste */
        while (idInfo != NULL)
        {
            if (pushCollectQueue(collectQueue, *idInfo->idPlg, *idInfo->idAsset, *idInfo->idSrc, *idInfo->idSearch, *valueNum, lotNum, lineNum, res, *now))
            {
                perror("pushCollectQueue()");
                exit(EXIT_FAILURE);
            }
            /* On avance d'une case */
            idInfo = idInfo->nxt;
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
        
        /* What time is it ? */
        time(&now);

        /* Opening file */
        file = fopen(srcInfoParams->path, "r");

        if (file != NULL)
        {
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
                                           &addonTypeParamsInfo->valueNum,
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
                                              &addonTypeParamsInfo->valueNum,
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
        }
        else
        {
            perror("fopen()");
            pthread_exit(NULL);
        }
        
        /* Closing file */
        fclose(file);

        SLEEP(*addonParamsInfo->period);
    }

    pthread_exit(NULL);
}

