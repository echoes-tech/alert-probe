/* 
 * Search Functions
 * @author ECHOES Technologies (FPO)
 * @date 08/11/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS
 * WITHOUT COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "search.h"

gushort searchRegex(
                    char **values,
                    const char *line,
                    int *err,
                    regex_t *preg,
                    size_t *nmatch,
                    regmatch_t *pmatch
                    )
{
    char *res = NULL;

    int match = -1;

    gushort i = 0;

    if (pmatch)
    {
        /* TODO: Comprendre le comportement étrange avec \d */
        match = regexec(
                        preg,
                        line,
                        *nmatch,
                        pmatch,
                        0
                        );
        if (match == 0)
        {
            for (i = 1; i < *nmatch; ++i)
            {
                int start = pmatch[i].rm_so;
                int end = pmatch[i].rm_eo;
                size_t size = end - start;
                
                res = malloc(sizeof (*res) * (size  + 1));
                if (res)
                {
                    strncpy(res, &line[start], size);
                    res[size] = '\0';

                    values[i - 1] = strdup(res);

                    /* Cleanup */
                    free(res);
                    res = NULL;
                }
            }
        }
        else if (match == REG_NOMATCH)
        {
            return EXIT_FAILURE;
        }
        else
        {
            char *text;
            size_t size;

            size = regerror(*err, preg, NULL, 0);
            text = malloc(sizeof (*text) * size);
            if (text)
            {
                regerror(*err, preg, text, size);
                g_warning("Critical: %s", text);
                free(text);
                text = NULL;
            }
            else
                g_warning("Critical: Insufficient memory");

            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

gushort searchLocation(
                       char **values,
                       const char *line,
                       const unsigned int *length,
                       const unsigned int *firstChar
                       )
{
    char *res = NULL;
    
    res = calloc(*length, sizeof (char));
    if (res)
    {
        strncpy(
                res,
                &line[*firstChar - 1],
                *length
                );

        values[0] = strdup(res);

        /* Cleanup */
        free(res);
        res = NULL;
    }
    else
    {
        g_critical("Critical: %s", strerror(errno));
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

