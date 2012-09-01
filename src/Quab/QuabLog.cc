#include "debug/QuabLog.h"
#include <map>
#include <assert.h>
#include <stdio.h>

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

namespace Quab
{
    /*    the log struct definitions  */
    struct logQueue {
        int color;
        const char *text;
        logQueue *next;
    };

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

    public:
        unsigned refCount;

        static QuabLog* create(const char *dir) {

        }

        void message(const char *msg)   {
        }

        void log(const char *msg)   {

        }

        void debug(const char *msg) {

        }

        void error(const char *msg) {

        }
    };
}

/*    the implementions of QuabDebug    */
QuabDebug::QuabDebug(const char *dir)   {
    assert(dir);
    if(_logger_map.count(dir) > 0)
}

void QuabDebug::_writefile(const char *text)  {

}

void QuabDebug::console(const char *msg)  {
}
void QuabDebug::log(const char *msg)  {
}
void QuabDebug::debug(void *obj)  {
}
