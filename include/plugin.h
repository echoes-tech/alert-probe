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

#include "addon/addonList.h"

/* Max size of file conf lines in caracters */
#define MAX_SIZE 300

/* Default period (second) */
#define DEFAULT 60

/* Search Informations Parameters for Addon Log Type Regex */
typedef struct SearchInfoParams2_1 SearchInfoParams3_1;
/* Search Informations Parameters for Addon File Type Regex */
typedef struct SearchInfoParams2_1 SearchInfoParams2_1;
struct SearchInfoParams2_1
{
    gchar *regex;
    int err;
    regex_t preg;
    size_t nmatch;
    regmatch_t *pmatch;
};

/* Search Informations Parameters for Addon File Type Localisation */
typedef struct SearchInfoParams2_2 SearchInfoParams2_2;
struct SearchInfoParams2_2
{
    unsigned int line, firstChar, length;
};

/* Search Informations Parameters for Addon Log Type Localisation */
typedef struct SearchInfoParams3_2 SearchInfoParams3_2;
struct SearchInfoParams3_2
{
    unsigned int firstChar, length;
};

/* Search Informations Parameters for Addon MySQL Type Query */
typedef struct SearchInfoParams4_3 SearchInfoParams4_3;
struct SearchInfoParams4_3
{
    gchar *query;
};

/* Search Informations Parameters for Addon SNMP Type Regex */
typedef struct SearchInfoParams5_4 SearchInfoParams5_4;
struct SearchInfoParams5_4
{
    gchar *oid, *regex;
    int err;
    regex_t preg;
    size_t nmatch;
    regmatch_t *pmatch;
};

/* Search Informations Parameters for Addon SNMP Type All */
typedef struct SearchInfoParams5_5 SearchInfoParams5_5;
struct SearchInfoParams5_5
{
    gchar *oid;
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

/* Source Informations Parameters for Addon File */
typedef struct SrcInfoParams2 SrcInfoParams2;
struct SrcInfoParams2
{
    gchar *path;
};

/* Source Informations Parameters for Addon Log */
typedef struct SrcInfoParams3 SrcInfoParams3;
struct SrcInfoParams3
{
    gchar *path;
    unsigned int nbLine, lastNLines;
};

/* Source Informations Parameters for Addon MySQL */
typedef struct SrcInfoParams4 SrcInfoParams4;
struct SrcInfoParams4
{
    gchar *host, *user, *passwd, *db;
    unsigned short port;
};

/* Source Informations Parameters for Addon SNMP */
typedef struct SrcInfoParams5 SrcInfoParams5;
struct SrcInfoParams5
{
    gchar *host, *version, *community, *user, *authProto, *authPass, *privProto, *privPass;
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

