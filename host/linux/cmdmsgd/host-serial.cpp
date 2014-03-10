#include "host-serial.h"

#define NEWLINE "\r\n"

Stream::~Stream() {

}

Stream::Stream() {

}

int Stream::print(const char *data) {
    int pos = 0;
    while (*(data + pos)) {
        print(*(data + pos));
        ++pos;
    }
    return pos;
}

int Stream::println() {
    return print(NEWLINE);
}

size_t Stream::readBytes(char *buffer, size_t length) {
    while (length--) {
        *(buffer++) = read();
    }
    return 0;
}

