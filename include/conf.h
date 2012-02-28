/* 
 * Header of Configuration Loader
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#ifndef CONF_H
#define	CONF_H

// Probe Configurations
typedef struct Conf Conf;
struct Conf
{
    unsigned int enginePort, probeProto;
    char engineFQDN[255], pluginDir[255];
};

// Max size of file conf lines in caracters
#define MAX_SIZE 300

/**
 * Parse line from Configuration file
 * @param *line Line of configuration file
 * @param conf  Pointer of Configurations
 * @return Exit status
 */
int parseLineConf(const char *line, Conf *conf);
/**
 * Load probe configuration file
 * @param *confDir Configuration file directory
 * @param conf     Pointer of Configurations
 * @return Exit status
 */
int loadConf(const char *confLocation, Conf *conf);

#endif	/* CONF_H */

