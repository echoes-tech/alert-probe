/* 
 * Addon SNMP
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

#include "addon/addonSNMP.h"

char* typeCut(const char* tmpValue)
{
    /* find the first space */
    char *tmp = strpbrk(tmpValue, " ");
    
    return &tmp[1];
}

int addonSNMPAll(
                 CollectQueue *collectQueue,
                 const char *res,
                 const unsigned short lotNum,
                 IDList *idList,
                 time_t *now
                 )
{
    char *values[1];
    
    values[0] = strdup(res);

    if (pushCollectQueue(
                         collectQueue,
                         idList,
                         lotNum,
                         1,
                         1,
                         values,
                         *now
                         ))
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

int addonSNMPRegex(
                   CollectQueue *collectQueue,
                   const char *res,
                   void *params,
                   const unsigned short lotNum,
                   IDList *idList,
                   time_t *now
                   )
{
    SearchInfoParams5_4 *searchInfoParams = (SearchInfoParams5_4*)params;

    char **values = NULL;

    values = calloc(searchInfoParams->nmatch, sizeof (char*));

    if(values)
    {
        if (searchRegex(
                        values,
                        res,
                        &searchInfoParams->err,
                        &searchInfoParams->preg,
                        &searchInfoParams->nmatch,
                        searchInfoParams->pmatch
                        ))
        {
            /* Cleanup */
            free(values);
            values = NULL;
            return EXIT_FAILURE;
        }

        if (pushCollectQueue(
                             collectQueue,
                             idList,
                             lotNum,
                             1,
                             (searchInfoParams->nmatch - 1),
                             values,
                             *now
                             ))
        {
            /* Cleanup */
            free(values);
            values = NULL;
            return EXIT_FAILURE;
        }

        /* Cleanup */
        free(values);
        values = NULL;
    }
    else
    {
        g_warning("Critical: Insufficient memory");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void *addonSNMP(void *arg)
{
    AddonParamsInfo *addonParamsInfo = (AddonParamsInfo*) arg;
    SrcInfoParams5 *srcInfoParams = (SrcInfoParams5*)addonParamsInfo->params;

    time_t now;

    netsnmp_session Session;
    void *sessp;  /* <-- an opaque pointer, not a struct pointer */
    netsnmp_pdu *pdu = NULL, *response = NULL;

    oid OID[MAX_OID_LEN];
    size_t OID_len;

    netsnmp_variable_list *vars = NULL;

    char *errstr = NULL;
    int liberr = 0, syserr = 0;
    
    int status = STAT_ERROR;

#ifndef NDEBUG
    printf("Dans le thread addonSNMP.\n");
#endif

    while(TRUE)
    {        
        AddonTypeInfo *addonTypeInfo = addonParamsInfo->addonTypeList;

        addonSleep(*addonParamsInfo->period);
        
        addonParamsInfo->lotNum = increaseLotNum(
                                                 addonParamsInfo->mutex,
                                                 addonParamsInfo->lotNumPtr
                                                 );

        /*
         * Initialize a "session" that defines who we're going to talk to
         */
        snmp_sess_init(&Session); /* set up defaults */
        Session.peername = srcInfoParams->host;
        
        /* set up the authentication parameters for talking to the server */

        /* SNMPv1 or SNMPv2c*/
        if (strcmp(srcInfoParams->version, "1") == 0 || strcmp(srcInfoParams->version, "2c") == 0)
        {
            /* set the SNMP version number */
            if (strcmp(srcInfoParams->version, "1") == 0 )
                Session.version = SNMP_VERSION_1;
            else
                Session.version = SNMP_VERSION_2c;

            /* set the SNMPv1 community name used for authentication */
            Session.community = (u_char *)srcInfoParams->community;
            Session.community_len = strlen((char *)Session.community);
        }
        /* SNMPv3 */
        else if (strcmp(srcInfoParams->version, "3") == 0)
        {
            /* set the SNMP version number */
            Session.version=SNMP_VERSION_3;

            /* set the SNMPv3 user name */
            Session.securityName = srcInfoParams->user;
            Session.securityNameLen = strlen(Session.securityName);

            /* set the security level to authenticated, but not encrypted */
            Session.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;

            /* set the authentication method */
            if(!strcmp(srcInfoParams->authProto, "MD5"))
            {
                Session.securityAuthProto = usmHMACMD5AuthProtocol;
                Session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
            }
            else if (!strcmp(srcInfoParams->authProto, "SHA"))
            {
                Session.securityAuthProto = usmHMACSHA1AuthProtocol;
                Session.securityAuthProtoLen = sizeof(usmHMACSHA1AuthProtocol)/sizeof(oid);
            }
            else
            {
                g_warning("Warning: Addon SNMP: Unknown auth protocol: %s (MD5 | SHA).", srcInfoParams->authProto);
                pthread_exit(NULL);
            }
            Session.securityAuthKeyLen = USM_AUTH_KU_LEN;

            /* set the authentication key to a hashed version of our passphrase */
            if (generate_Ku(Session.securityAuthProto,
                            Session.securityAuthProtoLen,
                            (u_char *) srcInfoParams->authPass, strlen(srcInfoParams->authPass),
                            Session.securityAuthKey,
                            &Session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
                g_warning("Warning: Addon SNMP: Error generating Ku from authentication pass phrase.");

                pthread_exit(NULL);
            }

            /* set the encryption method */
            if(!strcmp(srcInfoParams->privProto, "DES"))
            {
                Session.securityPrivProto = usmDESPrivProtocol;
                Session.securityPrivProtoLen = sizeof(usmDESPrivProtocol)/sizeof(oid);
            }
            else if(!strcmp(srcInfoParams->privProto, "AES"))
            {
                Session.securityPrivProto = usmAESPrivProtocol;
                Session.securityPrivProtoLen = sizeof(usmAESPrivProtocol)/sizeof(oid);
            }
            else if(!strcmp(srcInfoParams->privProto, "AES128"))
            {
                Session.securityPrivProto = usmAES128PrivProtocol;
                Session.securityPrivProtoLen = sizeof(usmAES128PrivProtocol)/sizeof(oid);
            }
            else
            {
                g_warning("Warning: Addon SNMP: Unknown priv protocol: %s (DES | AES | AES128).", srcInfoParams->privProto);
                pthread_exit(NULL);
            }
            Session.securityPrivKeyLen = USM_PRIV_KU_LEN;

            /* set the encryption key to hashed version of our passphrase */
            if (generate_Ku(Session.securityAuthProto,
                            Session.securityAuthProtoLen,
                            (u_char *) srcInfoParams->privPass, strlen(srcInfoParams->privPass),
                            Session.securityPrivKey,
                            &Session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
                g_warning("Warning: Addon SNMP: Error generating Ku from encryption pass phrase.");

                pthread_exit(NULL);
            }
        }
        else
        {
            g_warning("Warning: Addon SNMP: Unknown version: %s (1 | 2c | 3).", srcInfoParams->version);
            pthread_exit(NULL);
        }

        /*
         * Open the session
         */
        sessp = snmp_sess_open(&Session); /* establish the session */
        if (sessp == NULL)
        {
            /* Error codes found in open calling argument */
            snmp_error(&Session, &liberr, &syserr, &errstr);

            g_warning("Warning: Addon SNMP: %s.", errstr);

            free(errstr);
            errstr = NULL;

            SOCK_CLEANUP;
            
            SLEEP(*addonParamsInfo->period);
            continue;
        }

        /*
         * Create the PDU for the data for our request.
         */
        pdu = snmp_pdu_create(SNMP_MSG_GET);
        OID_len = MAX_OID_LEN;
        
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
                case 4:
                {
                    SearchInfoParams5_4 *searchInfoParams = (SearchInfoParams5_4*)addonTypeParamsInfo->params;

                    if (!snmp_parse_oid(searchInfoParams->oid, OID, &OID_len))
                    {
                        snmp_sess_error(sessp, &liberr, &syserr, &errstr);
                        g_warning("Warning: Addon SNMP: %s: %s.", searchInfoParams->oid, errstr);
                        free(errstr);
                        errstr = NULL;

                        SOCK_CLEANUP;
                        SLEEP(*addonParamsInfo->period);
                        continue;
                    }

                    snmp_add_null_var(pdu, OID, OID_len);

                    break;
                }
                case 5:
                {
                    SearchInfoParams5_5 *searchInfoParams = (SearchInfoParams5_5*)addonTypeParamsInfo->params;
                    
                    if (!snmp_parse_oid(searchInfoParams->oid, OID, &OID_len))
                    {
                        snmp_sess_error(sessp, &liberr, &syserr, &errstr);
                        g_warning("Warning: Addon SNMP: %s: %s.", searchInfoParams->oid, errstr);
                        free(errstr);
                        errstr = NULL;

                        SOCK_CLEANUP;
                        SLEEP(*addonParamsInfo->period);
                        continue;
                    }

                    snmp_add_null_var(pdu, OID, OID_len);

                    break;
                }
                default:
                    g_warning("Warning: idType %d does'nt exist for the SNMP addon.", *addonTypeInfo->idType);
                    break;
                }

                /* On avance d'une case */
                addonTypeParamsInfo = addonTypeParamsInfo->nxt;
            }

            /* On avance d'une case */
            addonTypeInfo = addonTypeInfo->nxt;
        }

        /*
         * Send the Request out.
         */
        status = snmp_sess_synch_response(sessp, pdu, &response);

        /*
        * Process the response.
        */
       if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR)
       {
            addonTypeInfo = addonParamsInfo->addonTypeList;

            /* Tant que l'on n'est pas au bout de la liste */
            while (addonTypeInfo != NULL)
            {
                AddonTypeParamsInfo *addonTypeParamsInfo = addonTypeInfo->addonTypeParamsList;
                vars = response->variables;

                /* Tant que l'on n'est pas au bout de la liste */
                while (addonTypeParamsInfo != NULL)
                {
                    /* Retrieve the value */
                    size_t size = 10;
                    char *res, *tmpRes = calloc(1, sizeof (char*) * size);

                    while (snprint_value(tmpRes, size, vars->name, vars->name_length, vars) == -1 && size < 550)
                    {
                        size += 10;
                        tmpRes = realloc(tmpRes, size);
                    }
                    res = typeCut(tmpRes);

                    switch (*addonTypeInfo->idType)
                    {
                    case 4:
                        addonSNMPRegex(
                                       addonParamsInfo->collectQueue,
                                       res,
                                       addonTypeParamsInfo->params,
                                       addonParamsInfo->lotNum,
                                       &addonTypeParamsInfo->IDList,
                                       &now
                                       );
                        break;
                    case 5:
                        addonSNMPAll(
                                     addonParamsInfo->collectQueue,
                                     res,
                                     addonParamsInfo->lotNum,
                                     &addonTypeParamsInfo->IDList,
                                     &now
                                     );
                        break;
                    default:
                        g_warning("Warning: idType %d does'nt exist for the SNMP addon.", *addonTypeInfo->idType);
                        break;
                    }

                    free(tmpRes);
                    tmpRes = NULL;

                    /* On avance d'une case */
                    addonTypeParamsInfo = addonTypeParamsInfo->nxt;
                    vars = vars->next_variable;
                }

                /* On avance d'une case */
                addonTypeInfo = addonTypeInfo->nxt;
            }
            
        }
        else
        {
            /* FAILURE: log what went wrong! */
            if (status == STAT_SUCCESS)
                g_warning("Warning: Addon SNMP: Error in packet\nReason: %s.", snmp_errstring(response->errstat));
            else if (status == STAT_TIMEOUT)
                g_warning("Warning: Addon SNMP: Timeout: No response from %s.", Session.peername);
            else
            {
                snmp_sess_error(sessp, &liberr, &syserr, &errstr);
                g_warning("Warning: Addon SNMP: %s.", errstr);
                free(errstr);
                errstr = NULL;
            }
        }

        /*
         * Clean up:
         *  1) free the response.
         *  2) close the session.
         */
        if (response)
            snmp_free_pdu(response);
        snmp_sess_close(sessp);
    }
    
#ifndef NDEBUG
    printf("Fin du thread addonSNMP.\n");
#endif
    
    pthread_exit(NULL);
}

