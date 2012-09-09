/* 
 * Addons List Manager
 * @author ECHOES Technologies (FPO)
 * @date 08/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "addon/addonList.h"


AddonTypeParamsInfo* isAddonTypeParamsOnList(
                                             const AddonTypeParamsList *addonTypeParamsList,
                                             const unsigned int *idAddon,
                                             const unsigned int *idType,
                                             void *params
                                             )
{
    AddonTypeParamsInfo *addonTypeParamsInfo = *addonTypeParamsList;

    /* Tant que l'on n'est pas au bout de la liste */
    while (addonTypeParamsInfo != NULL)
    {
        switch(*idAddon)
        {
        case 2:
            switch(*idType)
            {
            case 1:
            {
                SearchInfoParams2_1 *searchInfoParams = (SearchInfoParams2_1*) params;
                SearchInfoParams2_1 *searchInfoParamsTmp = (SearchInfoParams2_1*) addonTypeParamsInfo->params;
                if (!strcmp(searchInfoParams->regex, searchInfoParamsTmp->regex))
                {
                    return addonTypeParamsInfo;
                }
                break;
            }
            case 2:
            {
                SearchInfoParams2_2 *searchInfoParams = (SearchInfoParams2_2*) params;
                SearchInfoParams2_2 *searchInfoParamsTmp = (SearchInfoParams2_2*) addonTypeParamsInfo->params;
                if (
                    searchInfoParams->firstChar == searchInfoParamsTmp->firstChar
                    && searchInfoParams->length == searchInfoParamsTmp->length
                    && searchInfoParams->line == searchInfoParamsTmp->line
                    )
                {
                    return addonTypeParamsInfo;
                }
                break;
            }
            default:
                break;
            }
            break;
        case 3:
            switch(*idType)
            {
            case 1:
            {
                SearchInfoParams3_1 *searchInfoParams = (SearchInfoParams3_1*) params;
                SearchInfoParams3_1 *searchInfoParamsTmp = (SearchInfoParams3_1*) addonTypeParamsInfo->params;
                if (!strcmp(searchInfoParams->regex, searchInfoParamsTmp->regex))
                {
                    return addonTypeParamsInfo;
                }
                break;
            }
            case 2:
            {
                SearchInfoParams3_2 *searchInfoParams = (SearchInfoParams3_2*) params;
                SearchInfoParams3_2 *searchInfoParamsTmp = (SearchInfoParams3_2*) addonTypeParamsInfo->params;
                if (
                    searchInfoParams->firstChar == searchInfoParamsTmp->firstChar
                    && searchInfoParams->length == searchInfoParamsTmp->length
                    )
                {
                    return addonTypeParamsInfo;
                }
                break;
            }
            default:
                break;
            }
            break;
        default:
            break;
        }
        
        /* On avance d'une case */
        addonTypeParamsInfo = addonTypeParamsInfo->nxt;   
    }
    return NULL;
}

AddonTypeInfo* isAddonTypeOnList(const AddonTypeList *addonTypeList, const unsigned int *idType)
{
    AddonTypeInfo *addonTypeInfo = *addonTypeList;

    /* Tant que l'on n'est pas au bout de la liste */
    while (addonTypeInfo != NULL)
    {
        if (*addonTypeInfo->idType == *idType)
        {
            return addonTypeInfo;
        }
        
        /* On avance d'une case */
        addonTypeInfo = addonTypeInfo->nxt;   
    }
    return NULL;
}

AddonParamsInfo* isAddonParamsOnList(
                                     const AddonParamsList *addonParamsList,
                                     const unsigned int *idAddon,
                                     void *params,
                                     const unsigned int *period,
                                     const unsigned int *staticValues
                                     )
{
    AddonParamsInfo *addonParamsInfo = *addonParamsList;

    /* Tant que l'on n'est pas au bout de la liste */
    while (addonParamsInfo != NULL)
    {
        if (*addonParamsInfo->period == *period && *addonParamsInfo->staticValues == *staticValues)
        {
            switch(*idAddon)
            {
            case 2:
            {
                SrcInfoParams2 *srcInfoParams = (SrcInfoParams2*) params;
                SrcInfoParams2 *srcInfoParamsTmp = (SrcInfoParams2*) addonParamsInfo->params;
                if (!strcmp(srcInfoParams->path, srcInfoParamsTmp->path))
                {
                    return addonParamsInfo;
                }
                break;
            }
            case 3:
            {
                SrcInfoParams3 *srcInfoParams = (SrcInfoParams3*) params;
                SrcInfoParams3 *srcInfoParamsTmp = (SrcInfoParams3*) addonParamsInfo->params;
                if (!strcmp(srcInfoParams->path, srcInfoParamsTmp->path))
                {
                    return addonParamsInfo;
                }
                break;
            }
            default:
                break;
            }
        }
        
        /* On avance d'une case */
        addonParamsInfo = addonParamsInfo->nxt;   
    }
    return NULL;
}

AddonInfo* isAddonOnList(const AddonList *addonList, const unsigned int *idAddon)
{
    AddonInfo *addonInfo = *addonList;

    /* Tant que l'on n'est pas au bout de la liste */
    while (addonInfo != NULL)
    {
        if (*addonInfo->idAddon == *idAddon)
        {
            return addonInfo;
        }
        
        /* On avance d'une case */
        addonInfo = addonInfo->nxt;   
    }
    return NULL;
}                                                                                                             

int pushAddonList(
                  AddonList *addonList,
                  unsigned int *idAddon,
                  void *srcParams,
                  unsigned int *period,
                  unsigned int *staticValues,
                  unsigned int *idType,
                  void *searchParams,
                  unsigned int *idPlg,
                  unsigned int *idAsset,
                  unsigned int *idSrc,
                  unsigned int *idSearch
                  )
{
    AddonInfo *addonInfo = NULL, *addonInfoTmp = NULL;

    AddonParamsInfo *addonParamsInfo = NULL, *addonParamsInfoTmp = NULL;

    AddonTypeInfo *addonTypeInfo = NULL, *addonTypeInfoTmp = NULL;

    AddonTypeParamsInfo *addonTypeParamsInfo = NULL, *addonTypeParamsInfoTmp = NULL;

    IDInfo *idInfo = NULL, *idInfoTmp = NULL;

    addonInfoTmp = isAddonOnList(addonList, idAddon);
    if(addonList == NULL || addonInfoTmp == NULL){
        addonInfo = calloc(1, sizeof (AddonInfo));

        addonInfo->idAddon = idAddon;
        
        /* Assign the address of the next element in the new element */
        addonInfo->nxt = *addonList;

        /* Update the pointer of linked list */
        *addonList = addonInfo;
    }
    else
    {
        addonInfo = addonInfoTmp;
    }

    addonParamsInfoTmp = isAddonParamsOnList(&addonInfo->addonParamsList, idAddon, srcParams, period, staticValues);
    if (addonInfo->addonParamsList == NULL || addonParamsInfoTmp == NULL)
    {
        addonParamsInfo = calloc(1, sizeof (AddonParamsInfo));
        addonParamsInfo->params = srcParams;
        addonParamsInfo->period = period;
        addonParamsInfo->staticValues = staticValues;
        addonParamsInfo->lotNumPtr = NULL;
        addonParamsInfo->lotNum = 0;
        addonParamsInfo->lineNum = 0;
        addonParamsInfo->mutex = NULL;
        addonParamsInfo->collectQueue = NULL;

        /* Assign the address of the next element in the new element */
        addonParamsInfo->nxt = addonInfo->addonParamsList;

        /* Update the pointer of linked list */
        addonInfo->addonParamsList = addonParamsInfo;
    }
    else
    {
        addonParamsInfo = addonParamsInfoTmp;
    }

    addonTypeInfoTmp = isAddonTypeOnList(&addonParamsInfo->addonTypeList, idType);
    if (addonParamsInfo->addonTypeList == NULL || addonTypeInfoTmp == NULL)
    {
        addonTypeInfo = calloc(1, sizeof (AddonTypeInfo));
        addonTypeInfo->idType = idType;

        /* Assign the address of the next element in the new element */
        addonTypeInfo->nxt = addonParamsInfo->addonTypeList;

        /* Update the pointer of linked list */
        addonParamsInfo->addonTypeList = addonTypeInfo;
    }
    else
    {
        addonTypeInfo = addonTypeInfoTmp;
    }

    addonTypeParamsInfoTmp = isAddonTypeParamsOnList(&addonTypeInfo->addonTypeParamsList, idAddon, idType, searchParams);
    if(addonTypeInfo->addonTypeParamsList == NULL || addonTypeParamsInfoTmp == NULL)
    {
        addonTypeParamsInfo = calloc(1, sizeof (AddonTypeParamsInfo));
        addonTypeParamsInfo->valueNum = 1;
        addonTypeParamsInfo->params = searchParams;

        /* Assign the address of the next element in the new element */
        addonTypeParamsInfo->nxt = addonTypeInfo->addonTypeParamsList;

        /* Update the pointer of linked list */
        addonTypeInfo->addonTypeParamsList = addonTypeParamsInfo;
    }
    else
    {
        addonTypeParamsInfo = addonTypeParamsInfoTmp;
    }

    if (addonTypeParamsInfo->IDList == NULL || idInfoTmp == NULL)
    {
        idInfo = calloc(1, sizeof (IDInfo));
        idInfo->idPlg = idPlg;
        idInfo->idAsset = idAsset;
        idInfo->idSrc = idSrc;
        idInfo->idSearch = idSearch;

        /* Assign the address of the next element in the new element */
        idInfo->nxt = addonTypeParamsInfo->IDList;

        /* Update the pointer of linked list */
        addonTypeParamsInfo->IDList = idInfo; 
    }
    else
    {
        idInfo = idInfoTmp;
    }
    
    return (EXIT_SUCCESS);
}

