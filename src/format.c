/* 
 * Format Module
 * @author ECHOES Technologies (FPO)
 * @date 24/04/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "format.h"

int pushSDElementQueue(
                       SDElementQueue *sdElementQueue,
                       char *afterOffset,
                       time_t time
                       )
{
    SDElementQueueElement *new = calloc(1, sizeof (SDElementQueueElement));
    if (sdElementQueue == NULL || new == NULL)
        g_error("Error: Queue of SD-Element unavailable");

    new->time = time;

    sprintf(
            new->beforeMsgID,
            "%s %s %d ID",
            sdElementQueue->hostname,
            sdElementQueue->appName,
            sdElementQueue->pID
            );

    sprintf(
            new->afterMsgID,
            "[prop@40311 ver=%d probe=%d token=\"%s\"][res1@40311 offset=",
            *sdElementQueue->transportMsgVersion,
            *sdElementQueue->probeID,
            sdElementQueue->token
            );

    new->afterOffset = strdup(afterOffset);

    /* Debut de la zone protegee. */
    pthread_mutex_lock(& sdElementQueue->mutex);

    if (sdElementQueue->first != NULL) /* La file n'est pas vide */
    {
        /* On se positionne à la fin de la file */
        SDElementQueueElement *lastElement = sdElementQueue->first;
        while (lastElement->next != NULL)
        {
            lastElement = lastElement->next;
        }
        lastElement->next = new;
    }
    else /* La file est vide, notre élément est le premier */
    {
        sdElementQueue->first = new;
    }

    /* Fin de la zone protegee. */
    pthread_mutex_unlock(& sdElementQueue->mutex);

    return EXIT_SUCCESS;
}

int popCollectQueue(CollectQueue *collectQueue, SDElementQueue *sdElementQueue)
{
    char afterOffset[10000] = "", afterOffsetTmp[10000] = "";
    unsigned short i = 0;

    if (collectQueue == NULL)
        g_error("Error: Queue of collected data unavailable");

    /* Debut de la zone protegee. */
    pthread_mutex_lock(& collectQueue->mutex);

    /* On vérifie s'il y a quelque chose à défiler */
    if (collectQueue->first != NULL)
    {
        CollectQueueElement *popedElement = collectQueue->first;

        sprintf(
                afterOffset,
                " lotNum=%d lineNum=%d",
                popedElement->lotNum,
                popedElement->lineNum
                );

        for (i = 0; i < popedElement->valuesLength; i++)
        {
            gchar *url_encoded = g_uri_escape_string(
                                                     popedElement->values[i],
                                                     "",
                                                     TRUE
                                                     );


            sprintf(
                    afterOffsetTmp,
                    " %d=\"%s\"",
                    popedElement->idsIDA[i],
                    url_encoded
                    );

            g_free(url_encoded);
            url_encoded = NULL;

            strcat(afterOffset, afterOffsetTmp);

            free(popedElement->values[i]);
            popedElement->values[i] = NULL;
        }
        free(popedElement->values);
        popedElement->values = NULL;

        pushSDElementQueue(
                           sdElementQueue,
                           afterOffset,
                           popedElement->time
                           );
        collectQueue->first = popedElement->next;
        free(popedElement);
    }

    /* Fin de la zone protegee. */
    pthread_mutex_unlock(& collectQueue->mutex);

    return EXIT_SUCCESS;
}

void *format(void *arg)
{
    FormatParams *formatParams = (FormatParams*) arg;

    while (*formatParams->signum == 0)
    {
        while (formatParams->collectQueue->first != NULL)
        {
            popCollectQueue(formatParams->collectQueue, formatParams->sdElementQueue);
        }
        SLEEP(1);
    }

#ifndef NDEBUG
    printf("Fin du thread de formatage\n");
#endif

    pthread_exit(NULL);
}

