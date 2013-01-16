/* 
 * Logger
 * @author ECHOES Technologies (FPO)
 * @date 27/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
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

