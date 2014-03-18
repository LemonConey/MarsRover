#include <algorithm>
#include "client-mgr.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
#define CLIENT_MANAGER_DEFAULT_SOCKET_TYPE      -1

void ClientManager::freeClients() {
   for_each(m_clients.begin(), m_clients.end(), [](Clients::value_type &value) {
        delete value.second;
    });
}

Poco::Net::DatagramSocket * ClientManager::getSocket(const uint8_t type) {
    if (m_clients.count(type)) {
        return m_clients[type];
    }
    return m_clients[CLIENT_MANAGER_DEFAULT_SOCKET_TYPE];
}

Poco::Net::DatagramSocket * ClientManager::registerClient(
    const uint8_t type,
    const Poco::Net::SocketAddress &address) {
    return registerInternal(type, address);
}

Poco::Net::DatagramSocket * ClientManager::registerDefault(
    const Poco::Net::SocketAddress &address) {
    return registerInternal(CLIENT_MANAGER_DEFAULT_SOCKET_TYPE, address);
}

void ClientManager::deregister(const uint8_t type) {
    deregisterInternal(type);
}

void ClientManager::deregisterDefault() {
    deregisterInternal(CLIENT_MANAGER_DEFAULT_SOCKET_TYPE);
}

bool ClientManager::send(const uint8_t type, const char *buffer, size_t size) {
    ScopedReadRWLock guard(m_clients_lock);
    DatagramSocket *socket = getSocket(type);
    if (socket) {
        int ret = socket->sendBytes(buffer, size);
        return ret == (int)size;
    }
    return false;
}

Poco::Net::DatagramSocket * ClientManager::registerInternal(
    int type, 
    const Poco::Net::SocketAddress &address) {
    printf("register type %d %s\n", type, address.toString().c_str());
    ScopedWriteRWLock guard(m_clients_lock);
    DatagramSocket *socket = m_clients[type];
    delete socket;
    socket = new DatagramSocket();
    socket->connect(address);
    return (m_clients[type] = socket);
}

void ClientManager::deregisterInternal(int type) {
    printf("deregister type %d\n", type);
    ScopedWriteRWLock guard(m_clients_lock);
    if (m_clients.count(type)) {
        delete m_clients[type];
        m_clients.erase(type);
    }
}


