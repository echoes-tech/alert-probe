/* 
 * File:   main.c
 * Author: Florent Poinsaut <florent.poinsaut@echoes-tech.com>
 *
 * Created on 13 février 2012, 16:04
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <CUnit/Basic.h>

#include "main.h"

/**
 * Main function
 * @return Exit status
 */
int main(int argc, char** argv) {

    // Affichage à l'écran le démarrage de la sonde
    //TODO : ne l'afficher qu'en mode verbose
    printf("---------- %s %s ----------\n", NAME, VERSION);

    printf("Début de l'envoi du message\n");
    if (sender(SRV_FQDN, PORT, PROTO)) {
        perror("sender()");
        return(errno);
    }
    printf("Fin de l'envoi du message\n");

    return (EXIT_SUCCESS);
}

