/* 
 * Header of Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */


#ifndef ADDON_H
#define	ADDON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "plugin.h"

// Portable sleep
#if defined (WIN32) || defined (WIN64)

#include <windows.h>
#define SLEEP(time) Sleep(time*1000)

#else

#include <unistd.h>
#define SLEEP(time) sleep(time)

#endif


/**
 * Main function of Addons Manager
 * @param *plgList Pointer of Plugins list with informations
 * @return Exit status
 */
int addon(PlgList *plgList);

#endif	/* ADDON_H */

