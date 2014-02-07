/* 
 * File:   utilUnitTest.h
 * Author: mla
 *
 * Created on 25 septembre 2013, 10:18
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
