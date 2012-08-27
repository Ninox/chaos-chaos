#ifndef QUAB_DEBUG_LOG_H
#define QUAB_DEBUG_LOG_H

namespace Quab
{
    class QuabLog
    {
    public:
        QuabLog();
        QuabLog(const char *dir);

        void console(const char *msg);
        void log(const char *msg);
        void debug(void *obj);
    private:
        const char *_logdir;
    };
}
#endif // QuabLog.h
