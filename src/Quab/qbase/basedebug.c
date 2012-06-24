#include "basedebug.h"
#include "basedef.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifdef QBASE_OS_LINUX
#include <dir.h>
#define LOG_PATH "log/"
#else
#include <direct.h>
#define LOG_PATH "log\\"
#endif

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
	char *filename = debug_filestring();
	char *tstr = debug_timestring();
	char *logfile = NULL;
	FILE *f = NULL;

	len += strlen(LOG_PATH);
	len += strlen(filename);
#ifdef QBASE_OS_LINUX
	mkdir(LOG_PATH);
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
	printf("[%s]<%s> IN \"%s\" :%s\n", tstr, status != NULL? status: "MESSAGE", obj, msg);
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
