#ifndef QUAB_DEBUG_LOG_H
#define QUAB_DEBUG_LOG_H

#define QUAB_DEFAULT_LOG_DIR "./log/"

namespace Quab
{
    class QuabLog
    {
    public:
        QuabLog(const char *dir = QUAB_DEFAULT_LOG_DIR);

        void console(const char *msg);
        void log(const char *msg);
        void debug(void *obj);
    private:
        const char *_logdir;
    };
}
#endif // QuabLog.h
