/* 
 * Header of Signal Module
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

#ifndef SIGNAL_H
#define	SIGNAL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "addonLib.h"

/**
 * Define signals handlers
 * @param _signum               Pointer of signal identifier from main
 * @param _threadIdentifiers    Pointer of struct thread identifiers
 */
void signalsHandler(int *_signum, ThreadIdentifiers *_threadIdentifiers);

/**
 * Handler of specified signal
 * @param _signum        Signal identifier
 */
void signalHandling(int _signum);

/**
 * Wait end of threads to shutdown
 * @return              Signal identifier
 */
int waitForShutdown();

/**
 * Restart probe
 * @param argv          Array of arguments passed to the program
 * @param envp          Array of environment variables passed to the program
 * @return              Exit status
 */
int restart(char **argv, char **envp);


#endif	/* SIGNAL_H */

