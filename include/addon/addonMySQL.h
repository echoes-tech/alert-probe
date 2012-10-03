/* 
 * Header of Addon MySQL
 * @author ECHOES Technologies (FPO)
 * @date 02/10/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ADDONMYSQL_H
#define	ADDONMYSQL_H

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef WIN32
#include <winsock.h>
#include <MYSQL/mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include "addon.h"

/**
 * Thread - Addon MySQL Query.
 * @param 
 */
int addonMySQLQuery(CollectQueue *collectQueue,
                    MYSQL *mysql,
                    void *params,
                    unsigned short lotNum,
                    IDList *idList,
                    time_t *now
                    );

/**
 * Thread - Addon MySQL.
 * @param *aqmp Pointer of addon mysql parameters
 */
void *addonMySQL(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* ADDONMYSQL_H */

