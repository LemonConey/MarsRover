#include <alloca.h>
#include "arduino-missing.h"
#include "smsg-pdu.h"

void SMessagePDU::setupRawMessageCallback()
{
    memset(m_callbacks, 0, sizeof(m_callbacks));
    m_default_callback = NULL;
    m_coder.onDecode(rawMessageCallback, this);
}

void SMessagePDU::rawMessageCallback( const char *buf, size_t size, void *arg )
{
    SMessagePDU *msgpdu = (SMessagePDU *)arg;
    msgpdu->processRawMessage(buf, size);
}

void SMessagePDU::processRawMessage( const char *buf, size_t size )
{
    Message *msg = (Message *)buf;
#ifdef SMSG_PDU_MESSAGE_SIZE_VALIDATION
    if (msg->size + sizeof(*msg) != size) {
        return;
    }
#endif // SMSG_PDU_MESSAGE_SIZE_VALIDATION

    for (size_t i = 0; i < SMSG_PDU_MAX_CALLBACK_COUNT; ++i) {
        if (!m_callbacks[i].callback) {
            break;
        }
        if (m_callbacks[i].type == msg->type) {
            m_callbacks[i].callback(msg);
            return ;
        }
    }
    
    if (m_default_callback) {
        m_default_callback(msg);
    }
}

void SMessagePDU::feed( const char data )
{
    m_coder.feed(data);
}

void SMessagePDU::onUnhandledMessage( MessageCallbackProc callback )
{
    m_default_callback = callback;
}

void SMessagePDU::onMessage( uint8_t type, MessageCallbackProc callback )
{
    MessageCallbackEntry *entry = NULL;
    for (size_t i = 0; i < SMSG_PDU_MAX_CALLBACK_COUNT; ++i) {
        // first available entry
        if (!entry && !m_callbacks[i].callback) {
            entry = m_callbacks + i;
        }

        // replace current callback
        if (m_callbacks[i].callback && m_callbacks[i].type == type) {
            entry = m_callbacks + i;
            break;
        }
    }
 
    // if entry is null, you need to enlarge the SMSG_PDU_MAX_CALLBACK_COUNT
    assert(entry);
    entry->type = type;
    entry->callback = callback;
}

bool SMessagePDU::encode( uint8_t type, const char *inbuf, size_t insize, char *outbuf, size_t *outsize )
{
    Message *msg = (Message *)alloca(insize + sizeof(Message));
    msg->type = type;
    msg->size = insize;
    memcpy(msg->data, inbuf, insize);

    return m_coder.encode((const char *)msg, insize + sizeof(Message),
        outbuf, outsize);
}

SMessagePDU::SMessagePDU()
{
    setupRawMessageCallback();
}

SMessagePDU::~SMessagePDU()
{

}
