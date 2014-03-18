#ifndef __TEXT_CLIENT_MGR_H__
#define __TEXT_CLIENT_MGR_H__

#include <string>
#include <Poco/Net/DatagramSocket.h>
#include "client-mgr.h"

class TextClientManager : public ClientManager {
public:
    bool processCommand(const std::string &command, 
        const Poco::Net::SocketAddress &address);

    static TextClientManager *getInstance() {
        static TextClientManager textClientMgr;
        return &textClientMgr;
    }
private:
    bool processRegisterCommand(const std::string &command,
        const Poco::Net::SocketAddress &address);
    bool processDeregisterCommand(const std::string &command,
        const Poco::Net::SocketAddress &address);
};

#endif // __TEXT_CLIENT_MGR_H__
