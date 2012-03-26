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

// Plugin Informations
typedef struct PlgInfo PlgInfo;
struct PlgInfo
{
    unsigned int idPlg, idAsset;
};

// Source Informations
typedef struct SrcInfo SrcInfo;
struct SrcInfo
{
    PlgInfo *plgInfo;
    unsigned int idSrc, idAddon;
    JSONNODE *params;
};

// File Search Informations
typedef struct FileSearchInfo FileSearchInfo;
struct FileSearchInfo
{
    SrcInfo *srcInfo;
    unsigned int idSearch, idType, staticValues;
    char *period;
    JSONNODE *params;
    FileSearchInfo *nxt;
};

typedef FileSearchInfo* FileSearchList;

// Plugin List
typedef struct PlgList PlgList;
struct PlgList
{
    FileSearchList fileSearchList;
    //SysList sysList;
};

// Max size of file conf lines in caracters
#define MAX_SIZE 300

/**
 * Check if the filename ends with .json
 * @param s[] Filename
 * @return Exit status
 */
int verifExt(char s[]);

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

