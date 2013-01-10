/* 
 * Addon SNMP
 * @author ECHOES Technologies (FPO)
 * @date 07/01/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
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
                 unsigned int resNum,
                 unsigned short lotNum,
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
                         resNum,
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
                   unsigned int resNum,
                   void *params,
                   unsigned short lotNum,
                   IDList *idList,
                   time_t *now
                   )
{
    SearchInfoParams5_2 *searchInfoParams = (SearchInfoParams5_2*)params;

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
            return EXIT_FAILURE;
        }

        if (pushCollectQueue(
                             collectQueue,
                             idList,
                             lotNum,
                             resNum,
                             (searchInfoParams->nmatch - 1),
                             values,
                             *now
                             ))
        {
            /* Cleanup */
            free(values);
            return EXIT_FAILURE;
        }

        /* Cleanup */
        free(values);
    }
    else
    {
        g_warning("Critical: Insufficient memory");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int addonSNMPLocation(
                      CollectQueue *collectQueue,
                      const char *res,
                      unsigned int resNum,
                      void *params,
                      unsigned short lotNum,
                      IDList *idList,
                      time_t *now
                      )
{
    SearchInfoParams5_3 *searchInfoParams = (SearchInfoParams5_3*)params;

    char *values[1];

    if (searchLocation(
                       values,
                       res,
                       &searchInfoParams->length,
                       &searchInfoParams->firstChar
                       ))
        return EXIT_FAILURE;

    if (pushCollectQueue(
                         collectQueue,
                         idList,
                         lotNum,
                         resNum,
                         1,
                         values,
                         *now
                         ))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void *addonSNMP(void *arg)
{
    AddonParamsInfo *addonParamsInfo = (AddonParamsInfo*) arg;
    SrcInfoParams5 *srcInfoParams = (SrcInfoParams5*)addonParamsInfo->params;

    time_t now, temp;

    netsnmp_session session, *ss;
    netsnmp_pdu *pdu;
    netsnmp_pdu *response;

    oid OID[MAX_OID_LEN];
    size_t OID_len;

    netsnmp_variable_list *vars;

    char *errstr;
    int liberr, syserr;
    
    int status;
    unsigned int n = 1;

#ifndef NDEBUG
    printf("Dans le thread addonSNMP.\n");
#endif

    /* What time is it ? */
    time(&now);
    
    /* Method to know when start the loop */
    temp =  ((int)(now / *addonParamsInfo->period) * *addonParamsInfo->period) + *addonParamsInfo->period;
    
    /* Diff between now and the start of the loop */
    SLEEP(difftime(temp, now));
    while(1)
    {
        AddonTypeInfo *addonTypeInfo = addonParamsInfo->addonTypeList;

        addonParamsInfo->lotNum = increaseLotNum(
                                                 addonParamsInfo->mutex,
                                                 addonParamsInfo->lotNumPtr
                                                 );

        /*
         * Initialize the SNMP library
         */
        init_snmp("addonsnmp");

        /*
         * Initialize a "session" that defines who we're going to talk to
         */
        snmp_sess_init( &session );                   /* set up defaults */
        session.peername = strdup(srcInfoParams->host);
        
        /* set up the authentication parameters for talking to the server */

        if (srcInfoParams->version == 3)
        {
            /* Use SNMPv3 to talk to the experimental server */

            /* set the SNMP version number */
            session.version=SNMP_VERSION_3;

            /* set the SNMPv3 user name */
            session.securityName = strdup(srcInfoParams->user);
            session.securityNameLen = strlen(session.securityName);

            /* set the security level to authenticated, but not encrypted */
            session.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;

            /* set the authentication method */
            if(!strcmp(srcInfoParams->authProto, "MD5"))
            {
                session.securityAuthProto = usmHMACMD5AuthProtocol;
                session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
            }
            else if (!strcmp(srcInfoParams->authProto, "SHA1"))
            {
                session.securityAuthProto = usmHMACSHA1AuthProtocol;
                session.securityAuthProtoLen = sizeof(usmHMACSHA1AuthProtocol)/sizeof(oid);
            }
            else
            {
                g_warning("Warning: Addon SNMP: Unknown auth protocol: %s (MD5 | SHA1).", srcInfoParams->authProto);
                pthread_exit(NULL);
            }
            session.securityAuthKeyLen = USM_AUTH_KU_LEN;

            /* set the authentication key to a hashed version of our passphrase */
            if (generate_Ku(session.securityAuthProto,
                            session.securityAuthProtoLen,
                            (u_char *) srcInfoParams->authPass, strlen(srcInfoParams->authPass),
                            session.securityAuthKey,
                            &session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
                g_warning("Warning: Addon SNMP: Error generating Ku from authentication pass phrase.");

                pthread_exit(NULL);
            }

            /* set the encryption method */
            if(!strcmp(srcInfoParams->privProto, "DES"))
            {
                session.securityPrivProto = usmDESPrivProtocol;
                session.securityPrivProtoLen = sizeof(usmDESPrivProtocol)/sizeof(oid);
            }
            else if(!strcmp(srcInfoParams->privProto, "AES"))
            {
                session.securityPrivProto = usmAESPrivProtocol;
                session.securityPrivProtoLen = sizeof(usmAESPrivProtocol)/sizeof(oid);
            }
            else if(!strcmp(srcInfoParams->privProto, "AES128"))
            {
                session.securityPrivProto = usmAES128PrivProtocol;
                session.securityPrivProtoLen = sizeof(usmAES128PrivProtocol)/sizeof(oid);
            }
            else
            {
                g_warning("Warning: Addon SNMP: Unknown priv protocol: %s (DES | AES | AES128).", srcInfoParams->privProto);
                pthread_exit(NULL);
            }
            session.securityPrivKeyLen = USM_PRIV_KU_LEN;

            /* set the encryption key to hashed version of our passphrase */
            if (generate_Ku(session.securityAuthProto,
                            session.securityAuthProtoLen,
                            (u_char *) srcInfoParams->privPass, strlen(srcInfoParams->privPass),
                            session.securityPrivKey,
                            &session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
                g_warning("Warning: Addon SNMP: Error generating Ku from encryption pass phrase.");

                pthread_exit(NULL);
            }
        }
        else
        {
            /* we'll use the insecure (but simplier) SNMPv1 */

            /* set the SNMP version number */
            session.version = SNMP_VERSION_1;

            /* set the SNMPv1 community name used for authentication */
            session.community = (u_char *)srcInfoParams->community;
            session.community_len = strlen((char *)session.community);
        }

        /*
         * Open the session
         */
        SOCK_STARTUP;
        ss = snmp_open(&session);                     /* establish the session */

        if (!ss)
        {
            snmp_error(&session, &liberr, &syserr, &errstr);

            g_warning("Warning: Addon SNMP: %s.", errstr);

            free(errstr);
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
                case 1:
                {
                    SearchInfoParams5_1 *searchInfoParams = (SearchInfoParams5_1*)addonTypeParamsInfo->params;
                    
                    if (!snmp_parse_oid(searchInfoParams->oid, OID, &OID_len))
                    {
                        snmp_error(ss, &liberr, &syserr, &errstr);
                        g_warning("Warning: Addon SNMP: %s: %s.", searchInfoParams->oid, errstr);
                        free(errstr);
                        SOCK_CLEANUP;
                        SLEEP(*addonParamsInfo->period);
                        continue;
                    }

                    snmp_add_null_var(pdu, OID, OID_len);

                    break;
                }
                case 2:
                {
                    SearchInfoParams5_2 *searchInfoParams = (SearchInfoParams5_2*)addonTypeParamsInfo->params;

                    if (!snmp_parse_oid(searchInfoParams->oid, OID, &OID_len))
                    {
                        snmp_error(ss, &liberr, &syserr, &errstr);
                        g_warning("Warning: Addon SNMP: %s: %s.", searchInfoParams->oid, errstr);
                        free(errstr);
                        SOCK_CLEANUP;
                        SLEEP(*addonParamsInfo->period);
                        continue;
                    }

                    snmp_add_null_var(pdu, OID, OID_len);

                    break;
                }
                case 3:
                {
                    SearchInfoParams5_3 *searchInfoParams = (SearchInfoParams5_3*)addonTypeParamsInfo->params;

                    if (!snmp_parse_oid(searchInfoParams->oid, OID, &OID_len))
                    {
                        snmp_error(ss, &liberr, &syserr, &errstr);
                        g_warning("Warning: Addon SNMP: %s: %s.", searchInfoParams->oid, errstr);
                        free(errstr);
                        SOCK_CLEANUP;
                        SLEEP(*addonParamsInfo->period);
                        continue;
                    }

                    snmp_add_null_var(pdu, OID, OID_len);

                    break;
                }
                default:
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
        status = snmp_synch_response(ss, pdu, &response);

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
                     free(tmpRes);

                    switch (*addonTypeInfo->idType)
                    {
                    case 1:
                        addonSNMPAll(
                                       addonParamsInfo->collectQueue,
                                       res,
                                       n++,
                                       addonParamsInfo->lotNum,
                                       &addonTypeParamsInfo->IDList,
                                       &now
                                       );
                        break;
                    case 2:
                        addonSNMPRegex(
                                       addonParamsInfo->collectQueue,
                                       res,
                                       n++,
                                       addonTypeParamsInfo->params,
                                       addonParamsInfo->lotNum,
                                       &addonTypeParamsInfo->IDList,
                                       &now
                                       );
                        break;
                    case 3:
                        addonSNMPLocation(
                                          addonParamsInfo->collectQueue,
                                          res,
                                          n++,
                                          addonTypeParamsInfo->params,
                                          addonParamsInfo->lotNum,
                                          &addonTypeParamsInfo->IDList,
                                          &now
                                          );
                        break;
                    default:
                        break;
                    }

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
                g_warning("Warning: Addon SNMP: Timeout: No response from %s.", session.peername);
            else
            {
                snmp_error(ss, NULL, NULL, &errstr);
                g_warning("Warning: Addon SNMP: %s.", errstr);
                free(errstr);
            }
        }

        /*
         * Clean up:
         *  1) free the response.
         *  2) close the session.
         */
        if (response)
            snmp_free_pdu(response);
        snmp_close(ss);

        SOCK_CLEANUP;

        SLEEP(*addonParamsInfo->period);
    }

    pthread_exit(NULL);
}
