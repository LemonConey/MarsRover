#ifndef __BLOCKING_READER_H__
#define __BLOCKING_READER_H__

#include <tr1/functional>
#include <SerialStream.h>
#include "host-serial.h"

template <class DataType>
class BlockingReader {
public:
    typedef std::tr1::function<void(byte data)> Trigger;
    typedef std::tr1::function<DataType()> Reader;
    //typedef void (*Trigger)(byte data);
    BlockingReader(const Reader &reader,
        const Trigger &trigger) {
            m_reader = reader;
            m_trigger = trigger;
    }
    virtual ~BlockingReader() {

    }

    void run() {
        while (1)
        {
            m_trigger(m_reader());
        }
    }
private:
    Reader      m_reader;
    Trigger     m_trigger;
};

#endif // __BLOCKING_READER_H__