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

#ifndef SIGNAL_H
#define	SIGNAL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <glib.h>

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

