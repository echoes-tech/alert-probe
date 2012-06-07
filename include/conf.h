/* 
 * Header of Configuration Loader
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#ifndef CONF_H
#define	CONF_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Max size of file conf lines in caracters
#define MAX_SIZE 300

// Probe Configurations
typedef struct Conf Conf;
struct Conf
{
    unsigned int enginePort, probeID, transportProto, transportMsgVersion;
    char engineFQDN[255], probePluginDir[255];
};

/**
 * Parse line from Configuration file
 * @param *line Line of configuration file
 * @param *conf  Pointer of Configurations
 * @return Exit status
 */
int parseLineConf(const char *line, Conf *conf);
/**
 * Load probe configuration file
 * @param *confPath Configuration file path
 * @param *conf     Pointer of Configurations
 * @return Exit status
 */
int loadConf(const char *confPath, Conf *conf);

#endif	/* CONF_H */

