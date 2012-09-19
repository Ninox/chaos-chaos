#include "debug/QuabLog.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef QUAB_OS_WIN32
#include <windows.h>
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
	return NULL;
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
	sprintf(timeString, "%4d-%02d-%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
	free(timeinfo);	// can free?	
	return timeString;
}

static char *
log_getname()   {
    char * dateString = date_string();
    int bufferLen = strlen(QUAB_DEFAULT_LOG_DIR) + 15;
    char *filename = (char*)malloc(bufferLen);
    memset(filename, 0, bufferLen);
    strcpy(filename, QUAB_DEFAULT_LOG_DIR);
    strcat(filename, dateString);
    strcat(filename, ".txt");
    free(dateString);
	return filename;
}

static char * 
_getLogpath()	{
	char *filename = log_getname();
	int logLen = strlen(QUAB_DEFAULT_LOG_DIR) + strlen(filename) + 1;
	char *logpath = (char*)malloc(logLen);
	memset(logpath, 0, logLen);
	strcpy(logpath, QUAB_DEFAULT_LOG_DIR);
	strcat(logpath, filename);
	free(filename);
	return logpath;
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
		
		fwrite(outputString, outputLength, 1, f);
		free(outputString);
	}
	
	free(timeStr);
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

void QuabHello::Hello(const char *hello)	{
	std::cout<<"Hello world!"<<std::ends<<hello<<std::endl;
}