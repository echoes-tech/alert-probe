/* 
 * Addon MySQL
 * @author ECHOES Technologies (FPO)
 * @date 02/10/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "addon/addonMySQL.h"

int addonMySQLQuery(
                    CollectQueue *collectQueue,
                    MYSQL *mysql,
                    void *params,
                    unsigned short lotNum,
                    IDList *idList,
                    time_t *now
                    )
{
    SearchInfoParams4_1 *searchInfoParams = (SearchInfoParams4_1*)params;

    char **values = NULL;

    /* Objects */
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;

    unsigned int i = 0, n = 1;
    unsigned int nbFields = 0;

    /* Query */
    mysql_query(mysql, searchInfoParams->query);

    /* Storing result */
    result = mysql_use_result(mysql);

    /* Have we a result ? */
    if(result)
    {
        /* Number of fields */
        nbFields = mysql_num_fields(result);

        values = calloc(nbFields, sizeof (char*));

        while ((row = mysql_fetch_row(result)))
        {
            /* Loop to retrieve each field */
            for(i = 0; i < nbFields; i++)
            {
                    values[i] = strdup(row[i] ? row[i] : "");
            }
            if (pushCollectQueue(
                                 collectQueue,
                                 idList,
                                 lotNum,
                                 n,
                                 nbFields,
                                 values,
                                 *now
                                 ))
                return EXIT_FAILURE;

            ++n;
        }

        /* Cleanup */
        free(values);
        mysql_free_result(result);
    }

    return EXIT_SUCCESS;
}

void *addonMySQL(void *arg)
{
    AddonParamsInfo *addonParamsInfo = (AddonParamsInfo*) arg;
    SrcInfoParams4 *srcInfoParams = (SrcInfoParams4*)addonParamsInfo->params;

    time_t now, temp;
    /* MySQL Struct pointer */
    MYSQL mysql;

#ifndef NDEBUG
    printf("Dans le thread addonMyQSL.\n");
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

        /* MySQL Initialization */
        mysql_init(&mysql);
        /* Connection options */
        mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "option");

        if (
            mysql_real_connect(
                               &mysql,
                               srcInfoParams->host,
                               srcInfoParams->user,
                               srcInfoParams->passwd,
                               srcInfoParams->db,
                               srcInfoParams->port,
                               NULL,
                               0
                               )
            )
        {
            AddonTypeInfo *addonTypeInfo = addonParamsInfo->addonTypeList;

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
                    case 1:
                        addonMySQLQuery(
                                        addonParamsInfo->collectQueue,
                                        &mysql,
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
        }
        else
        {
            g_warning("Warning: %s", mysql_error(&mysql));
        }
        
        /* Closing MySQL session */
        mysql_close(&mysql);

        SLEEP(*addonParamsInfo->period);
    }

    pthread_exit(NULL);
}

