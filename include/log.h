/* 
 * Header of Logger
 * @author ECHOES Technologies (FPO)
 * @date 27/08/2012
 */

#ifndef LOG_H
#define	LOG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

