/* 
 * Plug-in Manager
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#include "plugin.h"

int verifExt(char s[])
{
   int i = strlen(s);
 
   if ((s[i - 5] == '.') && (s[i - 4] == 'j') && (s[i - 3] == 's') && (s[i - 2] == 'o') && (s[i - 1] == 'n'))
   {
      return EXIT_SUCCESS;
   }
   return EXIT_FAILURE;
}
int addBackslash (char *string)
{
    char *backslash = NULL, *rest = NULL, tmp[25500] = "";
    
    // Search \ character
    backslash = strchr(string, '\\');
    
    if (backslash != NULL)
    {
        // Value is String after equal, end of line
        rest = backslash + 1;
         
        // Replace = character by a end of String
        *backslash = '\0';
        
        strcpy(tmp,string);
        strcat(tmp, "\\\\");
        
        addBackslash(rest);
        
        strcat(tmp, rest);
        strcpy(string, tmp);
    }

    return (EXIT_SUCCESS);
}

int listPlugins(const char *plgDir, int *nbPlg, PlgList *plgList)
{  
    DIR* dir = NULL;
    struct dirent* read = NULL;

    dir = opendir(plgDir);
    if (dir == NULL)
        return(EXIT_FAILURE);

    // Reading filename file by file
    while ((read = readdir(dir)))
    {
        if (!verifExt(read->d_name))
        {
            // Count number of plugin
            (*nbPlg)++;

            char plgPath[255] = "";
            //TODO: define max line by plugin
            json_char json[MAX_SIZE * 1000] = "";
            JSONNODE *n = NULL;

            // Put repository for plugin path
            strcpy(plgPath, plgDir);
            // Add filename for plugin path
            strcat(plgPath, read->d_name);
            
            printf("Loading plugin %s\n", plgPath);

            // New element of linked list
            //PlgInfo* plgInfo = calloc(1, sizeof(PlgInfo));

            if (file2json(plgPath, json))
            {
                perror("file2json()");
                return (errno);
            }

            n = json_parse(json);

            if (json2llist(n, plgList))
            {
                perror("json2llist()");
                return (errno);
            }
 
            // Assign the address of the next element in the new element
            //plgInfo->nxt = plgList;

            // Update the pointer of linked list
            //plgList = plgInfo;
        }
    }

    if (closedir(dir))
    {
        perror("closedir()");
        return (errno);
    }
    return EXIT_SUCCESS;
}

int file2json(const char *plgPath, json_char* json)
{
    FILE* plgFile = NULL;
    char line[MAX_SIZE] = "";

    // Opening file
    plgFile = fopen(plgPath, "r");

    if (plgFile != NULL)
    {

        // Reading file line by lfileine
        while (fgets(line, MAX_SIZE, plgFile) != NULL)
        {
            addBackslash(line);
            strcat(json, line);
        }
        // Closing plugin file
        fclose(plgFile);

        printf("%s",json);
        
        if(!json_is_valid(json))
        {
            printf("Invalid JSON Node : %s\n", plgPath);
            return (EXIT_FAILURE);
        }
    }
    else
    {
        perror("fopen()");
        return (errno);
    }

    return (EXIT_SUCCESS);
}

int json2llist(JSONNODE *n, PlgList *plgList)
{
    if (n == NULL){
        printf("Invalid JSON Node\n");
        return (EXIT_FAILURE);
    }

    JSONNODE_ITERATOR i = json_begin(n);

    while (i != json_end(n)){
        if (*i == NULL){
            printf("Invalid JSON Node\n");
            return (EXIT_FAILURE);
        }
 
        // get the node name and value as a string
        json_char *node_name = json_name(*i);

        // find out where to store the values
        PlgInfo plgInfo = {0};
        if (!strcmp(node_name, "id"))
        {
            plgInfo.idPlg = json_as_int(*i);
        }
        else if (!strcmp(node_name, "idAsset"))
        {
            plgInfo.idAsset = json_as_int(*i);
        }
        else if (!strcmp(node_name, "sources"))
        {
            SrcInfo srcInfo = {&plgInfo, 0, 0, NULL};
            JSONNODE_ITERATOR j = json_begin(*i);

            while (j != json_end(*i))
            {
                // get the node name and value as a string
                json_char *node_name = json_name(*j);

                if (*j == NULL)
                {
                    printf("Invalid JSON Node\n");
                    return (EXIT_FAILURE);
                }
                if (!strcmp(node_name, "id"))
                {
                    srcInfo.idSrc = json_as_int(*j);
                }
                else if (!strcmp(node_name, "idAddon"))
                {
                    srcInfo.idAddon = json_as_int(*j);
                }
                else if (!strcmp(node_name, "params"))
                {
                    srcInfo.params = *j;
                }
                else if (!strcmp(node_name, "searches"))
                {
                    JSONNODE_ITERATOR k = json_begin(*j);
                    // New element of linked list
                    FileSearchInfo* fileSearchInfo = calloc(1, sizeof(fileSearchInfo));
                    while (k != json_end(*j))
                    {
                        // get the node name and value as a string
                        json_char *node_name = json_name(*j);
                        switch (srcInfo.idAddon)
                        {
                        case 1:
                            if (!strcmp(node_name, "id"))
                            {
                                fileSearchInfo->idSearch = json_as_int(*i);
                            }
                            else if (!strcmp(node_name, "idType"))
                            {
                                fileSearchInfo->idType = json_as_int(*i);
                            }
                            else if (!strcmp(node_name, "params"))
                            {
                                fileSearchInfo->params = *i;
                            }
                            else if (!strcmp(node_name, "period"))
                            {
                                json_char *node_value = json_as_string(*i);
                                strcpy(fileSearchInfo->period, node_value);
                                json_free(node_value);
                            }
                            else if (!strcmp(node_name, "staticValues"))
                            {
                                fileSearchInfo->staticValues = json_as_int(*i);
                            }
                            break;
                        default:
                            break;
                        }
                    // cleanup and increment the iterator
                    json_free(node_name);
                    ++k;
                    }
                    switch (srcInfo.idAddon)
                    {
                    case 1:
                        // Assign the address of the next element in the new element
                        fileSearchInfo->nxt = plgList->fileSearchList;

                        // Update the pointer of linked list
                        plgList->fileSearchList = fileSearchInfo;
                        break;
                    default:
                        break;
                    }
                }
                // cleanup and increment the iterator
                json_free(node_name);
                ++j;
            }
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
    return (EXIT_SUCCESS);
}

int plugin(const char *plgDir, PlgList *plgList)
{
    // Plugins counter initialisation
    int nbPlg = 0;

    if (listPlugins(plgDir, &nbPlg, plgList))
    {
        perror("listPlugins()");
        return (errno);
    }
    printf("%d plugins load.\n", nbPlg);
    
    return (EXIT_SUCCESS);
}

