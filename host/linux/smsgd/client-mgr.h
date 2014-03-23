#ifndef __CLIENT_MGR_H__
#define __CLIENT_MGR_H__

#include <map>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/RWLock.h>

class ClientManager {
public:
    ClientManager() {

    }

    virtual ~ClientManager() {

    }

    bool send(const uint8_t type, const char *buffer, size_t size);

    Poco::Net::DatagramSocket *registerClient(const uint8_t type,
        const Poco::Net::SocketAddress &address);
    Poco::Net::DatagramSocket *registerDefault(
        const Poco::Net::SocketAddress &address);
    void deregister(const uint8_t type);
    void deregister(const Poco::Net::DatagramSocket *socket);
    void deregisterDefault();

private:
    typedef std::map<int, Poco::Net::DatagramSocket *> Clients;

    Poco::Net::DatagramSocket *registerInternal(int type,
        const Poco::Net::SocketAddress &address);
    void deregisterInternal(int type);
    Poco::Net::DatagramSocket *getSocket(const uint8_t type);
    void freeClients();

    Clients             m_clients;
    Poco::RWLock        m_clients_lock;
};

#endif // __CLIENT_MGR_H__
