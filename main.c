/* 
 * Main
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// To init struct Conf
#include "conf.h"
// To init struct PlgList
#include "plugin.h"
// To init struct AddonLoop and Params
#include "addon.h"

// Probe Name
#define NAME "ECHOES Alert - Probe"
// Probe Version
#define VERSION "0.1.0"
// Conf Repository
#define CONF_DIR "conf/echoes-alert.conf"

/**
 * Main function
 * @return Exit status
 */
int main(int argc, char** argv)
{
    // Initialization
    Conf conf = {0, 0, "", ""};
    
    // Plugins counter initialisation
    unsigned int nbThreads = 0;
    PlgList plgList = NULL;
    
    // Addons threads initialisation
    pthread_t *addonsThreads = NULL;
/*
    LoopParams *loopParams = {NULL};
    AddonLocationFileParams *alfp = {NULL};
*/
    

    // Help message and version
    if (argc > 1)
    {
        if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
        {
            printf("%s %s\n", NAME, VERSION);
        }
        else
        {
            printf(
                   "Usage:\n"
                   "\techoes-alert [OPTION...]\n\n"
                   "%s options\n"
                   "\t-h or --help\tPrint this message.\n"
                   "\t-v or --version\tPrint %s version.\n",
                   NAME,
                   NAME
                   );
        }
        return (EXIT_SUCCESS);
    }

    // Affichage à l'écran le démarrage de la sonde
    //TODO: ne l'afficher qu'en mode verbose
    printf("---------- %s %s ----------\n", NAME, VERSION);

    printf("Début du chargement des conf\n");
    if (loadConf(CONF_DIR, &conf))
    {
        perror("loadConf()");
        return (errno);
    }
    printf("Fin du chargement des conf\n");

    printf("Début du chargement des plugins\n");
    if (plugin(conf.pluginDir, &plgList, &nbThreads))
    {
        perror("plugin()");
        return (errno);
    }
    printf("Fin du chargement des plugins\n");

    addonsThreads = calloc(nbThreads, sizeof (pthread_t));
    if (addonsThreads == NULL)
    {
        return (EXIT_FAILURE);
    }
/*
    loopParams = calloc(nbThreads, sizeof (pthread_t));
    if (loopParams == NULL)
    {
        return (EXIT_FAILURE);
    }
    alfp = calloc(nbThreads, sizeof (pthread_t));
    if (alfp == NULL)
    {
        return (EXIT_FAILURE);
    }
*/

    printf("Début du chargement des addons\n");
    if (addon(&nbThreads, &plgList, addonsThreads))
    //if (addon(&nbThreads, &plgList, addonsThreads, loopParams, alfp))
    {
        perror("addon()");
        return (errno);
    }
    printf("Fin du chargement des addons\n");

    printf("Début de l'envoi du message\n");
    if (sender(conf.engineFQDN, &conf.enginePort, &conf.probeProto))
    {
        perror("sender()");
        return (errno);
    }
    printf("Fin de l'envoi du message\n");

    unsigned int i;
    for (i = 0; i < nbThreads; i++)
    {
        if (pthread_join(addonsThreads[i], NULL))
        {
/*
            perror("pthread_join");
            return EXIT_FAILURE;
*/
        }        
    }

    //TODO: free calloc!
    
    return (EXIT_SUCCESS);
}

