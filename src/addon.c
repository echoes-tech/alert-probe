/* 
 * Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */

#include "addon.h"

void *addonLoop(void *arg)
{
    LoopParams *loopParams = (LoopParams*) arg;

    time_t now;

    // What time is it ?
    time(&now);

    switch(loopParams->idAddon)
    {
    case 1:
        switch(loopParams->idType)
        {
        case 1:
            break;
        default:
            break;        
        }
        break;
    case 2:
        switch(loopParams->idType)
        {
        case 1:
            break;
        case 2:
        {
            AddonLocationFileParams *params = (AddonLocationFileParams*) loopParams->params;

            // Method to know when start the loop
            time_t temp =  ((int)(now / params->period) * params->period) + params->period;

            // Diff between now and the start of the loop
            SLEEP(difftime(temp, now));

            while(1)
            {
                addonLocationFile(params->path, params->line, params->firstChar, params->length);
                SLEEP(params->period);
            }
            break;
        }
        default:
            break;        
        }
        break;        
    default:
        break;
    }
    
    pthread_exit(NULL);
}

int addonLocationFile(const char* filePath, unsigned int nLine, unsigned int firstChar, unsigned int length)
{
    FILE* file = NULL;
    char line[MAX_SIZE] = "", res[MAX_SIZE]= "";
    unsigned int n = 0, i = 0;

    // Opening file
    file = fopen(filePath, "r");

    if (file != NULL)
    {
        // Reading file line by line
        while (n < nLine && fgets(line, MAX_SIZE, file) != NULL)
        {
            ++n;
        }

        //TODO: Gérer le fait que le numéro de ligne demandée soit supérieur au nombre de ligne du fichier

        // Closing conf file
        fclose(file);
        
        for(i=0; i < length; ++i)
        {
            res[i] = line[firstChar + i - 1];
        }

        //TODO: envoyer le résultat
        printf("res: %s.\n", res);
    }
    else
    {
        perror("fopen()");
        return (errno);
    }
    
    return (EXIT_SUCCESS);
}

int addon(PlgList *plgList)
{
    char path[255], regex[255] = "";

    PlgInfo *plgInfo = *plgList;
    // Tant que l'on n'est pas au bout de la liste
    while (plgInfo != NULL)
    {
        // On affiche
        printf("idPlg: %d\n", plgInfo->idPlg);
        printf("idAsset: %d\n", plgInfo->idAsset);
        
        SrcInfo *srcInfo = plgInfo->srcList;
        while (srcInfo != NULL)
        {
            
            printf("idSrc: %d\n", srcInfo->idSrc);
            printf("idAddon: %d\n", srcInfo->idAddon);

            if (srcInfo->params == NULL)
            {
                printf("Invalid JSON Node\n");
                return (EXIT_FAILURE);
            }

            switch(srcInfo->idAddon)
            {
            case 2:
            case 3:
            {
                JSONNODE_ITERATOR i = json_begin(srcInfo->params);
                while (i != json_end(srcInfo->params))
                {
                    if (*i == NULL)
                    {
                        printf("Invalid JSON Node\n");
                        return (EXIT_FAILURE);
                    }
                    // Get the node name and value as a string
                    json_char *node_name = json_name(*i);
                    if (!strcmp(node_name, "path"))
                    {
                        json_char *node_value = json_as_string(*i);
                        strcpy(path, node_value);
                        json_free(node_value);
                    }
                    // Cleanup and increment the iterator
                    json_free(node_name);
                    ++i;
                }
                printf("path: %s\n", path);
                break;
            }
            default:
                break;
            }

            SearchInfo *searchInfo = srcInfo->searchList;
            while (searchInfo != NULL)
            {
                printf("idSearch: %d\n", searchInfo->idSearch);
                printf("idType: %d\n", searchInfo->idType);
                printf("period: %d\n", searchInfo->period);
                printf("staticValues: %d\n", searchInfo->staticValues);
                switch (srcInfo->idAddon)
                {
                case 2:
                case 3:
                    switch (searchInfo->idType)
                    {
                    case 1:
                    {
                        JSONNODE_ITERATOR i = json_begin(searchInfo->params);
                        while (i != json_end(searchInfo->params))
                        {
                            if (*i == NULL)
                            {
                                printf("Invalid JSON Node\n");
                                return (EXIT_FAILURE);
                            }
                            // Get the node name and value as a string
                            json_char *node_name = json_name(*i);
                            if (!strcmp(node_name, "regex"))
                            {
                                json_char *node_value = json_as_string(*i);
                                strcpy(regex, node_value);
                                json_free(node_value);
                            }
                            // Cleanup and increment the iterator
                            json_free(node_name);
                            ++i;
                        }
                        printf("regex: %s\n", regex);
                        break;
                    }
                    case 2:
                    {
                        AddonLocationFileParams alfp =
                        {
                            plgInfo->idPlg,
                            plgInfo->idAsset,
                            srcInfo->idSrc,
                            searchInfo->idSearch,
                            searchInfo->period,
                            searchInfo->staticValues,
                            0, // line
                            0, // firstChar
                            0, //length
                            "" // path
                        };
                        strcpy(alfp.path, path);
                        JSONNODE_ITERATOR i = json_begin(searchInfo->params);
                        while (i != json_end(searchInfo->params))
                        {
                            if (*i == NULL)
                            {
                                printf("Invalid JSON Node\n");
                                return (EXIT_FAILURE);
                            }
                            // Get the node name and value as a string
                            json_char *node_name = json_name(*i);
                            if (!strcmp(node_name, "line"))
                            {
                                alfp.line = json_as_int(*i);
                            }
                            else if (!strcmp(node_name, "firstChar"))
                            {
                                alfp.firstChar = json_as_int(*i);
                            }
                            else if (!strcmp(node_name, "length"))
                            {
                                alfp.length = json_as_int(*i);
                            }
                            // Cleanup and increment the iterator
                            json_free(node_name);
                            ++i;
                        }
                        printf("line: %i\n",alfp.line);
                        printf("firstChar: %i\n", alfp.firstChar);
                        printf("length: %i\n", alfp.length);

                        LoopParams loopParams = {srcInfo->idAddon, searchInfo->idType, (void*)&alfp};
                        
                        pthread_t alft;

                        printf("Avant la création du thread.\n");

                        if (pthread_create(&alft, NULL, addonLoop, (void*)&loopParams))
                        {
                            perror("pthread_create");
                            return EXIT_FAILURE;
                        }

                        if (pthread_join(alft, NULL))
                        {
                            perror("pthread_join");
                            return EXIT_FAILURE;
                        }

                        printf("Après la création du thread.\n"); 

                        break;
                    }
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
                            
                searchInfo = searchInfo->nxt;
            }

            srcInfo = srcInfo->nxt;
        }
        
        // On avance d'une case
        plgInfo = plgInfo->nxt;
    }

    return (EXIT_SUCCESS);
}

