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

#ifndef UTILUNITTEST_H
#define	UTILUNITTEST_H

#define OUTPUT_FILE_PATH ".tmpout.txt"
    
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <regex.h>
#include "log.h"

/**
 * Getter d'OUTPUT_FILE_PATH
 * @return              OUTPUT_FILE_PATH
 */
char* getOuputFilePath ();

/**
 * Detourne la sortie standard vers OUTPUT_FILE_PATH
 */
void readStdout_init();

/**
 * Retablis la sortie standard
 */
void readStdout_end();

/**
 * Verifie si la regex match avec la premiere ligne du fichier OUTPUT_FILE_PATH
 * @param regex         Expression reguliere
 * @return              Booleen
 */
int outputFile_compare(char *regex);

/**
 * Affiche dans la sortie standard le contenu du fichier OUTPUT_FILE_PATH
 */
void outputFile_display();

/**
 * Retourne le contenu du fichier OUTPUT_FILE_PATH
 * @return      Contenu
 */
char* outputFile_getText();

/**
 * Supprime le fichier OUTPUT_FILE_PATH
 */
void outputFile_remove ();

/**
 * Verifie si la regex match avec la chaine
 * @param regex         Expression reguliere
 * @param line          Chaine a tester
 * @return              Booleen
 */
int regex_compare(char *regex, char *string);

/**
 * Defini l'action de g_warning, g_critical et g_error
 * @param logParams     Paramtres log
 */
void g_message_defineHandler (LogParams *logParams);

#endif	/* UTILUNITTEST_H */
