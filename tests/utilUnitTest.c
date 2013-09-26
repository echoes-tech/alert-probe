#include "utilUnitTest.h"

static int stdoutdesc;

char* getOuputFilePath()
{
    return OUTPUT_FILE_PATH;
}

void readStdout_init()
{
    int fdesc;

    /* Sauvegarde le descripteur de stdout */
    stdoutdesc = dup(STDOUT_FILENO);
    /* Ouvre le fichier OUTPUT_FILE_PATH */
    fdesc = open(OUTPUT_FILE_PATH, O_CREAT | O_TRUNC | O_RDWR, 0644);
    /* Redirige la sortie standard vers FILE_PATH*/
    dup2(fdesc, STDOUT_FILENO);
    close(fdesc);
}

void readStdout_end()
{
    /* Retablis la sortie standard */
    dup2(stdoutdesc, STDOUT_FILENO);
    close(stdoutdesc);
    stdoutdesc = 0;

    /* Supprime le fichier temporaire */
    outputFile_remove();
}

int outputFile_compare(char *regex)
{
    return regex_compare(regex, outputFile_getText());
}

void outputFile_display()
{
    int fdesc;
    
    fdesc = dup(STDOUT_FILENO);
    dup2(stdoutdesc, STDOUT_FILENO);  
    
    fprintf(stdout, "\n\nOutput : \n%s\n", outputFile_getText());  
    
    dup2(fdesc, STDOUT_FILENO);
    close(fdesc);
}

char* outputFile_getText()
{
    FILE *logFile = NULL;
    char text[1024] = "", line[1024] = "";
    int match = 0;
    logFile = fopen(OUTPUT_FILE_PATH, "r");
    if (logFile != NULL)
    {
        while (fgets(line, 1024, logFile) != NULL)
        {
            strcat(text, line);
        }
        fclose(logFile);
        return text;
    }
    return NULL;
}

void outputFile_remove()
{
    remove(OUTPUT_FILE_PATH);
}

int regex_compare(char *regex, char *line)
{
    int match = 0;
    if (line != NULL) {
    regex_t preg;
    regcomp(&preg, regex, REG_NOSUB | REG_EXTENDED);
    match = !regexec(&preg, line, 0, NULL, 0);
    regfree(&preg);
    }
    return match;
}

void g_message_defineHandler(LogParams *logParams)
{
    /* Definis l'action de g_warning, g_critical et g_error
     * Marche avec log2console et log2file
     * Cf testLog2Console et testLog2File (Log CUnit Test) */
    g_log_set_handler(
                      G_LOG_DOMAIN,
                      G_LOG_LEVEL_MASK,
                      log2Console,
                      (gpointer) logParams
                      );
}
