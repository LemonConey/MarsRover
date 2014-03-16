#ifndef __SMSG_SOURCE_H__
#define __SMSG_SOURCE_H__

class SMessageSource {
public:
    virtual ~SMessageSource() {}
    virtual int send(const char *buffer, size_t size) = 0;
    virtual int receive(char *outBuffer, size_t outSize) = 0;
};

class SMessageStreamSource : public SMessageSource {
public:
    virtual ~SMessageStreamSource() {}
    int send(const char *buffer, size_t size){
        for (size_t i = 0; i < size; ++i) {
            send(buffer[i]);
        }
        return size;
    }

    int receive(char *outBuffer, size_t outSize) {
        size_t pos = 0;
        while (available() && pos < outSize)
        {
            outBuffer[pos++] = receive();
        }
        return (int)pos;
    }
protected:
    virtual void send(const char data) = 0;
    virtual char receive() = 0;
    virtual bool available() = 0;
};




#endif // SMSG_SOURCE_H__