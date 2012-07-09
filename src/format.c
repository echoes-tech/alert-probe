/* 
 * Format Module
 * @author ECHOES Technologies (FPO)
 * @date 24/04/2012
 */

#include "format.h"

int popCollectQueue(CollectQueue *collectQueue)
{
    if (collectQueue == NULL)
    {
        exit(EXIT_FAILURE);
    }

    /* Debut de la zone protegee. */
    pthread_mutex_lock (& collectQueue->mutex);
    
    /* On vérifie s'il y a quelque chose à défiler */
    if (collectQueue->first != NULL)
    {
        CollectQueueElement *popedElement = collectQueue->first;

        printf("time: %f, value: %s, ids: %s.\n", (double) popedElement->time, popedElement->value, popedElement->identifier);
        collectQueue->first = popedElement->next;
        free(popedElement);
    }

    /* Fin de la zone protegee. */
    pthread_mutex_unlock (& collectQueue->mutex);

    return (EXIT_SUCCESS);
}

void *format(void *arg)
{
    FormatParams *formatParams = (FormatParams*) arg;
    
    while (1)
    {
        while (formatParams->collectQueue->first != NULL)
        {
            popCollectQueue(formatParams->collectQueue);
        }
        SLEEP(1);
    }
    
    pthread_exit(NULL);
}

