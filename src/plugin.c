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

void addBackslash(char *string)
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

    return;
}

int listPlugins(
                const char *plgDir,
                gushort *nbPlg,
                PlgList *plgList,
                AddonList *addonList,
                unsigned int *nbThreads
                )
{
    DIR* dir = NULL;
    struct dirent* read = NULL;

    dir = opendir(plgDir);
    if (dir == NULL)
    {
        g_critical("Critical: %s: %s", strerror(errno), plgDir);
        return EXIT_FAILURE;
    }

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

            if (file2data(plgPath, data)) continue;

            if (data2llist(plgList, addonList, nbThreads, plgPath, data)) continue;
            /*if (file2llist(plgList, addonList, nbThreads, collectQueue, plgPath)) continue;*/

            /* Count number of plugin */
            (*nbPlg)++;
        }
    }

    if (closedir(dir))
    {
        g_critical("Critical: %s: %s", strerror(errno), plgDir);
        return errno;
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
        g_critical("Critical: %s: %s", strerror(errno), plgPath);
        return errno;
    }

    return EXIT_SUCCESS;
}

gushort getIntValue(
                    JsonParser *parser,
                    JsonReader *reader,
                    const char *key,
                    gint64 *value,
                    const char *plgPath,
                    const char *errorMsg
                    )
{
    if (json_reader_read_member(reader, key))
    {
        *value = json_reader_get_int_value(reader);
    }
    else
    {
        g_warning("Invalid Plugin '%s': %s", plgPath, errorMsg);
        g_object_unref(parser);
        g_object_unref(reader);
        return EXIT_FAILURE;
    }
    json_reader_end_member(reader);

    return EXIT_SUCCESS;
}

gushort getStringValue(
                       JsonParser *parser,
                       JsonReader *reader,
                       const char *key,
                       gchar **value,
                       const char *plgPath,
                       const char *errorMsg
                       )
{
    if (json_reader_read_member(reader, key))
    {
        *value = g_strdup(json_reader_get_string_value(reader));
    }
    else
    {
        g_warning("Invalid Plugin '%s': %s", plgPath, errorMsg);
        g_object_unref(reader);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }
    json_reader_end_member(reader);

    return EXIT_SUCCESS;
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
        g_warning("Unable to create a parser for '%s'", plgPath);
        return EXIT_FAILURE;
    }

    json_parser_load_from_data (parser, data, -1, &error);
    if (error)
    {
        g_warning("Unable to parse '%s': %s", plgPath, error->message);
        g_error_free(error);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }
    /*json_parser_load_from_file(parser, plgPath, &error);
    if (error)
    {
        g_warning("Unable to parse '%s': %s", plgPath, error->message);
        g_error_free(error);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }*/

    root = json_parser_get_root(parser);
    if (NULL == root)
    {
        g_warning("Unable to retrieves the top level node of file '%s'.", plgPath);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }
    
    reader = json_reader_new(root);
    if (NULL == reader)
    {
        g_warning("Unable to create new JsonReader for file '%s'.", plgPath);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }

    /* Find out where to store the values */
    if (getIntValue(
                    parser, reader, "id", (gint64*) & plgInfo->idPlg,
                    plgPath, "Invalid Plugin ID"
                    ))
        return EXIT_FAILURE;

    if (getIntValue(
                    parser, reader, "idAsset", (gint64*) & plgInfo->idAsset,
                    plgPath, "Invalid idAsset"
                    ))
        return EXIT_FAILURE;

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

                    if (getIntValue(
                                    parser, reader, "id", (gint64*) & srcInfo->idSrc, plgPath,
                                    g_strdup_printf("Invalid Source ID for Source n°%d", i + 1)
                                    ))
                        return EXIT_FAILURE;

                    if (getIntValue(
                                    parser, reader, "idAddon", (gint64*) & srcInfo->idAddon, plgPath,
                                    g_strdup_printf("Invalid idAddon of Source ID '%d'", srcInfo->idSrc)
                                    ))
                        return EXIT_FAILURE;

                    if (json_reader_read_member (reader, "params"))
                    {
                        switch (srcInfo->idAddon)
                        {
                            case 2:
                            {
                                SrcInfoParams2 *srcInfoParams = calloc(1, sizeof (SrcInfoParams2));
                                
                                if (getStringValue(
                                                   parser, reader, "path", &srcInfoParams->path, plgPath,
                                                   g_strdup_printf("Invalid path of Source ID '%d'", srcInfo->idSrc)
                                                   ))
                                    return EXIT_FAILURE;

                                srcInfo->params = (void*)srcInfoParams;
                                break;
                            }
                            case 3:
                            {
                                SrcInfoParams3 *srcInfoParams = calloc(1, sizeof (SrcInfoParams3));
                                srcInfoParams->nbLine = 0;

                                if (getStringValue(
                                                   parser, reader, "path", &srcInfoParams->path, plgPath,
                                                   g_strdup_printf("Invalid path of Source ID '%d'", srcInfo->idSrc)
                                                   ))
                                    return EXIT_FAILURE;

                                if (getIntValue(
                                                parser, reader, "lastNLines", (gint64*) & srcInfoParams->lastNLines, plgPath,
                                                g_strdup_printf("Invalid lastNLines of Source ID '%d'", srcInfo->idSrc)
                                                ))
                                    return EXIT_FAILURE;

                                srcInfo->params = (void*)srcInfoParams;
                                break;
                            }
                            case 4:
                            {
                                SrcInfoParams4 *srcInfoParams = calloc(1, sizeof (SrcInfoParams4));

                                if (getStringValue(
                                                   parser, reader, "host", &srcInfoParams->host, plgPath,
                                                   g_strdup_printf("Invalid host of Source ID '%d'", srcInfo->idSrc)
                                                   ))
                                    return EXIT_FAILURE;

                                if (getStringValue(
                                                   parser, reader, "user", &srcInfoParams->user, plgPath,
                                                   g_strdup_printf("Invalid user of Source ID '%d'", srcInfo->idSrc)
                                                   ))
                                    return EXIT_FAILURE;

                                if (getStringValue(
                                                   parser, reader, "passwd", &srcInfoParams->passwd, plgPath,
                                                   g_strdup_printf("Invalid passwd of Source ID '%d'", srcInfo->idSrc)
                                                   ))
                                    return EXIT_FAILURE;

                                if (getStringValue(
                                                   parser, reader, "db", &srcInfoParams->db, plgPath,
                                                   g_strdup_printf("Invalid db of Source ID '%d'", srcInfo->idSrc)
                                                   ))
                                    return EXIT_FAILURE;

                                if (getIntValue(
                                                parser, reader, "port", (gint64*) & srcInfoParams->port, plgPath,
                                                g_strdup_printf("Invalid port of Source ID '%d'", srcInfo->idSrc)
                                                ))
                                    return EXIT_FAILURE;

                                srcInfo->params = (void*)srcInfoParams;
                                break;
                            }
                            case 5:
                            {
                                SrcInfoParams5 *srcInfoParams = calloc(1, sizeof (SrcInfoParams5));

                                if (getStringValue(
                                                   parser, reader, "host", &srcInfoParams->host, plgPath,
                                                   g_strdup_printf("Invalid host of Source ID '%d'", srcInfo->idSrc)
                                                   ))
                                    return EXIT_FAILURE;

                                if (getIntValue(
                                                parser, reader, "version", (gint64*) & srcInfoParams->version, plgPath,
                                                g_strdup_printf("Invalid version of Source ID '%d'", srcInfo->idSrc)
                                                ))
                                    return EXIT_FAILURE;

                                /* SNMPv3 */
                                if (srcInfoParams->version == 3)
                                {
                                    if (getStringValue(
                                                       parser, reader, "user", &srcInfoParams->user, plgPath,
                                                       g_strdup_printf("Invalid user of Source ID '%d'", srcInfo->idSrc)
                                                       ))
                                        return EXIT_FAILURE;

                                    if (getStringValue(
                                                       parser, reader, "authProto", &srcInfoParams->authProto, plgPath,
                                                       g_strdup_printf("Invalid authProto of Source ID '%d'", srcInfo->idSrc)
                                                       ))
                                        return EXIT_FAILURE;

                                    if (getStringValue(
                                                       parser, reader, "authPass", &srcInfoParams->authPass, plgPath,
                                                       g_strdup_printf("Invalid authPass of Source ID '%d'", srcInfo->idSrc)
                                                       ))
                                        return EXIT_FAILURE;

                                    if (getStringValue(
                                                       parser, reader, "privProto", &srcInfoParams->privProto, plgPath,
                                                       g_strdup_printf("Invalid privProto of Source ID '%d'", srcInfo->idSrc)
                                                       ))
                                        return EXIT_FAILURE;

                                    if (getStringValue(
                                                       parser, reader, "privPass", &srcInfoParams->privPass, plgPath,
                                                       g_strdup_printf("Invalid privPass of Source ID '%d'", srcInfo->idSrc)
                                                       ))
                                        return EXIT_FAILURE;
                                }
                                /* SNMPv1 */
                                else
                                {
                                    if (getStringValue(
                                                       parser, reader, "community", &srcInfoParams->community, plgPath,
                                                       g_strdup_printf("Invalid community of Source ID '%d'", srcInfo->idSrc)
                                                       ))
                                        return EXIT_FAILURE;
                                }
                                srcInfo->params = (void*)srcInfoParams;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                    else
                    {
                        g_warning("Invalid Plugin '%s': Invalid params of Source ID '%d'", plgPath, srcInfo->idSrc);
                        g_object_unref(reader);
                        g_object_unref(parser);
                        return EXIT_FAILURE;
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

                                    if (getIntValue(
                                                    parser, reader, "id", (gint64*) & searchInfo->idSearch, plgPath,
                                                    g_strdup_printf("Invalid Search ID for Search n°%d of Source ID '%d'", j + 1, srcInfo->idSrc)
                                                    ))
                                        return EXIT_FAILURE;

                                    if (getIntValue(
                                                    parser, reader, "idType", (gint64*) & searchInfo->idType, plgPath,
                                                    g_strdup_printf("Invalid idType of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                    ))
                                        return EXIT_FAILURE;

                                    if (getIntValue(
                                                    parser, reader, "period", (gint64*) & searchInfo->period, plgPath,
                                                    g_strdup_printf("Invalid period of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                    ))
                                        return EXIT_FAILURE;

                                    if (getIntValue(
                                                    parser, reader, "staticValues", (gint64*) & searchInfo->staticValues, plgPath,
                                                    g_strdup_printf("Invalid staticValues of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                    ))
                                        return EXIT_FAILURE;

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

                                                if (getStringValue(
                                                                   parser, reader, "regex", &searchInfoParams->regex, plgPath,
                                                                   g_strdup_printf("Invalid regex of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                   ))
                                                    return EXIT_FAILURE;

                                                /* Regex compilation */
                                                searchInfoParams->err = regcomp(&searchInfoParams->preg, searchInfoParams->regex, REG_EXTENDED);
                                                if (searchInfoParams->err == 0)
                                                {
                                                    searchInfoParams->nmatch = (searchInfoParams->preg.re_nsub + 1);
                                                    searchInfoParams->pmatch = malloc(sizeof (*searchInfoParams->pmatch) * (searchInfoParams->nmatch));
                                                }
                                                else
                                                {
                                                    g_warning("Invalid Plugin '%s': Compilation failed of regex of Search ID '%d' of Source ID '%d'", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    g_object_unref(reader);
                                                    g_object_unref(parser);
                                                    return EXIT_FAILURE;
                                                }

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            case 2:
                                            {
                                                SearchInfoParams2_2 *searchInfoParams = calloc(1, sizeof (SearchInfoParams2_2));

                                                if (getIntValue(
                                                                parser, reader, "line", (gint64*) & searchInfoParams->line, plgPath,
                                                                g_strdup_printf("Invalid line of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                ))
                                                    return EXIT_FAILURE;

                                                if (getIntValue(
                                                                parser, reader, "firstChar", (gint64*) & searchInfoParams->firstChar, plgPath,
                                                                g_strdup_printf("Invalid firstChar of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                ))
                                                    return EXIT_FAILURE;

                                                if (getIntValue(
                                                                parser, reader, "length", (gint64*) & searchInfoParams->length, plgPath,
                                                                g_strdup_printf("Invalid length of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                ))
                                                    return EXIT_FAILURE;

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

                                                if (getStringValue(
                                                                   parser, reader, "regex", &searchInfoParams->regex, plgPath,
                                                                   g_strdup_printf("Invalid regex of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                   ))
                                                    return EXIT_FAILURE;

                                                /* Regex compilation */
                                                searchInfoParams->err = regcomp(&searchInfoParams->preg, searchInfoParams->regex, REG_EXTENDED);
                                                if (searchInfoParams->err == 0)
                                                {
                                                    searchInfoParams->nmatch = (searchInfoParams->preg.re_nsub + 1);
                                                    searchInfoParams->pmatch = malloc(sizeof (*searchInfoParams->pmatch) * (searchInfoParams->nmatch));
                                                }
                                                else
                                                {
                                                    g_warning("Invalid Plugin '%s': Compilation failed of regex of Search ID '%d' of Source ID '%d'", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    g_object_unref(reader);
                                                    g_object_unref(parser);
                                                    return EXIT_FAILURE;
                                                }

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            case 2:
                                            {
                                                SearchInfoParams3_2 *searchInfoParams = calloc(1, sizeof (SearchInfoParams3_2));

                                                if (getIntValue(
                                                                parser, reader, "firstChar", (gint64*) & searchInfoParams->firstChar, plgPath,
                                                                g_strdup_printf("Invalid firstChar of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                ))
                                                    return EXIT_FAILURE;

                                                if (getIntValue(
                                                                parser, reader, "length", (gint64*) & searchInfoParams->length, plgPath,
                                                                g_strdup_printf("Invalid length of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                ))
                                                    return EXIT_FAILURE;

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            default:
                                                break;
                                            }
                                            break;
                                        case 4:
                                            switch (searchInfo->idType)
                                            {
                                            case 1:
                                            {
                                                SearchInfoParams4_1 *searchInfoParams = calloc(1, sizeof (SearchInfoParams4_1));
                                                

                                                if (getStringValue(
                                                                   parser, reader, "query", &searchInfoParams->query, plgPath,
                                                                   g_strdup_printf("Invalid query of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                   ))
                                                    return EXIT_FAILURE;

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            default:
                                                break;
                                            }
                                            break;
                                        case 5:
                                            switch (searchInfo->idType)
                                            {
                                            case 1:
                                            {
                                                SearchInfoParams5_1 *searchInfoParams = calloc(1, sizeof (SearchInfoParams5_1));

                                                if (getStringValue(
                                                                   parser, reader, "oid", &searchInfoParams->oid, plgPath,
                                                                   g_strdup_printf("Invalid oid of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                   ))
                                                    return EXIT_FAILURE;

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            case 2:
                                            {
                                                SearchInfoParams5_2 *searchInfoParams = calloc(1, sizeof (SearchInfoParams5_2));

                                                if (getStringValue(
                                                                   parser, reader, "oid", &searchInfoParams->oid, plgPath,
                                                                   g_strdup_printf("Invalid oid of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                   ))
                                                    return EXIT_FAILURE;

                                                if (getStringValue(
                                                                   parser, reader, "regex", &searchInfoParams->regex, plgPath,
                                                                   g_strdup_printf("Invalid regex of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                   ))
                                                    return EXIT_FAILURE;

                                                /* Regex compilation */
                                                searchInfoParams->err = regcomp(&searchInfoParams->preg, searchInfoParams->regex, REG_EXTENDED);
                                                if (searchInfoParams->err == 0)
                                                {
                                                    searchInfoParams->nmatch = (searchInfoParams->preg.re_nsub + 1);
                                                    searchInfoParams->pmatch = malloc(sizeof (*searchInfoParams->pmatch) * (searchInfoParams->nmatch));
                                                }
                                                else
                                                {
                                                    g_warning("Invalid Plugin '%s': Compilation failed of regex of Search ID '%d' of Source ID '%d'", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                                    g_object_unref(reader);
                                                    g_object_unref(parser);
                                                    return EXIT_FAILURE;
                                                }

                                                searchInfo->params = (void*)searchInfoParams;
                                                break;
                                            }
                                            case 3:
                                            {
                                                SearchInfoParams5_3 *searchInfoParams = calloc(1, sizeof (SearchInfoParams5_3));

                                                if (getStringValue(
                                                                   parser, reader, "oid", &searchInfoParams->oid, plgPath,
                                                                   g_strdup_printf("Invalid oid of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                   ))
                                                    return EXIT_FAILURE;

                                                if (getIntValue(
                                                                parser, reader, "firstChar", (gint64*) & searchInfoParams->firstChar, plgPath,
                                                                g_strdup_printf("Invalid firstChar of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                ))
                                                    return EXIT_FAILURE;

                                                if (getIntValue(
                                                                parser, reader, "length", (gint64*) & searchInfoParams->length, plgPath,
                                                                g_strdup_printf("Invalid length of Search ID '%d' of Source ID '%d'", searchInfo->idSearch, srcInfo->idSrc)
                                                                ))
                                                    return EXIT_FAILURE;

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
                                        g_warning("Invalid Plugin '%s': Invalid params of Search ID '%d' of Source ID '%d'", plgPath, searchInfo->idSearch, srcInfo->idSrc);
                                        g_object_unref(reader);
                                        g_object_unref(parser);
                                        return EXIT_FAILURE;
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
                                    g_warning("Invalid Plugin '%s': Invalid Search n°%d of Source ID '%d'", plgPath, j + 1, srcInfo->idSrc);
                                    g_object_unref(reader);
                                    g_object_unref(parser);
                                    return EXIT_FAILURE;
                                }
                                json_reader_end_element(reader);
                            }

                            srcInfo->searchList = searchList;
                        }
                        else
                        {
                            g_warning("Invalid Plugin '%s': Searches of Source ID '%d' must be an array", plgPath, srcInfo->idSrc);
                            g_object_unref(reader);
                            g_object_unref(parser);
                            return EXIT_FAILURE;
                        }
                    }
                    else
                    {
                        g_warning("Invalid Plugin '%s': Invalide searches of Source ID '%d'", plgPath, srcInfo->idSrc);
                        g_object_unref(reader);
                        g_object_unref(parser);
                        return EXIT_FAILURE;
                    }
                    json_reader_end_member (reader);

                    /* Assign the address of the next element in the new element */
                    srcInfo->nxt = srcList;

                    /* Update the pointer of linked list */
                    srcList = srcInfo;
                }
                else
                {
                    g_warning("Invalid Plugin '%s': Invalid Source n°%d", plgPath, i + 1);
                    g_object_unref(reader);
                    g_object_unref(parser);
                    return EXIT_FAILURE;
                }
                json_reader_end_element(reader);
            }

            plgInfo->srcList = srcList;
        }
        else
        {
            g_warning("Invalid Plugin '%s': Sources must be an array", plgPath);
            g_object_unref(reader);
            g_object_unref(parser);
            return EXIT_FAILURE;
        }
    }
    else
    {
        g_warning("Invalid Plugin '%s': Invalide sources", plgPath);
        g_object_unref(reader);
        g_object_unref(parser);
        return EXIT_FAILURE;
    }
    json_reader_end_member (reader);
    
    /* Assign the address of the next element in the new element */
    plgInfo->nxt = *plgList;

    /* Update the pointer of linked list */
    *plgList = plgInfo;

    g_object_unref(reader);
    g_object_unref(parser);

    return EXIT_SUCCESS;
}

gushort plugin(
               const char *plgDir,
               PlgList *plgList,
               AddonList *addonList,
               unsigned int *nbThreads
               )
{
    /* Plugins counter initialisation */
    gushort nbPlg = 0;

    if (listPlugins(plgDir, &nbPlg, plgList, addonList, nbThreads))
        return EXIT_FAILURE;

    g_message("%d plugins loaded", nbPlg);

    return EXIT_SUCCESS;
}

