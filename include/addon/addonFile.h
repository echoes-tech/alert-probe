/* 
 * Header of Addon File
 * @author ECHOES Technologies (FPO)
 * @date 06/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ADDONFILE_H
#define	ADDONFILE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "addon.h"


/**
 * Thread - Addon File Location.
 * @param *alfp Pointer of addon location file parameters
 */
int addonFileLocation(
                      CollectQueue *collectQueue,
                      const char *line,
                      unsigned int *lineNum,
                      void *params,
                      unsigned short lotNum,
                      unsigned int *valueNum,
                      IDList *idList,
                      time_t *now
                      );

/**
 * Thread - Addon File Regex.
 * @param *arfp Pointer of addon regex file parameters
 */
int addonFileRegex(
                   CollectQueue *collectQueue,
                   const char *line,
                   void *params,
                   unsigned short lotNum,
                   unsigned int *lineNum,
                   unsigned int *valueNum,
                   IDList *idList,
                   time_t *now
                   );

/**
 * Thread - Addon File.
 * @param *arfp Pointer of addon file parameters
 */
void *addonFile(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* ADDONFILE_H */

