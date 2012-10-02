/* 
 * Header of Configuration Loader
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef CONF_H
#define	CONF_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "log.h"

/* Max size of file conf lines in caracters */
#define MAX_SIZE 300

/* Probe Configurations */
typedef struct Conf Conf;
struct Conf
{
    unsigned int probeID;
    unsigned short enginePort;
    unsigned char transportProto, transportMsgVersion;
    char probePluginDir[255], engineFQDN[255];
};

/* Probe Configuration Initializer */
#define CONF_INITIALIZER {0, 0, 0, 0, "", ""}

/**
 * Parse line from Configuration file
 * @param *conf Pointer of Configurations
 * @param *line Line of configuration file
 * @return Exit status
 */
int parseLineConf(Conf *conf, const char *line);

/**
 * Load probe configuration file
 * @param *conf     Pointer of Configurations
 * @param *confPath Configuration file path
 * @return Exit status
 */
int loadConf(Conf *conf, const char *confPath);

#endif	/* CONF_H */

