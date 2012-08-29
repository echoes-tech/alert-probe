/* 
 * Logger
 * @author ECHOES Technologies (FPO)
 * @date 27/08/2012
 */

#include "log.h"

void log2Console(
                 const gchar *log_domain,
                 GLogLevelFlags log_level,
                 const gchar *message,
                 gpointer user_data
                 )
{
    LogParams *logParams = user_data;
    
    g_print(
            "%s %s %s %d - %s\n",
            g_date_time_format(g_date_time_new_now_local(), "%FT%T%z"),
            logParams->hostname,
            logParams->appName,
            logParams->pId,
            message
            );

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

    FILE *logFile = fopen(logParams->logFilePath, "a");
    if (logFile == NULL)
    {
        log2Console(
                    log_domain,
                    log_level,
                    g_strdup_printf("%s: %s", strerror(errno), logParams->logFilePath),
                    user_data
                    );
        /*  Fall  back  to  console  output  if  unable  to  open  file  */
        log2Console(log_domain, log_level, message, user_data);
        return;
    }

    fprintf(
            logFile,
            "%s %s %s %d - %s\n",
            g_date_time_format(g_date_time_new_now_local(), "%FT%T%z"),
            logParams->hostname,
            logParams->appName,
            logParams->pId,
            message
            );
    
    fclose(logFile);

    return;
} 

