/* 
 * Configuration Loader
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "CUnit/Basic.h"

#include "conf.h"

int parseLineConf(const char* line, Conf *conf)
{
    char *equal, *key, *value;

    /* Search = character */
    equal = strstr(line, "=");
    if (equal != NULL)
    {
        /* Value is String after equal, end of line */
        value = equal + 1;
        /* Replace = character by a end of String */
        *equal = '\0';
        /* Key is begin of line */
        line;

        if (strcmp(line, "engine_fqdn"))
        {
            //TODO: test format
            conf->engineFQDN = *line;
        }
        else if (strcmp(line, "engine_port")) 
        {
            //TODO: test format
            conf->enginePort = *line;
        }
        else if (strcmp(line, "probe_proto"))
        {
            //TODO: test format
            conf->probeProto = *line;
        }
        else if (strcmp(line, "plugin_dir"))
        {
            //TODO: test format
            conf->pluginDir = *line;
        }
        else
        {
            printf("Bad Line!");
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

int loadConf(const char *confLocation, Conf *conf)
{
    FILE* confFile = NULL;
    char line[MAX_SIZE] = "";

    /* Opening file */
    confFile = fopen(confLocation, "r");

    if (confFile != NULL)
    {
        /* Reading file line by line */
        while (fgets(line, MAX_SIZE, confFile) != NULL)
        {
            /* Ignore commented lines */
            if (line[0]=='#') continue ;
            parseLineConf(line, conf);
        }

        /* Closing conf file */
        fclose(confFile);
    }
    else
    {
        perror("fopen()");
        return (errno);
    }
    
    return (EXIT_SUCCESS);
}