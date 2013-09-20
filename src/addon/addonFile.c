/* 
 * Addon File
 * @author ECHOES Technologies (FPO)
 * @date 06/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
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

    char **values = NULL;

    values = calloc(searchInfoParams->nmatch, sizeof (char*));

    if(values)
    {
        if (searchRegex(
                        values,
                        line,
                        &searchInfoParams->err,
                        &searchInfoParams->preg,
                        &searchInfoParams->nmatch,
                        searchInfoParams->pmatch
                        ))
        {
            /* Cleanup */
            free(values);
            values = NULL;
            return EXIT_FAILURE;
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
            /* Cleanup */
            free(values);
            values = NULL;
            return EXIT_FAILURE;
        }

        /* Cleanup */
        free(values);
        values = NULL;
    }
    else
    {
        g_warning("Critical: Insufficient memory");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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

    char *values[1];

    if (lineNum == searchInfoParams->line)
    {
        if (searchLocation(
                           values,
                           line,
                           &searchInfoParams->length,
                           &searchInfoParams->firstChar
                           ))
            return EXIT_FAILURE;

        if (pushCollectQueue(
                             collectQueue,
                             idList,
                             lotNum,
                             lineNum,
                             1,
                             values,
                             *now
                             ))
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void *addonFile(void *arg)
{
    AddonParamsInfo *addonParamsInfo = (AddonParamsInfo*) arg;
    SrcInfoParams2 *srcInfoParams = (SrcInfoParams2*)addonParamsInfo->params;

    time_t now;
    FILE* file = NULL;
    char line[MAX_SIZE] = "";
    unsigned int n = 0;

#ifndef NDEBUG
    printf("Dans le thread addonFile.\n");
#endif

    while(*addonParamsInfo->signum == 0)
    {        
        addonSleep(*addonParamsInfo->period);
        
        addonParamsInfo->lotNum = increaseLotNum(
                                                 addonParamsInfo->mutex,
                                                 addonParamsInfo->lotNumPtr
                                                 );

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
                            g_warning("Warning: idType %d does'nt exist for the File addon.", *addonTypeInfo->idType);
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
    }
    
#ifndef NDEBUG
    printf("Fin du thread addonFile.\n");
#endif

    pthread_exit(NULL);
}

