#ifndef __SMSGD_COMMON_H__
#define __SMSGD_COMMON_H__

#include <stdarg.h>
#include <Poco/Logger.h>

#define SMSGD_DEFAULT_SERIAL_PORT       "/dev/ttyACM0"
#define SMSGD_DEFAULT_UDP_PORT          4000
#define SMSGD_DEFAULT_LOG_PATH          "/var/log/smsgd.log"

#define DEFAULT_LOGGER                  (Poco::Logger::root())          

inline static void writelog(Poco::Message::Priority level, const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    char buf[4096];
    vsnprintf(buf, sizeof(buf), fmt, vl);
    buf[sizeof(buf) - 1] = 0;
    Poco::Message msg("smsgd", buf, level);
    DEFAULT_LOGGER.log(msg);
    va_end(vl);
}

#define logdebug(fmt...)  writelog(Poco::Message::PRIO_DEBUG, fmt)
#define logtrace(fmt...)  writelog(Poco::Message::PRIO_TRACE, fmt)
#define logerror(fmt...)  writelog(Poco::Message::PRIO_ERROR, fmt)
#define loginfo(fmt...)   writelog(Poco::Message::PRIO_INFORMATION, fmt)

#endif // SMSGD_COMMON_H__
