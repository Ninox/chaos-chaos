#include "debug/QuabLog.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef QUAB_OS_WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif
using namespace Quab;

#ifdef __cplusplus
extern "C" 
{
#endif
enum PRINT_COLOR{
	COLOR_GREEN  = 10,
	COLOR_BLUE   = 11,
	COLOR_RED    = 12,
	COLOR_YELLOW = 14
};

#ifdef QUAB_STATUS_DEBUG

/*    color printing helper    */
static void
_printColorText(const char *text, int color = 0) {
    if(color <= 0)  {
        printf(text);
    }
    else    {
#ifdef QUAB_OS_WIN32
        WORD colorOld;
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(handle, &csbi);
        colorOld = csbi.wAttributes;
        SetConsoleTextAttribute(handle, (WORD)color);
        printf(text);
        SetConsoleTextAttribute(handle, colorOld);
#else
        switch(color)
        {
        case COLOR_BLUE:
            printf("\033[34m%s\033[0m", text);
            break;
        case COLOR_GREEN:
            printf("\033[32m%s\033[0m", text);
            break;
        case COLOR_RED:
            printf("\033[31m%s\033[0m", text);
            break;
        case COLOR_YELLOW:
            printf("\033[33m%s\033[0m", text);
            break;
        default:
            return;
        }
#endif
    }
}

static void
_writeConsole(int color, const char *prefix, const char *text)	{
	_printColorText(prefix, color);
	printf(text);
	printf("\n");
}

#else
static void
_writeConsole(int color, const char *prefix, const char *text){}	
#endif

static char *
time_string()	{
	char *timestr = (char*)malloc(20);
	struct tm *timeinfo = NULL;
	time_t rawtime;	
	memset(timestr, 0, 20);
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	sprintf(
		timestr, 
		"%04d-%02d-%02d %02d:%02d:%02d",
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec
	);
	
	free(timeinfo);
	return timestr;
}

static char *
date_string()   {
	char *timeString = NULL;
	time_t rawtime;
	tm *timeinfo = NULL;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	timeString = (char*)malloc(11);
	memset(timeString, 0, 11);
	sprintf(timeString, "%04d-%02d-%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
	free(timeinfo);	// can free?	
	return timeString;
}

static char *
_getLogpath()   {
    char * dateString = date_string();
    int bufferLen = strlen(QUAB_DEFAULT_LOG_DIR) + 15;
    char *filename = (char*)malloc(bufferLen);

	// created directory
#ifdef QUAB_OS_WIN32
	_mkdir(QUAB_DEFAULT_LOG_DIR);
#else
	mkdir(QUAB_DEFAULT_LOG_DIR, S_IRWXU);
#endif	
	
    memset(filename, 0, bufferLen);
    strcpy(filename, QUAB_DEFAULT_LOG_DIR);
    strcat(filename, dateString);
    strcat(filename, ".txt");
    free(dateString);
	return filename;
}

static void 
_writeFile(char **filepath, const char *prefix, const char *msg)	{
	char *path = NULL;	
	FILE *f = NULL;
	int outputLength = 0;
	char *outputString = NULL;
	char *timeStr = time_string();
	
	assert(filepath != NULL && *filepath != NULL);
	
	path = *filepath;
	f = fopen(path, "a");
	if(f != NULL)		{
		outputLength = strlen(prefix)+strlen(msg)+ 24; // '\n' + '\0' + 'yyyy-MM-dd HH:mm:ss' + ' | '
		outputString = (char*)malloc(outputLength);
		memset(outputString, 0, outputLength);
		strcpy(outputString, timeStr);
		strcat(outputString, " | ");
		strcat(outputString, prefix);
		strcat(outputString, msg);
		
		fprintf(f, "%s\n", outputString);
		free(timeStr);
		free(outputString);
		fclose(f);
	}
	
	free(path);
	*filepath = NULL;
}

#ifdef __cplusplus
}
#endif

/*    QuabLog implements    */

void QuabLog::Log(const char *msg)	{
	char *logPath = _getLogpath();
	// print in console;
	_writeConsole(COLOR_BLUE, "[LOG]     ", msg);
	
	// write file
	_writeFile(&logPath, "[LOG]     ", msg);
}

void QuabLog::Error(const char *msg)	{
	char *logPath = _getLogpath();
	// print in console;
	_writeConsole(COLOR_RED, "[ERROR]   ", msg);
	
	// write file
	_writeFile(&logPath, "[ERROR]   ", msg);
}