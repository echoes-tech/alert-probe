/* 
 * Main
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "conf.h"

// Probe Name
#define NAME "ECHOES Alert - Probe"
// Probe Version
#define VERSION "0.1.0"

/**
 * Main function
 * @return Exit status
 */
int main(int argc, char** argv)
{
    // Initialization
    Conf conf = {0, 0, "", ""};

    if (argc > 1)
    {
        if (argv[1] == "-v" || argv[1] == "--version")
        {
            printf("%s %s\n", NAME, VERSION);
        }
        else
        {
            printf(
                    "Usage:\n"
                    "\techoes-alert [OPTION...]\n\n"
                    "%s options\n"
                    "\t-h or -help\tPrint this message.\n"
                    "\t-v or -version\tPrint %s version.\n",
                   NAME,
                   NAME
                    );
        }
        exit(EXIT_SUCCESS);
    }

    // Affichage à l'écran le démarrage de la sonde
    //TODO : ne l'afficher qu'en mode verbose
    printf("---------- %s %s ----------\n", NAME, VERSION);

    printf("Début du chargement des conf\n");
    if (loadConf("conf/echoes-alert.conf", &conf))
    {
        perror("loadConf()");
        return (errno);
    }
    printf("Fin du chargement des conf\n");

    printf("Début du chargement des plug-in\n");
    if (plugin())
    {
        perror("plugin()");
        return (errno);
    }
    printf("Fin du chargement des plug-in\n");

    printf("Début de l'envoi du message\n");
    if (sender(conf.engineFQDN, conf.enginePort, conf.probeProto))
    {
        perror("sender()");
        return (errno);
    }
    printf("Fin de l'envoi du message\n");

    return (EXIT_SUCCESS);
}

