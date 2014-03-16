#include "smsg-pdu.h"

void SMessagePDU::setupRawMessageCallback()
{
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

    if (m_callbacks.count(msg->type)) {
        m_callbacks[msg->type](msg);
    }
    else if (m_callbacks.count(SMSG_PDU_DEFALFT_CALLBACK_INDEX)) {
        m_callbacks[SMSG_PDU_DEFALFT_CALLBACK_INDEX](msg);
    }
}

void SMessagePDU::feed( const char data )
{
    m_coder.feed(data);
}

void SMessagePDU::onUnhandledMessage( MessageCallbackProc callback )
{
    m_callbacks[SMSG_PDU_DEFALFT_CALLBACK_INDEX] = callback;
}

void SMessagePDU::onMessage( uint8_t type, MessageCallbackProc callback )
{
    m_callbacks[type] = callback;
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
