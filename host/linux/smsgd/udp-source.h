#ifndef __UDP_SOURCE_H__
#define __UDP_SOURCE_H__

#include <Poco/Net/DatagramSocket.h>
#include "smsg-source.h"

class UDPSource : public SMessageSource {
public:
    UDPSource(Poco::Net::DatagramSocket *comm) {
        m_comm = comm;
    }

    virtual int send(const char *buffer, size_t size) {
        return m_comm->sendBytes(buffer, size);
    }
    virtual int receive(char *outBuffer, size_t outSize)
    {
        return m_comm->receiveBytes(outBuffer, outSize);
    }
private:
    Poco::Net::DatagramSocket  *m_comm;
};

#endif // UDP_SOURCE_H__
