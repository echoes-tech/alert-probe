/* 
 * Header of Addon FileSystem
 * @author ECHOES Technologies (FPO)
 * @date 05/08/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ADDONFILESYSTEM_H
#define	ADDONFILESYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/statvfs.h>

#include "addon.h"

int addonFileSystemStatsvfs(
                            CollectQueue *collectQueue,
                            void *params,
                            unsigned short lotNum,
                            IDList *idList,
                            time_t *now
                            );

/**
 * Thread - Addon FileSystem.
 * @param *arfsp Pointer of addon FileSystem parameters
 */
void *addonFileSystem(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* ADDONFILESYSTEM_H */

