/* 
 * Format Module
 * @author ECHOES Technologies (FPO)
 * @date 24/04/2012
 */

#include "format.h"
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

/* Source : http://www.ioncannon.net/programming/34/howto-base64-encode-with-cc-and-openssl/ */

char *base64(const char *input, int length)
{
    BIO *bmem, *b64;
    BUF_MEM *bptr;
    char *buff;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    if (BIO_flush(b64) != 1)
    {
        perror("BIO_flush()");
        exit (EXIT_FAILURE);
    }
    BIO_get_mem_ptr(b64, &bptr);

    buff = (char *) malloc(bptr->length);
    memcpy(buff, bptr->data, bptr->length - 1);
    buff[bptr->length - 1] = 0;

    BIO_free_all(b64);

    return buff;
}

int pushSDElementQueue(SDElementQueue *sdElementQueue, unsigned int idPlg, unsigned int idAsset, unsigned int idSrc, unsigned int idSearch, unsigned int valueNum, const char *b64Value, time_t time)
{
    SDElementQueueElement *new = calloc(1, sizeof(SDElementQueueElement));
    if (sdElementQueue == NULL || new == NULL)
    {
        exit(EXIT_FAILURE);
    }

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
            "[prop@40311 ver=%d probe=%d][res1@40311 offset=",
            *sdElementQueue->transportMsgVersion,
            *sdElementQueue->probeID
        );

    sprintf(
            new->afterOffset,
            "%d-%d-%d-%d-%d=\"%s\"]",
            idPlg,
            idAsset,
            idSrc,
            idSearch,
            valueNum,
            b64Value
        );
    
    /* Debut de la zone protegee. */
    pthread_mutex_lock (& sdElementQueue->mutex);
    
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
    pthread_mutex_unlock (& sdElementQueue->mutex);

    return (EXIT_SUCCESS);
}

int popCollectQueue(CollectQueue *collectQueue, SDElementQueue *sdElementQueue)
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

        pushSDElementQueue(
                           sdElementQueue,
                           popedElement->idPlg,
                           popedElement->idAsset,
                           popedElement->idSrc,
                           popedElement->idSearch,
                           popedElement->valueNum,
                           /*TODO: Tester le retour de base64 */
                           base64(popedElement->value, strlen(popedElement->value)),
                           popedElement->time
                           );
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
            popCollectQueue(formatParams->collectQueue, formatParams->sdElementQueue);
        }
        SLEEP(1);
    }
    
    pthread_exit(NULL);
}

