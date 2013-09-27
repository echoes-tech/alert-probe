/* 
 * Header of Plug-in Manager
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
  * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef PLUGIN_H
#define	PLUGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>
#include <regex.h>

#include <dirent.h>
#ifndef WIN32
#include <sys/types.h>
#endif

#include "addonList.h"

/* Default period (second) */
#define DEFAULT 60

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
 */
void addBackslash(char *string);

/**
 * Load probe plugin file to a String
 * @param plgPath Plugins file path
 * @param data     String
 * @return Exit status
 */
int file2data(const char *plgPath, gchar *data);

/**
 * Get an integer value on JsonReader
 * @param parser   JsonParser
 * @param reader   JsonReader
 * @param key      Name of the desired key
 * @param value    Value
 * @param plgPath  Plugins file path
 * @param errorMsg Log Error Message
 * @return Exit status
 */
gushort getIntValue(
                    JsonParser *parser,
                    JsonReader *reader,
                    const char *key,
                    gint *value,
                    const char *formatErrorMsg,
                    ...
                    );

/**
 * Get an unsigned integer value on JsonReader
 * @param parser   JsonParser
 * @param reader   JsonReader
 * @param key      Name of the desired key
 * @param value    Value
 * @param plgPath  Plugins file path
 * @param errorMsg Log Error Message
 * @return Exit status
 */
gushort getUIntValue(
                    JsonParser *parser,
                    JsonReader *reader,
                    const char *key,
                    guint *value,
                    const char *formatErrorMsg,
                    ...
                    );


/**
 * Get an unsigned short value on JsonReader
 * @param parser   JsonParser
 * @param reader   JsonReader
 * @param key      Name of the desired key
 * @param value    Value
 * @param plgPath  Plugins file path
 * @param errorMsg Log Error Message
 * @return Exit status
 */
gushort getUShortValue(
                       JsonParser *parser,
                       JsonReader *reader,
                       const char *key,
                       gushort *value,
                       const char *formatErrorMsg,
                       ...
                       );

/**
 * Get a string value on JsonReader
 * @param parser   JsonParser
 * @param reader   JsonReader
 * @param key      Name of the desired key
 * @param value    Value
 * @param plgPath  Plugins file path
 * @param errorMsg Log Error Message
 * @return Exit status
 */
gushort getStringValue(
                       JsonParser *parser,
                       JsonReader *reader,
                       const char *key,
                       gchar **value,
                       const char *formatErrorMsg,
                       ...
                       );

/**
 * Load data to a linkedlist
 * @param *plgList   Pointer of Plugins list with informations
 * @param *addonList Pointer of Addons list with informations
 * @param *nbThreads Pointer of Number of Searches
 * @param *plgPath Plugins file path
 * @param data     String
 * @return Exit status
 */
int data2llist(
               PlgList *plgList,
               AddonList *addonList,
               unsigned int *nbThreads,
               const char *plgPath,
               const char *data
               );

/**
 * List probe plugin
 * @param *plgDir    Plugins files directory
 * @param *nbPlg     Pointer of Number of plugins
 * @param *plgList   Pointer of Plugins list with informations
 * @param *nbThreads Pointer of Number of Searches
 * @return Exit status
 */
int listPlugins(
                const char *plgDir,
                gushort *nbPlg,
                PlgList *plgList,
                AddonList *addonList,
                unsigned int *nbThreads
                );

/**
 * Main function of Plug-in Manager
 * @param *plgDir    Plugins files directory
 * @param *plgList   Pointer of Plugins list with informations
 * @param *addonList Pointer of Addons list with informations
 * @param *nbThreads Pointer of Number of Searches
 * @return Exit status
 */
gushort plugin(
               const char *plgDir,
               PlgList *plgList,
               AddonList *addonList,
               unsigned int *nbThreads
               );

#endif	/* PLUGIN_H */

