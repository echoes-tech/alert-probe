/* 
 * Alert - Probe is a collect probe, part of the Alert software
 * Copyright (C) 2013 Florent Poinsaut, Thomas Saquet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#include "signals.h"

int *signum = NULL;
ThreadIdentifiers *threadIdentifiers = NULL;

void signalsHandler(int *_signum, ThreadIdentifiers *_threadIdentifiers)
{
    int i = 0, j = 0;
    int ignored [] = {SIGSTOP, SIGKILL, SIGCHLD, SIGCLD, SIGIO, SIGPOLL, SIGSTOP, SIGTSTP, SIGCONT, SIGTTIN, SIGTTOU, SIGURG, SIGWINCH};
    int ignored_length = 13;
    for (i = 1; i < SIGSYS; i++)
    {
        gboolean cont = FALSE;
        for (j = 0; j < ignored_length; j ++)
            if (i == ignored[j])
                cont = TRUE;    
        if(cont)
            continue;
        signal(i, signalHandling);
    }
    signum = _signum;
    threadIdentifiers = _threadIdentifiers;
}

void signalHandling(int _signum)
{
    unsigned int i = 0;
    if (_signum == SIGPIPE)
    {
        g_error("Error: Signal: %d", _signum);
        return;
    }
    if (_signum == SIGTERM || _signum == SIGHUP || _signum == SIGALRM || _signum == SIGVTALRM || _signum == SIGPROF)
        g_message("Signal: %d", _signum);    
    else
        g_critical("Critical: Signal: %d", _signum);
    if (*signum == 0)
    {
        *signum = _signum;
        for (i = 0; i < threadIdentifiers->nbAddonsThreads; i++)
        {
            pthread_cancel(threadIdentifiers->addonsThreads[i]);
        }
    }
}

int waitForShutdown()
{
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
