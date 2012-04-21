/* 
 * Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */

#include "addon.h"

void *addonLocationFileLoop(void *arg)
{
    unsigned int period = 5;
    time_t now;

    // What time is it ?
    time(&now);

    // Method to know when start the loop
    time_t temp =  ((int)(now / period) * period) + period;

    // Diff between now and the start of the loop
    SLEEP(difftime(temp, now));

    while(1)
    {
        addonLocationFile("/var/log/syslog", 10, 25, 2);
        SLEEP(period);
    }
    
    // Pour enlever le warning
    (void) arg;
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

void *addonLoop(void *arg)
{
    printf("On lance l'addon.\n");
    time_t temps;
    struct tm *tm;
    
    time(&temps);
    tm = gmtime(&temps);
    
    // Pour enlever le warning
    (void) arg;
    pthread_exit(NULL);
}

int addon(PlgList *plgList)
{
    PlgInfo *tmp = *plgList;
    // Tant que l'on n'est pas au bout de la liste
    while (tmp != NULL)
    {
        // On affiche
        printf("idPlg: %d\n", tmp->idPlg);
        printf("idAsset: %d\n", tmp->idAsset);
        
        SrcInfo *tmp2 = tmp->srcList;
        while (tmp2 != NULL)
        {
            
            printf("idSrc: %d\n", tmp2->idSrc);
            printf("idAddon: %d\n", tmp2->idAddon);
            SearchInfo *tmp3 = tmp2->searchList;
            while (tmp3 != NULL)
            {
                printf("idSearch: %d\n", tmp3->idSearch);
                printf("idType: %d\n", tmp3->idType);
                printf("period: %s\n", tmp3->period);
                printf("staticValues: %d\n", tmp3->staticValues);
                tmp3 = tmp3->nxt;
            }
            if (tmp2->params == NULL)
            {
                printf("Invalid JSON Node\n");
                return (EXIT_FAILURE);
            }   
            tmp2 = tmp2->nxt;
        }

/*
        JSONNODE_ITERATOR i = json_begin(tmp->params);
       // Get the node name and value as a string
        json_char *node_name = json_name(*i);
        switch (tmp->idType)
        {
        case 1:
            if (strcmp(node_name, "regex") == 0){
                json_char *node_value = json_as_string(*i);
                strcpy(regex, node_value);
                json_free(node_value);
            }
            break;
        case 2:
            break;
        default:
            break;
        }
*/
        
        // On avance d'une case
        tmp = tmp->nxt;
    }


/*
    pthread_t thread1;

    printf("Avant la création du thread.\n");

    if (pthread_create(&thread1, NULL, addonLocationFileLoop, NULL))
    {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    if (pthread_join(thread1, NULL))
    {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    printf("Après la création du thread.\n"); 
*/

    return (EXIT_SUCCESS);
}

