/* 
 * Alert - Probe is a collect probe, part of the Alert software
 * Copyright (C) 2013 Florent Poinsaut, Thomas Saquet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#ifndef CONF_H
#define	CONF_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "log.h"

/* Max size of file conf lines in caracters */
#define MAX_SIZE 300

/* Probe Configurations */
typedef struct Conf Conf;
struct Conf
{
    unsigned int probeID;
    unsigned short enginePort;
    unsigned char transportProto, transportMsgVersion;
    char probePluginDir[255], engineFQDN[255], token[25];
};

/* Probe Configuration Initializer */
#define CONF_INITIALIZER {0, 0, 0, 0, "", "", ""}

/**
 * Parse line from Configuration file
 * @param *conf Pointer of Configurations
 * @param *line Line of configuration file
 */
void parseLineConf(Conf *conf, const char *line);

/**
 * Load probe configuration file
 * @param *conf     Pointer of Configurations
 * @param *confPath Configuration file path
 * @return Exit status
 */
int loadConf(Conf *conf, const char *confPath);

#endif	/* CONF_H */

