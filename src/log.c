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

#include "log.h"

void logStopProbe(const char* probeName, const char* probeVersion)
{
    g_critical(
          "[origin enterpriseId=\"40311\" software=\"%s\" swVersion=\"%s\"] stop",
          probeName,
          probeVersion
          );
}

void log2Console(
                 const gchar *log_domain,
                 GLogLevelFlags log_level,
                 const gchar *message,
                 gpointer user_data
                 )
{
    LogParams *logParams = user_data;
    GDateTime *dateTime = g_date_time_new_now_local();
    gchar *dateTimeFormat = g_date_time_format(dateTime, "%FT%T%z");
    
    g_print(
            "%s %s %s %d - %s\n",
            dateTimeFormat,
            logParams->hostname,
            logParams->appName,
            logParams->pId,
            message
            );

    g_date_time_unref(dateTime);
    dateTime = NULL;
    free(dateTimeFormat);
    dateTimeFormat = NULL;

    return;
}

void log2File(
               const gchar *log_domain,
               GLogLevelFlags log_level,
               const gchar *message,
               gpointer user_data
               )
{
    LogParams *logParams = user_data;
    GDateTime *dateTime = NULL;
    gchar *dateTimeFormat = NULL, *errorMsg = NULL;

    FILE *logFile = fopen(logParams->logFilePath, "a");
    if (logFile == NULL)
    {
        errorMsg = g_strdup_printf(
                                   "Error opening file %s: %s",
                                   logParams->logFilePath,
                                   strerror(errno)
                                   );
        log2Console(log_domain, log_level, errorMsg, user_data);
        free(errorMsg);
        errorMsg = NULL;

        /*  Fall  back  to  console  output  if  unable  to  open  file  */
        log2Console(log_domain, log_level, message, user_data);
        return;
    }

    dateTime = g_date_time_new_now_local();
    dateTimeFormat = g_date_time_format(dateTime, "%FT%T%z");

    fprintf(
            logFile,
            "%s %s %s %d - %s\n",
            dateTimeFormat,
            logParams->hostname,
            logParams->appName,
            logParams->pId,
            message
            );

    g_date_time_unref(dateTime);
    dateTime = NULL;
    free(dateTimeFormat);
    dateTimeFormat = NULL;

    fclose(logFile);

    return;
} 

