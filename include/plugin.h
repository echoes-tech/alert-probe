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
#include <libjson/libjson.h>
#include <regex.h>

#include <dirent.h>
#ifndef WIN32
#include <sys/types.h>
#endif

#include "addon/addonList.h"

/* Max size of file conf lines in caracters */
#define MAX_SIZE 300

/* Default period (second) */
#define DEFAULT 10

/* Search Informations Parameters for Addon 3 Type 1 */
typedef struct SearchInfoParams2_1 SearchInfoParams3_1;
/* Search Informations Parameters for Addon 2 Type 1 */
typedef struct SearchInfoParams2_1 SearchInfoParams2_1;
struct SearchInfoParams2_1
{
    char regex[MAX_SIZE];
    int err, match;
    regex_t preg;
    size_t nmatch;
    regmatch_t *pmatch;
};

/* Search Informations Parameters for Addon 2 Type 2 */
typedef struct SearchInfoParams2_2 SearchInfoParams2_2;
struct SearchInfoParams2_2
{
    unsigned int line, firstChar, length;
};

/* Search Informations Parameters for Addon 3 Type 2 */
typedef struct SearchInfoParams3_2 SearchInfoParams3_2;
struct SearchInfoParams3_2
{
    unsigned int firstChar, length;
};

/* Search Informations */
typedef struct SearchInfo SearchInfo;
struct SearchInfo
{
    unsigned int idSearch, idType, period, staticValues;
    void *params;
    SearchInfo *nxt;
};
/* Search List */
typedef SearchInfo* SearchList;

/* Source Informations Parameters for Addon 3 */
typedef struct SrcInfoParams3 SrcInfoParams3;
struct SrcInfoParams3
{
    char path[MAX_SIZE];
    unsigned int nbLine;
};

/* Source Informations Parameters for Addon 2 */
typedef struct SrcInfoParams2 SrcInfoParams2;
struct SrcInfoParams2
{
    char path[MAX_SIZE];
};

/* Source Informations */
typedef struct SrcInfo SrcInfo;
struct SrcInfo
{
    unsigned int idSrc, idAddon;
    void *params;
    SearchList searchList;
    SrcInfo *nxt;
};
/* Sources List */
typedef SrcInfo* SrcList;

/* Plugin Informations */
typedef struct PlgInfo PlgInfo;
struct PlgInfo
{
    unsigned int idPlg, idAsset;
    SrcList srcList;
    PlgInfo *nxt;
};
/* Plugins List */
typedef PlgInfo* PlgList;


/**
 * Check if the filename ends with .json
 * @param s Filename
 * @return Exit status
 */
int verifExt(const char *s);

/**
 * Replace \ to \\
 * @param string String
 * @return Exit status
 */
int addBackslash(char *string);

/**
 * Convert the string period from plugin to a number of seconds
 * @param *periodSec   Pointer of period (second)
 * @param periodString Period (string)
 * @return 
 */
int periodString2Int(unsigned int *periodSec, const char *periodString);

/**
 * Load probe plugin file to a jsonnode
 * @param *plgPath Plugins file path
 * @param *json    Json string
 * @return Exit status
 */
int file2json(const char *plgPath, json_char *json);
/**
 * Load jsonnode to a linkedlist
 * @param *n         Json node
 * @param *plgList   Pointer of Plugins list with informations
 * @param *nbThreads Pointer of Number of Searches
 * @return Exit status
 */
int json2llist(JSONNODE *n, PlgList *plgList, AddonList *addonList, unsigned int *nbThreads, CollectQueue *collectQueue);
/**
 * List probe plugin
 * @param *plgDir    Plugins files directory
 * @param *nbPlg     Pointer of Number of plugins
 * @param *plgList   Pointer of Plugins list with informations
 * @param *nbThreads Pointer of Number of Searches
 * @return Exit status
 */
int listPlugins(const char *plgDir, int *nbPlg, PlgList *plgList, AddonList *addonList, unsigned int *nbThreads, CollectQueue *collectQueue);
/**
 * Main function of Plug-in Manager
 * @param *plgDir    Plugins files directory
 * @param *plgList   Pointer of Plugins list with informations
 * @param *addonList Pointer of Addons list with informations
 * @param *nbThreads Pointer of Number of Searches
 * @return Exit status
 */
int plugin(const char *plgDir, PlgList *plgList, AddonList *addonList, unsigned int *nbThreads, CollectQueue *collectQueue);

#endif	/* PLUGIN_H */

