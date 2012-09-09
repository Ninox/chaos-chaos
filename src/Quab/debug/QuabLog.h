#ifndef QUAB_DEBUG_LOG_H
#define QUAB_DEBUG_LOG_H

#define QUAB_DEFAULT_LOG_DIR "./log/"

struct logQueue;

namespace Quab
{
    class QuabLog;

    class QuabDebug
    {
    public:
        QuabDebug(const char *dir = QUAB_DEFAULT_LOG_DIR);
        ~QuabDebug();

        void message(const char *msg);
        void log(const char *msg);
        void debug(void *obj);
        void error(const char *msg);
    private:
        QuabLog *logger;
        const char *_logdir;
    };
}
#endif // QuabLog.h
