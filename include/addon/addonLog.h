/* 
 * Header of Addon Log
 * @author ECHOES Technologies (FPO)
 * @date 06/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ADDONLOG_H
#define	ADDONLOG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "addon.h"

int addonLogRegex(
                  CollectQueue *collectQueue,
                  const char *line,
                  unsigned int lineNum,
                  void *params,
                  unsigned short lotNum,
                  IDList *idList,
                  time_t *now
                  );

/**
 * Thread - Addon Location Log.
 * @param *allp Pointer of addon location log parameters
 */
int addonLogLocation(
                     CollectQueue *collectQueue,
                     const char *line,
                     unsigned int lineNum,
                     void *params,
                     unsigned short lotNum,
                     IDList *idList,
                     time_t *now
                     );


void whileAddonTypeInfo(
                        AddonParamsInfo *addonParamsInfo,
                        const char *line,
                        unsigned int lineNum,
                        time_t *now
                        );

/**
 * Thread - Addon Log.
 * @param *arfp Pointer of addon file parameters
 */
void *addonLog(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* ADDONLOG_H */

