/* 
 * Header of Addon ODBC
 * @author ECHOES Technologies (FPO)
 * @date 10/09/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ADDONODBC_H
#define	ADDONODBC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <sql.h>
#include <sqlext.h>
    
#include "addon.h"
    
/**
 * Thread - Addon ODBC Query.
 * @param 
 */
int addonODBCQuery(
        CollectQueue *collectQueue,
        SQLHSTMT *hstmt,
        void *params,
        unsigned short lotNum,
        IDList *idList,
        time_t *now
        );

void displayError(char* s, SQLSMALLINT HandleType, SQLHANDLE Handle);
  
/**
 * Thread - Addon ODBC.
 * @param *aqmp Pointer of addon ODBC parameters
 */
void *addonODBC(void *arg);

#ifdef	__cplusplus
}
#endif

#endif	/* ADDONODBC_H */

