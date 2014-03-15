#ifndef __SMSG_PDU_H__
#define __SMSG_PDU_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "smsg-coder.h"

#define SMSG_PDU_MESSAGE_SIZE_VALIDATION
#define SMSG_PDU_DEFALFT_CALLBACK_INDEX      -1
class SMessagePDU {
public:
    struct Message {
        template <class T>
        T *get() {
#ifdef SMSG_PDU_MESSAGE_SIZE_VALIDATION
            if (sizeof(T) != size) {
                return NULL;
            }
#endif // SMSG_PDU_MESSAGE_SIZE_VALIDATION
            return (T*)data;
        }
        uint8_t type;
        uint8_t size;
        char    data[];
    };
    
    typedef void(*MessageCallbackProc)(Message *message);

    SMessagePDU(SMessageCoder *coder) {
        m_coder = coder;
        setupRawMessageCallback();
    }

    template <class T>
    bool encode(uint8_t type, const T *data,
        char *outbuf, size_t *outsize) {
        return encode(type, (const char *)data, sizeof(T),
            outbuf, outsize);
    }

    bool encode(uint8_t type, const char *inbuf, size_t insize,
        char *outbuf, size_t *outsize) {
        Message *msg = (Message *)alloca(insize + sizeof(Message));
        msg->type = type;
        msg->size = insize;
        memcpy(msg->data, inbuf, insize);

        return m_coder->encode((const char *)msg, insize + sizeof(Message),
            outbuf, outsize);
    }

    void onMessage(uint8_t type, MessageCallbackProc callback) {
        m_callbacks[type] = callback;
    }
    void onUnhandledMessage(MessageCallbackProc callback) {
        m_callbacks[SMSG_PDU_DEFALFT_CALLBACK_INDEX] = callback;
    }

    void feed(const char data) {
        m_coder->feed(data);
    }

private:

    void processRawMessage(const char *buf, size_t size) {
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

    static void rawMessageCallback(const char *buf, size_t size, void *arg) {
        SMessagePDU *msgpdu = (SMessagePDU *)arg;
        msgpdu->processRawMessage(buf, size);
    }

    void setupRawMessageCallback() {
        m_coder->onDecode(rawMessageCallback, this);
    }

    SMessageCoder                       *m_coder;
    std::map<int, MessageCallbackProc>  m_callbacks;
};

#endif // __SMSG_PDU_H__
