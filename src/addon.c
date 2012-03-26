/* 
 * Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */

#include "addon.h"

void *addonFile(void *arg)
{
    printf("On lance l'addon.\n");

    /* Pour enlever le warning */
    (void) arg;
    pthread_exit(NULL);
}



int addon(PlgList plgList)
{


    return (EXIT_SUCCESS);
}

