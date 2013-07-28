/* 
 * Header of Addon SNMP
 * @author ECHOES Technologies (FPO)
 * @date 07/01/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ADDONSNMP_H
#define	ADDONSNMP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <string.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "addon.h"
#include "search.h"

/**
 * Suppress the type at the begginning of SNMP response (ex: INTEGER: )
 * @param tmpValue
 * @return 
 */
char* typeCut(const char* tmpValue);
    
/**
 * Thread - Addon SNMP Location.
 * @param *alfp Pointer of addon location SNMP parameters
 */
int addonSNMPLocation(
                      CollectQueue *collectQueue,
                      const char *line,
                      unsigned int lineNum,
                      void *params,
                      unsigned short lotNum,
                      IDList *idList,
                      time_t *now
                      );

/**
 * Thread - Addon SNMP Regex.
 * @param *arfp Pointer of addon regex SNMP parameters
 */
int addonSNMPRegex(
                   CollectQueue *collectQueue,
                   const char *line,
                   unsigned int lineNum,
                   void *params,
                   unsigned short lotNum,
                   IDList *idList,
                   time_t *now
                   );

/**
 * Thread - Addon SNMP.
 * @param *arfp Pointer of addon SNMP parameters
 */
void *addonSNMP(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* ADDONSNMP_H */

