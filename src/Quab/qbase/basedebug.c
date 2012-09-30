#include "basedebug.h"
#include "basedef.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifdef QUAB_OS_WIN32
#include <direct.h>
#include <windows.h>
#define LOG_PATH "log\\"
#else
#include <sys/stat.h>
#include <sys/types.h>
#define LOG_PATH "./log/"
#endif

enum PRINT_COLOR{
	COLOR_GREEN  = 10,
	COLOR_BLUE   = 11,
	COLOR_RED    = 12,
	COLOR_YELLOW = 14
};

/*    color printing helper    */
static void
_printColorText(const char *text, int color) {
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


static char *
debug_timestring()	{
	/*	time format: yyyy-MM-dd HH:mm:ss	*/
	char *tm_str = (char*)malloc(20);
	time_t ct;
	struct tm *tInfo = NULL;

	time(&ct);
	tInfo = localtime(&ct);
	sprintf(
		tm_str,	"%04d-%02d-%02d %02d:%02d:%02d",
		tInfo->tm_year + 1900, 1 + tInfo->tm_mon,
		tInfo->tm_mday, tInfo->tm_hour,
		tInfo->tm_min, tInfo->tm_sec
	);
//	free(tInfo);
	return tm_str;
}

static char *
debug_filestring()	{
	const char PREFIX[] = {"qLog-"};
	const char TAIL[] ={".txt"};
	char *filename = (char*)malloc(18);
	char datestr[9];
	time_t ct;
	struct tm *tInfo = NULL;

	time(&ct);
	tInfo = localtime(&ct);
	sprintf(&datestr[0], "%d%02d%02d", tInfo->tm_year+1900, 1+tInfo->tm_mon, tInfo->tm_mday);
	strcpy(filename, PREFIX);
	strcat(filename, datestr);
	strcat(filename, TAIL);

//	free(tInfo);
	return filename;
}

static void
debug_writemsg(const char * status, const char *obj, const char *msg)	{
	int len = 0;
	int color = -1;
	char *filename = debug_filestring();
	char *tstr = debug_timestring();
	char *logfile = NULL;
	FILE *f = NULL;

	len += strlen(LOG_PATH);
	len += strlen(filename);
#ifdef QUAB_OS_LINUX
	mkdir(LOG_PATH, S_IRWXU);
#else
	_mkdir(LOG_PATH);
#endif
	logfile = (char*)malloc(len+1);
	strcpy(logfile, LOG_PATH);
	strcat(logfile, filename);
	free(filename);

	f = fopen(logfile, "a");
	if(f != NULL)	{
		fprintf(f, "[%s]<%s> IN \"%s\" :%s\n", tstr, status != NULL? status: "MESSAGE", obj, msg);
		fclose(f);
	}
	free(logfile);
//	printf("[%s]<%s> IN \"%s\" :%s\n", tstr, status != NULL? status: "MESSAGE", obj, msg);
#ifdef QUAB_STATUS_DEBUG
	printf("[%s]",tstr);
	status = status != NULL? status : "MESSAGE";
	if(strcmp(status, "MESSAGE") == 0)
		color = COLOR_BLUE;
	else if(strcmp(status, "ERROR") == 0)
		color = COLOR_RED;
	else if(strcmp(status, "DEBUG") == 0)
		color = COLOR_YELLOW;
	_printColorText("<",color);
	_printColorText(status, color);
	_printColorText(">",color);
	printf(" IN \"%s\" :%s\n", obj, msg);
#endif
	free(tstr);
}



/*		implementation of the header file's functions		*/
void
qbase_debug(const char *obj, const char *msg)	{
#ifdef QBASE_DEBUG
	debug_writemsg("DEBUG", obj, msg);
#endif
}

void
qbase_log(const char *obj, const char *msg)	{
	debug_writemsg(NULL, obj, msg);
}

void
qbase_error(const char *obj, const char *msg)	{
	debug_writemsg("ERROR", obj, msg);
}
