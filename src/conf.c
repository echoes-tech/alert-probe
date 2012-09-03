/* 
 * Configuration Loader
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "conf.h"

int parseLineConf(Conf *conf, const char* line)
{
    char *equal = NULL, *value = NULL;

    /* Search = character */
    equal = strchr(line, '=');
    if (equal != NULL)
    {
        /* Value is String after equal, end of line */
        value = equal + 1;
        /* Replace = character by a end of String */
        *equal = '\0';
        /* Key is begin of line */

        if (!strcmp(line, "probe_id"))
        {
            /*TODO: test format */
            conf->probeID = atoi(value);
        }
        else if (!strcmp(line, "probe_plugin_dir"))
        {
            /*TODO: test format */
#ifdef NDEBUG
            strncpy(conf->probePluginDir, value, (strlen(value) - 1));
#else
            strcpy(conf->probePluginDir, "./plugins/");
#endif
        }
        else if (!strcmp(line, "transport_proto"))
        {
            /*TODO: test format */
            conf->transportProto = atoi(value);
        }
        else if (!strcmp(line, "transport_message_version"))
        {
            /*TODO: test format */
            conf->transportMsgVersion = atoi(value);
        }
        else if (!strcmp(line, "engine_fqdn"))
        {
            /*TODO: test format */
            strncpy(conf->engineFQDN, value, (strlen(value) - 1));
        }
        else if (!strcmp(line, "engine_port"))
        {
            /*TODO: test format */
            conf->enginePort = atoi(value);
        }
        else
        {
            printf("Bad Line!");
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

int loadConf(Conf *conf,const char *confPath)
{
    FILE* confFile = NULL;
    char line[MAX_SIZE] = "";

    /* Opening file */
    confFile = fopen(confPath, "r");

    if (confFile != NULL)
    {
        /* Reading file line by line */
        while (fgets(line, MAX_SIZE, confFile) != NULL)
        {
            /* Ignore commented lines */
            if (line[0] == '#') continue;
            parseLineConf(conf, line);
        }

        /* Closing conf file */
        fclose(confFile);
    }
    else
    {
        g_message("%s: %s", strerror(errno), confPath);
        return (errno);
    }

    return (EXIT_SUCCESS);
}

