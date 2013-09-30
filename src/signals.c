/* 
 * Signal Module
 * @author ECHOES Technologies (MLA)
 * @date 19/09/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "signals.h"

int *signum = NULL;
ThreadIdentifiers *threadIdentifiers = NULL;

void signalsHandler(int *_signum, ThreadIdentifiers *_threadIdentifiers)
{
    unsigned int i = 0;
    for (i = 1; i < SIGWINCH; i++)
    {
        signal(i, signalHandling);
    }
    signal(SIGPWR, signalHandling);
    signal(SIGSYS, signalHandling);
    signum = _signum;
    threadIdentifiers = _threadIdentifiers;
}

void signalHandling(int _signum)
{
    unsigned int i = 0;
    *signum = _signum;
    for (i = 0; i < threadIdentifiers->nbAddonsThreads; i++)
    {
        pthread_cancel(threadIdentifiers->addonsThreads[i]);
    }
}

int waitForShutdown()
{
    pthread_join(threadIdentifiers->addonsMgrThread, NULL);
    pthread_join(threadIdentifiers->formatThread, NULL);
    pthread_join(threadIdentifiers->senderThread, NULL);
    return *signum;
}

int restart(char **argv, char **envp)
{
    int i = 0;
    char *path = realpath(argv[0], NULL);
    /* Try a few times since this may fail because we have an incomplete binary ... */
    for (i = 0; i < 5; ++i)
    {
        int result = execve(path, argv, envp);
        if (result != 0)
            sleep(1);
        else
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}