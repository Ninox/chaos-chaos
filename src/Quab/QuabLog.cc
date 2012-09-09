#include "debug/QuabLog.h"
#include <map>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <boost/thread/thread.hpp>

#ifdef QUAB_OS_WIN32
#include <windows.h>
#endif
using namespace Quab;

enum PRINT_COLOR{
	COLOR_GREEN  = 10,
	COLOR_BLUE   = 11,
	COLOR_RED    = 12,
	COLOR_YELLOW = 14
};

/* global logger map  */
static std::map<const char*, QuabLog*>
_logger_map;

/*    the log struct definitions  */
struct logQueue {
    int color;
    bool isWrite;
    char timetrap[20];
    const char *msgtype;
    const char *text;
    logQueue *next;
    logQueue *prev;
};

/*    color printing helper    */
static void
printColorText(const char *text, int color = 0) {
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

/*    helper function's for logQueue    */
static void
lq_add(logQueue *lq, int color, const char *type, const char *msg, bool isWrite = true)    {
    logQueue *que = (logQueue*)malloc(sizeof(logQueue));
    que->color = color;
    que->isWrite = isWrite;
    que->msgtype = type;
    que->text = msg;
    que->next = NULL;
    que->prev = NULL;

    if(lq == NULL)  {
        lq = que;
    }
    else    {
        lq->prev->next = que;
        que->prev = lq->prev;
        que->next = lq;
        lq->prev = que;
    }
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
}

namespace Quab
{
    /*    the QuabLog class definitions    */
    class QuabLog
    {
    private:
        const char *logdir;
        logQueue *_logqueue;

        QuabLog():_logqueue(NULL), refCount(1) {}
        QuabLog(const char *dir):_logqueue(NULL), refCount(1)    {
            assert(dir != NULL);
            logdir = dir;
        }

        void _writefile(const char *text)   {
            char *filename = log_getname();
            FILE *f = fopen(filename, "a");
            free(filename);

            if(f == NULL)   {
                printf("cannot write log in local file\n");
                return;
            }
            fwrite(text, sizeof(char), strlen(text), f);
            fclose(f);
        }

        void _start()   {
        }
        void _stop() {
        }

    public:
        unsigned refCount;

        static QuabLog* create(const char *dir) {
            QuabLog *log = NULL;
            if(_logger_map.count(dir) > 0)  {
                log = _logger_map[dir];
                log->refCount += 1;
                return log;
            }
            else  {
                log = new QuabLog(dir);
                _logger_map[dir] = log;
                return log;
            }
        }

        void message(const char *msg)   {
            lq_add(_logqueue, COLOR_BLUE, "[MESSAGE] ", msg, false);
        }

        void log(const char *msg)   {
            lq_add(_logqueue, COLOR_GREEN, "[LOG]     ", msg);
        }

        void debug(const char *msg) {
            lq_add(_logqueue, COLOR_YELLOW, "[DEBUG]   ",msg);
        }

        void error(const char *msg) {
            lq_add(_logqueue, COLOR_RED, "[ERROR]   ", msg);
        }
    };
}

/*    the implementions of QuabDebug    */
QuabDebug::QuabDebug(const char *dir)   {
    assert(dir);
    this->logger = QuabLog::create(dir);
    this->_logdir = dir;
}

QuabDebug::~QuabDebug() {
    if(this->logger != NULL && logger->refCount == 1)   {
        delete logger;
        logger = NULL;
        _logger_map.erase(this->_logdir);
    }
}

void QuabDebug::message(const char *msg)  {
    this->logger->message(msg);
}
void QuabDebug::log(const char *msg)  {
    this->logger->log(msg);
}
void QuabDebug::debug(void *obj)  {
}
void QuabDebug::error(const char *msg)  {
    this->logger->error(msg);
}
