#ifndef __UDP_MESSAGE_H__
#define __UDP_MESSAGE_H__

#include <stdint.h>

#define UDPMSG_BUFFER_SIZE  252
class UdpMessage {
public:
    typedef struct {
        uint8_t type;       // command type
        uint8_t size;       // content size(without header)
        uint16_t chechsum;   // optional checksum(0 to disable)
    } Header;

    UdpMessage() {
        m_msgbuf = m_buffer + sizeof(Header);
        m_header = (Header *)m_buffer;
    }
    UdpMessage(const char *buffer, size_t size) {
        UdpMessage();
        memcpy(m_buffer, buffer, size);
    }
    
    UdpMessage(uint8_t type, const char *message, size_t size) {
        UdpMessage();
        m_header->size = size;
        m_header->type = type;
        memcpy(m_msgbuf, message, size);
    }

    virtual ~UdpMessage() {

    }

    bool isVaild() {
        if (m_header->size > UDPMSG_BUFFER_SIZE) {
            return false;
        }
        // todo - checksum
        return true;
    }

    Header *getHeader() {
        return m_header;
    }

    const char *getBuffer() {
        return m_buffer;
    }

    // get used buffer size
    size_t getBufferSize() {
        assert(m_buffer->size <= UDPMSG_BUFFER_SIZE);
        return m_header->size + sizeof(*m_header);
    }

private:
    char    m_buffer[UDPMSG_BUFFER_SIZE + sizeof(Header)];
    char    *m_msgbuf;
    Header  *m_header;
};

#endif // __UDP_MESSAGE_H__
