/* 
 * Header of Plug-in Manager
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#ifndef PLUGIN_H
#define	PLUGIN_H

// Plug-in Informations
typedef struct PluginInfo PluginInfo;
struct PluginInfo
{
    unsigned int id, idAsset;
    int sourceId[50];
};

// Plug-in File - Source Informations
typedef struct PlgFileSrcInfo PlgFileSrcInfo;
struct PlgFileSrcInfo
{
    unsigned int id, tail;
    char path[255];
    unsigned int searchId[100], searchIdType[100];
};

// Plug-in File - Regex - Parameters
typedef struct PlgFileRegParam PlgFileRegParam;
struct PlgFileRexParam
{
    unsigned int id;
    char regex[1000];
};
// Plug-in File - Localisation - Parameters
typedef struct PlgFileLocParam PlgFileLocParam;
struct PlgFileLocParam
{
    unsigned int id, line, firstChar, length;
};

// Add-on Parameters
typedef struct AddonParam AddonParam;
struct AddonParams
{
    unsigned int id;
    PluginInfo PluginInfo[100];
};

/**
 * Main function of Plug-in Manager
 * @param *confDir     Configuration file directory
 * @param *addonParams Pointer of Table of params to Add-ons
 * @return Exit status
 */
int plugin(const char *confDir, AddonParam *addonParam);

#endif	/* PLUGIN_H */

