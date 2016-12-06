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

#ifndef LOG_H
#define	LOG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <glib.h>

/* Log parameters */
typedef struct LogParams LogParams;
struct LogParams
{
    const gchar *hostname;
    char appName[19];
    pid_t pId;
    char *logFilePath;
};

/**
 * Log Stop Message
 * @param productName Name of Probe
 * @param version     Version of Probe
 */
void logStopProbe(const char* probeName, const char* probeVersion);

/**
 * Logs an error or debugging message in current console
 * @param log_domain The log domain, usually G_LOG_DOMAIN
 * @oaram log_level  The log level, either from GLogLevelFlags or a user-defined level
 * @param message 	 The log message
 * @param user_data  Data passed to the log handler
 */
void log2Console(
                 const gchar *log_domain,
                 GLogLevelFlags log_level,
                 const gchar *message,
                 gpointer user_data
                 );

/**
 * Logs an error or debugging message to log file
 * @param log_domain The log domain, usually G_LOG_DOMAIN
 * @oaram log_level  The log level, either from GLogLevelFlags or a user-defined level
 * @param message 	 The log message
 * @param user_data  Data passed to the log handler
 */
void log2File(
              const gchar *log_domain,
              GLogLevelFlags log_level,
              const gchar *message,
              gpointer user_data
              );
    
#ifdef	__cplusplus
}
#endif

#endif	/* LOG_H */

