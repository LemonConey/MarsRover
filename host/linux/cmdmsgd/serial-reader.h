#ifndef __SERIAL_READER_H__
#define __SERIAL_READER_H__

#include <tr1/functional>
#include <SerialStream.h>
#include "host-serial.h"

class SerialReader {
public:
    typedef std::tr1::function<void(byte data)> Trigger;
    //typedef void (*Trigger)(byte data);
    SerialReader(LibSerial::SerialStream *realSerial,
        const Trigger &trigger) {
            m_realSerial = realSerial;
            m_trigger = trigger;
    }
    virtual ~SerialReader() {

    }

    void run() {
        while (1)
        {
            char data;
            m_realSerial->get(data);
            m_trigger(data);
        }
    }
private:
    LibSerial::SerialStream    *m_realSerial;
    Trigger                    m_trigger;
};

#endif // SERIAL_READER_H__
