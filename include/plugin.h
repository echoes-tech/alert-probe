/* 
 * Header of Plug-in Manager
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#ifndef PLUGIN_H
#define	PLUGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <dirent.h>
#ifndef WIN32
    #include <sys/types.h>
#endif

#include <libjson/libjson.h>

// Plug-in Informations
typedef struct PlgInfo PlgInfo;
struct PlgInfo
{
    char plgPath[255];
    JSONNODE * n;
    PlgInfo *nxt;
};

typedef PlgInfo* PlgList;

// Max size of file conf lines in caracters
#define MAX_SIZE 300

/**
 * Check if the filename ends with .json
 * @param s[] Filename
 * @return Exit status
 */
int verifExt(char s[]);

/**
 * Load probe plugin file
 * @param plgInfo Plugin informations
 * @return Exit status
 */
int loadPlugin(PlgInfo* plgInfo);
/**
 * List probe plugin
 * @param *plgDir Plugins files directory
 * @param *nbPlg  Pointer of Number of plugins
 * @param plgList List of plugins with informations
 * @return Exit status
 */
int listPlugins(const char *plgDir, int *nbPlg, PlgList plgList);
/**
 * Parse JSON
 * @param *n json
 */
void ParseJSON(JSONNODE *n);
/**
 * Main function of Plug-in Manager
 * @param *plgDir Plugins files directory
 * @param plgList List of plugins with informations
 * @return Exit status
 */
int plugin(const char *plgDir, PlgList plgList);

#endif	/* PLUGIN_H */

