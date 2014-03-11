#ifndef __HOST_SERIAL_H__
#define __HOST_SERIAL_H__

#include <deque>
#include <algorithm>
#include <tr1/functional>
#include "arduino-stream.h"

class HostSerial : public Stream 
{
public:
    typedef std::deque<byte> SerialBuffer;
    typedef std::tr1::function<int(byte)> SendProc;
public:
    HostSerial(const SendProc &sender);
    virtual ~HostSerial();

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
    SerialBuffer    m_readBuffer;
    SendProc        m_sender;
};

#endif // HOST_SERIAL_H__
