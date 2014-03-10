#ifndef __HOST_SERIAL_H__
#define __HOST_SERIAL_H__

#include "stdint.h"
#include <string.h>
#include <sys/time.h>

#define min(a,b) ((a)<(b)?(a):(b))

// todo - hehe, notice here if you are using string arg
#define strlcpy strncpy

typedef uint8_t byte;

inline static unsigned long millis()
{
    struct timeval ts;
    gettimeofday(&ts, NULL);
    return ts.tv_sec * 1000 + ts.tv_usec / 1000;
}

class Stream{
public:
    Stream();
    virtual ~Stream();

    // stream write for adruino side
    virtual int print(byte data) = 0;
    int print(const char *data);
    int println();

    // stream read for arduino side
    virtual size_t available() = 0;
    virtual byte read() = 0;
    size_t readBytes(char *buffer, size_t length);
};

#endif // HOST_SERIAL_H__
