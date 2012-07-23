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
// To init struct AddonsMgrParams
#include "addon.h"
// To init struct FormatParams
#include "format.h"
// To init struct SenderParams
#include "sender.h"

// Probe Names
#define PRODUCT_NAME "ECHOES Alert - Probe"
#define APP_NAME "echoes-alert-probe"
// Probe Version
#define VERSION "0.1.0"
// Conf Repository
#define CONF_DIR "./conf/echoes-alert.conf"

/**
 * Main function
 * @return Exit status
 */
int main(int argc, char** argv)
{
    //
    // Initialization
    // 
    
    // Probe Configuration initialisation
    Conf conf = CONF_INITIALIZER;
    
    // Plugins counter initialisation
    unsigned int nbThreads = 0;
    
    // Addons Manager, Format Module and Sender Module threads initialisations
    pthread_t addonsMgrThread = 0, formatThread = 0, senderThread = 0;

    // Addons Manager thread params initialisation
    AddonsMgrParams addonsMgrParams = ADDON_PARAMS_INITIALIZER;

    // Queues initialisation
    SDElementQueue sdElementQueue = {
        PTHREAD_MUTEX_INITIALIZER,
        "",
        APP_NAME,
        &conf.probeID,
        &conf.transportMsgVersion,
        getpid(),
        NULL
    };
    
    gethostname(sdElementQueue.hostname, 255);

    // Format thread initilisation
    FormatParams formatParams = {
        &addonsMgrParams.collectQueue,
        &sdElementQueue
    };

    // Sender thread initilisation
    SenderParams senderParams = {
        &sdElementQueue,
        conf.engineFQDN,
        &conf.enginePort,
        &conf.transportProto,
        0
    };

    // Help message and version
    if (argc > 1)
    {
        if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
        {
            printf("%s %s\n", PRODUCT_NAME, VERSION);
        }
        else
        {
            printf(
                   "Usage:\n"
                   "\t%s [OPTION...]\n\n"
                   "%s options\n"
                   "\t-h or --help\tPrint this message.\n"
                   "\t-v or --version\tPrint %s version.\n",
                   APP_NAME,
                   PRODUCT_NAME,
                   PRODUCT_NAME
                   );
        }
        return (EXIT_SUCCESS);
    }

    // Affichage à l'écran le démarrage de la sonde
    //TODO: ne l'afficher qu'en mode verbose
    printf("---------- %s %s ----------\n", PRODUCT_NAME, VERSION);

    printf("Début du chargement des conf\n");
    if (loadConf(CONF_DIR, &conf))
    {
        perror("loadConf()");
        return (errno);
    }
    printf("Fin du chargement des conf\n");

    printf("Début du chargement des plugins\n");
    if (plugin(conf.probePluginDir, &addonsMgrParams.plgList, &nbThreads))
    {
        perror("plugin()");
        return (errno);
    }
    printf("Fin du chargement des plugins\n");

    // Table addonsThreads creation
    addonsMgrParams.addonsThreads = calloc(nbThreads, sizeof (pthread_t));
    if (addonsMgrParams.addonsThreads == NULL)
    {
        return (EXIT_FAILURE);
    }
    // Table loopsParams creation
    addonsMgrParams.loopsParams = calloc(nbThreads, sizeof (LoopParams));
    if (addonsMgrParams.loopsParams == NULL)
    {
        return (EXIT_FAILURE);
    }

    printf("Début du chargement des addons\n");
    if (pthread_create(&addonsMgrThread, NULL, addon, (void*) &addonsMgrParams))
    {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    printf("Début du chargement du module Format\n");
    if (pthread_create(&formatThread, NULL, format, (void*) &formatParams))
    {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    printf("Début de l'envoi des messages\n");
    if (pthread_create(&senderThread, NULL, sender, (void*) &senderParams))
    {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    pthread_join(addonsMgrThread, NULL);
    pthread_join(formatThread, NULL);
    pthread_join(senderThread, NULL);/* Attente de la fin des threads */

    // Cleanup
    free(addonsMgrParams.addonsThreads);
    free(addonsMgrParams.loopsParams);

    return (EXIT_SUCCESS);
}

