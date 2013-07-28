/* 
 * Header of Sender Module
 * @author ECHOES Technologies (FPO)
 * @date 14/02/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef SEARCH_H
#define	SEARCH_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "addon/addon.h"

/**
 * Search Regex
 * @param values
 * @param line
 * @param err
 * @param match
 * @param preg
 * @param nmatch
 * @param pmatch
 * @return Exit status
 */
gushort searchRegex(
                    char **values,
                    const char *line,
                    int *err,
                    regex_t *preg,
                    size_t *nmatch,
                    regmatch_t *pmatch
                    );

/**
 * Search Location
 * @param values    
 * @param line
 * @param length
 * @param firstChar 
 * @return Exit status
 */
gushort searchLocation(
                       char **values,
                       const char *line,
                       const unsigned int *length,
                       const unsigned int *firstChar
                       );

#ifdef	__cplusplus
}
#endif

#endif	/* SEARCH_H */

