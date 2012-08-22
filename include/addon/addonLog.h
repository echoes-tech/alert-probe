/* 
 * Header of Addon Log
 * @author ECHOES Technologies (FPO)
 * @date 06/08/2012
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
                  void *params,
                  unsigned short lotNum,
                  unsigned int *valueNum,
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
                     void *params,
                     unsigned short lotNum,
                     unsigned int *valueNum,
                     IDList *idList,
                     time_t *now
                     );


void whileAddonTypeInfo(AddonParamsInfo *addonParamsInfo, const char *line, time_t *now);

/**
 * Thread - Addon Log.
 * @param *arfp Pointer of addon file parameters
 */
void *addonLog(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* ADDONLOG_H */

