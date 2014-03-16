#ifndef __SMESSAGE_H__
#define __SMESSAGE_H__

#include <stdint.h>
#include "smsg-pdu.h"
#include "smsg-source.h"

class SMessage {
public:
    SMessage(SMessageSource *msgSource) {
        m_msgSrouce = msgSource;
    }

    ~SMessage() {

    }

    template< class T>
    bool send(uint8_t type, const T& data){
        return send(type, (const char *)&data, sizeof(data));
    }

    bool send(uint8_t type, const char *buffer, size_t size){
        // todo - worse case
        size_t outSize = size * 2 + 2; 
        char *encodedData = (char *)alloca(outSize);
        if (!m_msgPdu.encode(type, buffer, size, encodedData, &outSize)) {
            return false;
        }

        return (size_t)m_msgSrouce->send(encodedData, outSize) == outSize;
    }

    int feed() {
        char buffer[256];
        int ret = m_msgSrouce->receive(buffer, sizeof(buffer));
        if (ret > 0) {
            for (size_t i = 0; i < (size_t)ret; ++i) {
                m_msgPdu.feed(buffer[i]);
            }
        } 
        return ret;
    }

    void onMessage(uint8_t type, SMessagePDU::MessageCallbackProc callback) {
        m_msgPdu.onMessage(type, callback);
    }

    void onUnhandledMessage(SMessagePDU::MessageCallbackProc callback) {
        m_msgPdu.onUnhandledMessage(callback);
    }
private:
    SMessagePDU     m_msgPdu;
    SMessageSource  *m_msgSrouce;
};

#endif // SMESSAGE_H__
