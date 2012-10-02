/* 
 * Plug-in Manager
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "plugin.h"

gboolean verifExt(const char *s)
{
    int i = strlen(s);

    if ((s[i - 5] == '.') && (s[i - 4] == 'j') && (s[i - 3] == 's') && (s[i - 2] == 'o') && (s[i - 1] == 'n'))
    {
        return TRUE;
    }
    return FALSE;
}

int addBackslash(char *string)
{
    char *backslash = NULL, *rest = NULL, tmp[25500] = "";

    /* Search \ character */
    backslash = strchr(string, '\\');

    if (backslash != NULL)
    {
        /* Value is String after equal, end of line */
        rest = backslash + 1;

        /* Replace = character by a end of String */
        *backslash = '\0';

        strcpy(tmp, string);
        strcat(tmp, "\\\\");

        addBackslash(rest);

        strcat(tmp, rest);
        strcpy(string, tmp);
    }

    return (EXIT_SUCCESS);
}

int periodString2Int(unsigned int *periodSec, const char *periodString)
{
     /* default or [1-60]s or [1-60]m or [1-24]h or [1-365]d */
    
    int i = strlen(periodString);
    
    if(periodString[i - 1] == 's')
    {
       /*periodString[i - 1] = '\0'; */
       *periodSec = atoi(periodString);
    }
    else if(periodString[i - 1] == 'm')
    {
       /*periodString[i - 1] = '\0'; */
       *periodSec = 60 * atoi(periodString);
    }
    else if(periodString[i - 1] == 'h')
    {
       /*periodString[i - 1] = '\0'; */
       *periodSec = 60 * 60 * atoi(periodString);
    }
    else if(periodString[i - 1] == 'd')
    {
       /*periodString[i - 1] = '\0'; */
       *periodSec = 24 * 60 * 60 * atoi(periodString);
    }
    
    if (*periodSec == 0)
    {
        *periodSec = DEFAULT;
    }

    return (EXIT_SUCCESS);
}

int listPlugins(
                const char *plgDir,
                int *nbPlg,
                PlgList *plgList,
                AddonList *addonList,
                unsigned int *nbThreads
                )
{
    DIR* dir = NULL;
    struct dirent* read = NULL;

    dir = opendir(plgDir);
    if (dir == NULL)
        return (EXIT_FAILURE);

    /* Reading filename file by file */
    while ((read = readdir(dir)))
    {
        if (verifExt(read->d_name))
        {
            char plgPath[255] = "";
            /*TODO: define max line by plugin */
            gchar data[MAX_SIZE * 1000] = "";

            /* Put repository for plugin path */
            strcpy(plgPath, plgDir);
            /* Add filename for plugin path */
            strcat(plgPath, read->d_name);

            g_message("Loading plugin %s", plgPath);

            if (file2data(plgPath, data))
            {
                perror("file2datat()");
                return (errno);                
            }

            if (data2llist(plgList, addonList, nbThreads, plgPath, data))
            /*if (file2llist(plgList, addonList, nbThreads, collectQueue, plgPath))*/
            {
                perror("data2llist()");
                /*perror("file2llist()");*/
                return (errno);
            }

            /* Count number of plugin */
            (*nbPlg)++;
        }
    }

    if (closedir(dir))
    {
        perror("closedir()");
        return (errno);
    }
    return EXIT_SUCCESS;
}


int file2data(const char *plgPath, gchar* data)
{
    FILE* plgFile = NULL;
    char line[MAX_SIZE] = "";

    /* Opening file */
    plgFile = fopen(plgPath, "r");

    if (plgFile != NULL)
    {

        /* Reading file line by lfileine */
        while (fgets(line, MAX_SIZE, plgFile) != NULL)
        {
            addBackslash(line);
            strcat(data, line);
        }

        /* Closing plugin file */
        fclose(plgFile);
    }
    else
    {
        perror("fopen()");
        return (errno);
    }

    return (EXIT_SUCCESS);
}

int data2llist(
               PlgList *plgList,
               AddonList *addonList,
               unsigned int *nbThreads,
               const char *plgPath,
               const char *data
               )
/*int file2llist(PlgList *plgList, AddonList *addonList, unsigned int *nbThreads, CollectQueue *collectQueue, const char *plgPath)*/
{
    JsonParser *parser = NULL;
    JsonNode *root = NULL;
    JsonReader *reader = NULL;
    GError *error = NULL;

    PlgInfo* plgInfo = calloc(1, sizeof (PlgInfo));
    
    guint i = 0, j = 0;

    g_type_init();

    parser = json_parser_new();
    if (!JSON_IS_PARSER(parser))
    {
        g_print("Unable to create a parser for '%s'\n", plgPath);
        return EXIT_FAILURE;
    }

    json_parser_load_from_data (parser, data, -1, &error);
    if (error)
    {
        g_print("Unable to parse '%s': %s\n", plgPath, error->message);
        g_error_free(error);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }
    /*json_parser_load_from_file(parser, plgPath, &error);
    if (error)
    {
        g_print("Unable to parse '%s': %s\n", plgPath, error->message);
        g_error_free(error);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }*/

    root = json_parser_get_root(parser);
    if (NULL == root)
    {
        g_print("Unable to retrieves the top level node of file '%s'.\n", plgPath);
        return EXIT_FAILURE;
    }
    
    reader = json_reader_new(root);
    if (NULL == reader)
    {
        g_print("Unable to create new JsonReader for file '%s'.\n", plgPath);
        return EXIT_FAILURE;
    }

    /* Find out where to store the values */
    if (json_reader_read_member (reader, "id"))
    {
        plgInfo->idPlg = json_reader_get_int_value (reader);
    }
    else
    {
        printf("Invalid Plugin '%s': Invalid Plugin ID\n", plgPath);
        return (EXIT_FAILURE);
    }
    json_reader_end_member (reader);

    if (json_reader_read_member (reader, "idAsset"))
    {
        plgInfo->idAsset = json_reader_get_int_value (reader);
    }
    else
    {
        printf("Invalid Plugin '%s': Invalid idAsset\n", plgPath);
        return (EXIT_FAILURE);
    }
    json_reader_end_member (reader);

    if (json_reader_read_member (reader, "sources"))
    {
        if ( json_reader_is_array(reader))
        {
            SrcList srcList = NULL;
            for (i = 0; i < json_reader_count_elements(reader); ++i)
            {
                if (json_reader_read_element (reader, i))
                {
                    SrcInfo* srcInfo = calloc(1, sizeof (SrcInfo));

                    if (json_reader_read_member (reader, "id"))
                    {
                        srcInfo->idSrc = json_reader_get_int_value (reader);
                    }
                    else
                    {
                        printf("Invalid Plugin '%s': Invalid Source ID for Source n°%d\n", plgPath, i + 1);
                        return (EXIT_FAILURE);
                    }
                    json_reader_end_member (reader);

                    if (json_reader_read_member (reader, "idAddon"))
                    {
                        srcInfo->idAddon = json_reader_get_int_value (reader);
                    }
                    else
                    {
                        printf("Invalid Plugin '%s': Invalid idAddon of Source ID '%d'\n", plgPath, srcInfo->idSrc);
                        return (EXIT_FAILURE);
                    }
                    json_reader_end_member (reader);

                    if (json_reader_read_member (reader, "params"))
                    {
                        switch (srcInfo->idAddon)
                        {
                            case 2:
                            {
                                SrcInfoParams2 *srcInfoParams = calloc(1, sizeof (SrcInfoParams2));
                                if (json_reader_read_member (reader, "path"))
                                {
                                    srcInfoParams->path = g_strdup(json_reader_get_string_value(reader));
                                }
                                else
                                {
                                    printf("Invalid Plugin '%s': Invalid path of Source ID '%d'\n", plgPath, srcInfo->idSrc);
                                    return (EXIT_FAILURE);
                                }
                                json_reader_end_member (reader);

                                srcInfo->params = (void*)srcInfoParams;
                                break;
                            }
                            case 3:
                            {
                                SrcInfoParams3 *srcInfoParams = calloc(1, sizeof (SrcInfoParams3));
                                srcInfoParams->nbLine = 0;
                                if (json_reader_read_member (reader, "path"))
                                {
                                    srcInfoParams->path = g_strdup(json_reader_get_string_value(reader));
                                }
                                else
                                {
                                    printf("Invalid Plugin '%s': Invalid path of Source ID '%d'\n", plgPath, srcInfo->idSrc);
                                    return (EXIT_FAILURE);
                                }
                                json_reader_end_member (reader);

                                if (json_reader_read_member (reader, "lastNLines"))
                                {
                                    srcInfoParams->lastNLines = json_reader_get_int_value(reader);
                                }
                                else
                                {
                                    printf("Invalid Plugin '%s': Invalid lastNLines of Source ID '%d'\n", plgPath, srcInfo->idSrc);
                                    return (EXIT_FAILURE);
                                }
                                json_reader_end_member (reader);

                                srcInfo->params = (void*)srcInfoParams;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                    else
                    {
                        printf("Invalid Plugin '%s': Invalid params of Source ID '%d'\n", plgPath, srcInfo->idSrc);
                        return (EXIT_FAILURE);
                    }
                    json_reader_end_member (reader);

                    if (json_reader_read_member (reader, "searches"))
                    {
                        if (json_reader_is_array(reader))
                        {
                            SearchList searchList = NULL;
                            for (j = 0; j < json_reader_count_elements(reader); ++j)
                            {
                                if (json_reader_read_element(reader, j))
                                {
                                    SearchInfo* searchInfo = calloc(1, sizeof (SearchInfo));
                                    ++(*nbThreads);

                                    if (json_reader_read_member (reader, "id"))
                                    {
                                        searchInfo->idSearch = json_reader_get_int_value (reader);
                                    }
                                    else
                                    {
                                        printf("Invalid Plugin '%s': Invalid Search ID for Search n°%d of Source ID '%d'\n", plgPath, j + 1, srcInfo->idSrc);
                                        return (EXIT_FAILURE);
                                    }
                                    json_reader_end_member (reader);

                                    if (json_reader_read_member (reader, "idType"))
                                    {
                                        searchInfo->idType = json_reader_get_int_value (reader);
                                    }
                                    else
                                    {
                                        printf("Invalid Plugin '%s': Invalid idType of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                        return (EXIT_FAILURE);
                                    }
                                    json_reader_end_member (reader);

                                    if (json_reader_read_member (reader, "period"))
                                    {
                                        periodString2Int(&searchInfo->period, json_reader_get_string_value (reader));
                                    }
                                    else
                                    {
                                        printf("Invalid Plugin '%s': Invalid period of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                        return (EXIT_FAILURE);
                                    }
                                    json_reader_end_member (reader);

                                    if (json_reader_read_member (reader, "staticValues"))
                                    {
                                        searchInfo->staticValues = json_reader_get_boolean_value (reader);
                                    }
                                    else
                                    {
                                        printf("Invalid Plugin '%s': Invalid staticValues of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                        return (EXIT_FAILURE);
                                    }
                                    json_reader_end_member (reader);

                                    if (json_reader_read_member (reader, "params"))
                                    {
                                        switch (srcInfo->idAddon)
                                        {
                                        case 2:
                                            switch (searchInfo->idType)
                                            {
                                            case 1:
                                            {
                                                SearchInfoParams2_1 *searchInfoParams = calloc(1, sizeof (SearchInfoParams2_1));
                                                if (json_reader_read_member(reader, "regex"))
                                                {
                                                    searchInfoParams->regex = g_strdup(json_reader_get_string_value(reader));

                                                    /* Regex compilation */
                                                    searchInfoParams->err = regcomp(&searchInfoParams->preg, searchInfoParams->regex, REG_EXTENDED);
                                                    if (searchInfoParams->err == 0)
                                                    {
                                                        searchInfoParams->nmatch = (searchInfoParams->preg.re_nsub + 1);
                                                        searchInfoParams->pmatch = malloc(sizeof (*searchInfoParams->pmatch) * (searchInfoParams->nmatch));
                                                    }
                                                    else
                                                    {
                                                        printf("Invalid Plugin '%s': Compilation failed of regex of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                        exit(EXIT_FAILURE);
                                                    }
                                                }
                                                else
                                                {
                                                    printf("Invalid Plugin '%s': Invalid regex of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    return (EXIT_FAILURE);
                                                }
                                                json_reader_end_member (reader);

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            case 2:
                                            {
                                                SearchInfoParams2_2 *searchInfoParams = calloc(1, sizeof (SearchInfoParams2_2));

                                                if (json_reader_read_member(reader, "line"))
                                                {
                                                    searchInfoParams->line = json_reader_get_int_value(reader);
                                                }
                                                else
                                                {
                                                    printf("Invalid Plugin '%s': Invalid line of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    exit(EXIT_FAILURE);
                                                }
                                                json_reader_end_member (reader);

                                                if (json_reader_read_member(reader, "firstChar"))
                                                {
                                                    searchInfoParams->firstChar = json_reader_get_int_value(reader);
                                                }
                                                else
                                                {
                                                    printf("Invalid Plugin '%s': Invalid firstChar of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    exit(EXIT_FAILURE);
                                                }
                                                json_reader_end_member (reader);

                                                if (json_reader_read_member(reader, "length"))
                                                {
                                                    searchInfoParams->length = json_reader_get_int_value(reader);
                                                }
                                                else
                                                {
                                                    printf("Invalid Plugin '%s': Invalid length of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    exit(EXIT_FAILURE);
                                                }
                                                json_reader_end_member (reader);

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            default:
                                                break;
                                            }
                                            break;
                                        case 3:
                                            switch (searchInfo->idType)
                                            {
                                            case 1:
                                            {
                                                SearchInfoParams3_1 *searchInfoParams = calloc(1, sizeof (SearchInfoParams3_1));
                                                if (json_reader_read_member(reader, "regex"))
                                                {
                                                    searchInfoParams->regex = g_strdup(json_reader_get_string_value(reader));

                                                    /* Regex compilation */
                                                    searchInfoParams->err = regcomp(&searchInfoParams->preg, searchInfoParams->regex, REG_EXTENDED);
                                                    if (searchInfoParams->err == 0)
                                                    {
                                                        searchInfoParams->nmatch = (searchInfoParams->preg.re_nsub + 1);
                                                        searchInfoParams->pmatch = malloc(sizeof (*searchInfoParams->pmatch) * (searchInfoParams->nmatch));
                                                    }
                                                    else
                                                    {
                                                        printf("Invalid Plugin '%s': Compilation failed of regex of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                        exit(EXIT_FAILURE);
                                                    }
                                                }
                                                else
                                                {
                                                    printf("Invalid Plugin '%s': Invalid regex of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    return (EXIT_FAILURE);
                                                }
                                                json_reader_end_member (reader);

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            case 2:
                                            {
                                                SearchInfoParams3_2 *searchInfoParams = calloc(1, sizeof (SearchInfoParams3_2));
                                                if (json_reader_read_member(reader, "firstChar"))
                                                {
                                                    searchInfoParams->firstChar = json_reader_get_int_value(reader);
                                                }
                                                else
                                                {
                                                    printf("Invalid Plugin '%s': Invalid firstChar of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    exit(EXIT_FAILURE);
                                                }
                                                json_reader_end_member (reader);

                                                if (json_reader_read_member(reader, "length"))
                                                {
                                                    searchInfoParams->length = json_reader_get_int_value(reader);
                                                }
                                                else
                                                {
                                                    printf("Invalid Plugin '%s': Invalid length of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    exit(EXIT_FAILURE);
                                                }
                                                json_reader_end_member (reader);

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            default:
                                                break;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        printf("Invalid Plugin '%s': Invalid params of Search ID '%d' of Source ID '%d'\n", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                        return (EXIT_FAILURE);
                                    }
                                    json_reader_end_member (reader);

                                    pushAddonList(
                                                  addonList,
                                                  &srcInfo->idAddon,
                                                  srcInfo->params,
                                                  &searchInfo->period,
                                                  &searchInfo->staticValues,
                                                  &searchInfo->idType,
                                                  searchInfo->params,
                                                  &plgInfo->idPlg,
                                                  &plgInfo->idAsset,
                                                  &srcInfo->idSrc,
                                                  &searchInfo->idSearch
                                                  );
                                    
                                    /* Assign the address of the next element in the new element */
                                    searchInfo->nxt = searchList;

                                    /* Update the pointer of linked list */
                                    searchList = searchInfo;
                                }
                                else
                                {
                                    g_print("Invalid Plugin '%s': Invalid Search n°%d of Source ID '%d'\n", plgPath, j + 1, srcInfo->idSrc);
                                    return EXIT_FAILURE;
                                }
                                json_reader_end_element(reader);
                            }

                            srcInfo->searchList = searchList;
                        }
                        else
                        {
                            printf("Invalid Plugin '%s': Searches of Source ID '%d' must be an array\n", plgPath, srcInfo->idSrc);
                            return (EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        printf("Invalid Plugin '%s': Invalide searches of Source ID '%d'\n", plgPath, srcInfo->idSrc);
                        return (EXIT_FAILURE);
                    }
                    json_reader_end_member (reader);

                    /* Assign the address of the next element in the new element */
                    srcInfo->nxt = srcList;

                    /* Update the pointer of linked list */
                    srcList = srcInfo;
                }
                else
                {
                    g_print("Invalid Plugin '%s': Invalid Source n°%d\n", plgPath, i + 1);
                    return EXIT_FAILURE;
                }
                json_reader_end_element(reader);
            }

            plgInfo->srcList = srcList;
        }
        else
        {
            printf("Invalid Plugin '%s': Sources must be an array\n", plgPath);
            return (EXIT_FAILURE);
        }
    }
    else
    {
        printf("Invalid Plugin '%s': Invalide sources\n", plgPath);
        return (EXIT_FAILURE);
    }
    json_reader_end_member (reader);
    
    /* Assign the address of the next element in the new element */
    plgInfo->nxt = *plgList;

    /* Update the pointer of linked list */
    *plgList = plgInfo;

    g_object_unref(reader);
    g_object_unref(parser);

    return (EXIT_SUCCESS);
}

int plugin(
           const char *plgDir,
           PlgList *plgList,
           AddonList *addonList,
           unsigned int *nbThreads
           )
{
    /* Plugins counter initialisation */
    int nbPlg = 0;

    if (listPlugins(plgDir, &nbPlg, plgList, addonList, nbThreads))
    {
        perror("listPlugins()");
        return (errno);
    }
    g_message("%d plugins loaded", nbPlg);

    return (EXIT_SUCCESS);
}

