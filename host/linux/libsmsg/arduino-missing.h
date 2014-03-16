#ifndef __ARDUINO_MISSING_H__
#define __ARDUINO_MISSING_H__

#ifdef ARDUINO
#include <stdlib.h>
inline static void *memcpy(void *dest, const void *src, size_t count)
{
    char *tmp = (char *)dest;
    const char *s = (const char *)src;

    while (count--)
        *tmp++ = *s++;
    return dest;
}

inline static void *memset(void *s, int c, size_t count)
{
    char *xs = (char *)s;

    while (count--)
        *xs++ = c;
    return s;
}
#endif



#endif // __ARDUINO_MISSING_H__
