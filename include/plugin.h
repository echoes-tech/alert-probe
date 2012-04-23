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

// Search Informations
typedef struct SearchInfo SearchInfo;
struct SearchInfo
{
    unsigned int idSearch, idType, period, staticValues;
    JSONNODE *params;
    SearchInfo *nxt;
};
// Search List
typedef SearchInfo* SearchList;

// Source Informations
typedef struct SrcInfo SrcInfo;
struct SrcInfo
{
    unsigned int idSrc, idAddon;
    JSONNODE *params;
    SearchList searchList;
    SrcInfo *nxt;
};
// Sources List
typedef SrcInfo* SrcList;

// Plugin Informations
typedef struct PlgInfo PlgInfo;
struct PlgInfo
{
    unsigned int idPlg, idAsset;
    SrcList srcList;
    PlgInfo *nxt;
};
// Plugins List
typedef PlgInfo* PlgList;

// Max size of file conf lines in caracters
#define MAX_SIZE 300

// Default period (second)
#define DEFAULT 10

/**
 * Check if the filename ends with .json
 * @param s[] Filename
 * @return Exit status
 */
int verifExt(char s[]);

/**
 * Replace \ to \\
 * @param string String
 * @return Exit status
 */
int addBackslash(char *string);

/**
 * Convert the string period from plugin to a number of seconds
 * @param *periodSec Pointer of period (second)
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
 * @param *n       Json node
 * @param *plgList Pointer of Plugins list with informations
 * @return Exit status
 */
int json2llist(JSONNODE *n, PlgList *plgList);
/**
 * List probe plugin
 * @param *plgDir Plugins files directory
 * @param *nbPlg  Pointer of Number of plugins
 * @param *plgList Pointer of Plugins list with informations
 * @return Exit status
 */
int listPlugins(const char *plgDir, int *nbPlg, PlgList *plgList);
/**
 * Main function of Plug-in Manager
 * @param *plgDir  Plugins files directory
 * @param *plgList Pointer of Plugins list with informations
 * @return Exit status
 */
int plugin(const char *plgDir, PlgList *plgList);

#endif	/* PLUGIN_H */

