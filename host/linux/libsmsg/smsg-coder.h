#ifndef __SMSG_CODER_H__
#define __SMSG_CODER_H__

#include <memory.h>
#include <stdlib.h>

#define SMSG_DECODER_BUFFER_SIZE    256

class SMessageCoder {
public:
    typedef void(*DecoderCallbackProc)(const char *buf, size_t size, void *userdata);
    SMessageCoder(const char delimiter = '|', const char escaper = '\\')
    : m_delimiter(delimiter), m_escaper(escaper)
    {
        memset(m_buffer, 0, sizeof(m_buffer));
        m_buffer_index = 0;
        m_error = false;
        m_escaping = false;
    }

    ~SMessageCoder() {
    }

    bool encode(const char *inbuf, size_t insize, 
        char *outbuf, size_t *outsize) {

        // encode will append 1 byte delimiter
        if (*outsize < insize + 1) {
            return false;
        }

        size_t outidx = 0;
        for (size_t i = 0; i < insize; ++i)
        {
            int escape = !!needEscape(inbuf[i]);

            // escape will cost 1 more extra byte
            // outsize will minus 1 for delimiter
            // if out buffer is not big enough
            // insize * 2 is a good option
            if (outidx + escape >= *outsize - 1) {
                return false;
            }
            if (escape) {
                outbuf[outidx++] = m_escaper;
            }
            outbuf[outidx++] = inbuf[i];
        }

        outbuf[outidx++] = m_delimiter;
        *outsize = outidx;
        return true;
    }

    void feed(const char data) {
        if (m_escaping) {
            feedByte(data);
            m_escaping = false;
        } 
        else if (data == m_escaper) {
            m_escaping = true;
        }
        else if (data == m_delimiter) {
            endMessage();
        }
        else {
            feedByte(data);
        }
    }

    void onDecode(DecoderCallbackProc callback, void *userdata) {
        m_decode_callback = callback;
        m_userdata = userdata;
    }

private:
    bool needEscape(const char data) {
        return (data == m_delimiter || data == m_escaper);
    }

    void feedByte(const char data) {
        if (!m_error) {
            if (m_buffer_index >= sizeof(m_buffer)) {
                // if buffer is full, ignore current message
                m_error = true;
                return;
            }

            m_buffer[m_buffer_index++] = data;
        }
    }

    void endMessage() {
        if (m_error) {
            // reset error, ready for next message
            m_error = false;
        }
        else if (m_decode_callback) {
            m_decode_callback(m_buffer, m_buffer_index, m_userdata);
        }

        // reset buffer, process next message;
        m_buffer_index = 0;
    }

    DecoderCallbackProc     m_decode_callback;
    void                    *m_userdata;
    const char              m_delimiter;
    const char              m_escaper;
    char                    m_buffer[SMSG_DECODER_BUFFER_SIZE];
    size_t                  m_buffer_index;
    bool                    m_escaping;
    bool                    m_error;
};

#endif // __SMSG_CODER_H__
