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

int listPlugins(const char *plgDir, int *nbPlg, PlgList plgList)
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
            
            // New element of linked list
            PlgInfo* plgInfo = calloc(1, sizeof(PlgInfo));
            
            // Put repository for plugin path
            strcpy(plgInfo->plgPath, plgDir);
            // Add filename for plugin path
            strcat(plgInfo->plgPath, read->d_name);
            
            printf("FICHIER: %s\n", plgInfo->plgPath);
            
            loadPlugin(plgInfo);

            // Assign the address of the next element in the new element
            plgInfo->nxt = plgList;

            // Update the pointer of linked list
            plgList = plgInfo;
        }
    }

    if (closedir(dir))
    {
        perror("closedir()");
        return (errno);
    }
    return EXIT_SUCCESS;
}

int loadPlugin(PlgInfo* plgInfo)
{
    FILE* plgFile = NULL;
    char line[MAX_SIZE] = "";
    //TODO: define max line by plugin
    char json[MAX_SIZE * 1000] = "";

    // Opening file
    plgFile = fopen(plgInfo->plgPath, "r");

    if (plgFile != NULL)
    {
        // Reading file line by line
        while (fgets(line, MAX_SIZE, plgFile) != NULL)
        {
            strcat(json, line);
        }
        // Closing plugin file
        fclose(plgFile);

        plgInfo->n = json_parse(json);
    }
    else
    {
        perror("fopen()");
        return (errno);
    }

    return (EXIT_SUCCESS);
}

int plugin(const char *plgDir, PlgList plgList)
{
    int nbPlg = 0;

    if (listPlugins(plgDir, &nbPlg, plgList))
    {
        perror("listPlugins()");
        return (errno);
    }
    printf("Nombre de plugins : %d\n", nbPlg);
    
    return (EXIT_SUCCESS);
}

