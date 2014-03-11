#ifndef __HOST_SERIAL_H__
#define __HOST_SERIAL_H__

#include <deque>
#include <algorithm>
#include "arduino-stream.h"

class HostSerial : public Stream 
{
public:
    HostSerial();
    virtual ~HostSerial();
    typedef std::deque<byte> SerialBuffer;

    // override functions
    virtual int print(byte data);
    virtual size_t available();
    virtual byte read();

    // data feeeder
    void push(byte data);

    template <class Iter>
    void push(Iter begin, Iter end) {
        std::copy(begin, end, back_inserter(m_readBuffer));
    }

private:
    SerialBuffer m_readBuffer;
};

#endif // HOST_SERIAL_H__
