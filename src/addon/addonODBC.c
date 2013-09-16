/* 
 * Addon ODBC
 * @author ECHOES Technologies (MLA)
 * @date 10/09/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "addon/addonODBC.h"

int addonODBCQuery(
                   CollectQueue *collectQueue,
                   SQLHSTMT *hstmt,
                   void *params,
                   unsigned short lotNum,
                   IDList *idList,
                   time_t *now
                   )
{
    SearchInfoParams6_3 *searchInfoParams = (SearchInfoParams6_3*) params;

    SQLRETURN retcode;

    char **values = NULL;

    unsigned int i = 0, n = 1;
    SQLSMALLINT nbFields = 0;

    /* Executes query */
    if (SQLExecDirect(hstmt, searchInfoParams->query, SQL_NTS) == SQL_SUCCESS)
    {
        SQLNumResultCols(hstmt, &nbFields);
        values = calloc(nbFields, sizeof (char*));

        if (values)
        {
            while (SQLFetch(hstmt) == SQL_SUCCESS)
            {
                for (i = 1; i <= (unsigned int) nbFields; i++)
                {
                    SQLLEN indicator;

                    /* Size of integer and date in char format */
                    size_t bufLenght = 32 * sizeof (char);
                    char *buf = calloc(1, bufLenght);

                    retcode = SQLGetData(hstmt, i, SQL_C_CHAR, buf, bufLenght, &indicator) == SQL_SUCCESS_WITH_INFO;
                    if (SQL_SUCCEEDED(retcode))
                    {
                        /* If lenght greater than 32 */
                        if (retcode == SQL_SUCCESS_WITH_INFO)
                        {
                            char *tmp = NULL;

                            bufLenght = indicator * sizeof (char);
                            buf = realloc(buf, bufLenght);
                            tmp = calloc(1, bufLenght);
                            /* buf is first part of the string, tmp second */
                            if (SQLGetData(hstmt, i, SQL_C_CHAR, tmp, bufLenght, &indicator) == SQL_SUCCESS)
                            {
                                strcat(buf, tmp);
                                free(tmp);
                            }
                            else
                            {
                                free(tmp);
                                free(buf);
                                displayError("get data", SQL_HANDLE_STMT, hstmt);
                                return EXIT_FAILURE;
                            }
                        }
                        values[i - 1] = strdup(indicator != SQL_NULL_DATA ? buf : "");
                    }
                    else
                    {
                        free(buf);
                        displayError("get data", SQL_HANDLE_STMT, hstmt);
                        return EXIT_FAILURE;
                    }
                    free(buf);
                }
                if (pushCollectQueue(
                                     collectQueue,
                                     idList,
                                     lotNum,
                                     n,
                                     (unsigned int) nbFields,
                                     values,
                                     *now
                                     ))
                {
                    /* Cleanup */
                    free(values);
                    return EXIT_FAILURE;
                }
                ++n;
            }
            /* Cleanup */
            free(values);
        }
        else
        {
            g_warning("Critical: Insufficient memory");
            return EXIT_FAILURE;
        }
    }
    else
        displayError("query", SQL_HANDLE_STMT, hstmt);

    /* Cleanup to execute another query */
    SQLMoreResults(hstmt);

    return EXIT_SUCCESS;
}

void displayError(char* s, SQLSMALLINT HandleType, SQLHANDLE Handle)
{
    SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
    SQLSMALLINT MsgLen;
    SQLINTEGER NativeError;
    SQLRETURN retcode;
    int i = 1;
    do
    {
        retcode = SQLGetDiagRec(HandleType, Handle, i++, SqlState, &NativeError, Msg, sizeof (Msg), &MsgLen);
        if (SQL_SUCCEEDED(retcode))
            g_warning("%s - %s - %d - %d : %s\n", s, SqlState, i, NativeError, Msg);
    }
    while (retcode == SQL_SUCCESS);
}

void *addonODBC(void *arg)
{
    AddonParamsInfo *addonParamsInfo = (AddonParamsInfo*) arg;
    SrcInfoParams6 *srcInfoParams = (SrcInfoParams6*) addonParamsInfo->params;

    SQLHENV henv;
    SQLHDBC hdbc;
    SQLHSTMT hstmt;

    time_t now;

#ifndef NDEBUG
    printf("Dans le thread addonODBC.\n");
#endif

    while (TRUE)
    {
        addonSleep(*addonParamsInfo->period);

        addonParamsInfo->lotNum = increaseLotNum(
                                                 addonParamsInfo->mutex,
                                                 addonParamsInfo->lotNumPtr
                                                 );

        if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv) == SQL_SUCCESS)
        {
            if (SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0) == SQL_SUCCESS)
            {
                if (SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc) == SQL_SUCCESS)
                {
                    SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER) 5, 0);
                    /* Connect to data base */
                    if (SQLDriverConnect(hdbc, NULL, srcInfoParams->connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT) == SQL_SUCCESS)
                    {
                        if (SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt) == SQL_SUCCESS)
                        {

                            AddonTypeInfo *addonTypeInfo = addonParamsInfo->addonTypeList;

                            /* What time is it ? */
                            time(&now);

                            /* Tant que l'on n'est pas au bout de la liste */
                            while (addonTypeInfo != NULL)
                            {
                                AddonTypeParamsInfo *addonTypeParamsInfo = addonTypeInfo->addonTypeParamsList;

                                /* Tant que l'on n'est pas au bout de la liste */
                                while (addonTypeParamsInfo != NULL)
                                {
                                    switch (*addonTypeInfo->idType)
                                    {
                                    case 3:
                                        addonODBCQuery(
                                                       addonParamsInfo->collectQueue,
                                                       hstmt,
                                                       addonTypeParamsInfo->params,
                                                       addonParamsInfo->lotNum,
                                                       &addonTypeParamsInfo->IDList,
                                                       &now
                                                       );
                                        break;
                                    default:
                                        g_warning("Warning: idType %d does'nt exist for the ODBC addon.", *addonTypeInfo->idType);
                                        break;
                                    }

                                    /* On avance d'une case */
                                    addonTypeParamsInfo = addonTypeParamsInfo->nxt;
                                }

                                /* On avance d'une case */
                                addonTypeInfo = addonTypeInfo->nxt;
                            }
                            SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
                        }
                        else
                            displayError("alloc handle stmt", SQL_HANDLE_DBC, hdbc);
                        SQLDisconnect(hdbc);
                    }
                    else
                        displayError("connect", SQL_HANDLE_DBC, hdbc);
                    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
                }
                else
                    displayError("alloc handle dbc", SQL_HANDLE_ENV, henv);
            }
            else
                displayError("set env attr", SQL_HANDLE_ENV, henv);
            SQLFreeHandle(SQL_HANDLE_ENV, henv);
        }
        else
            g_warning("Warning: alloc handle env failure.");

    }

    pthread_exit(NULL);
}

