#include <string.h>
#include <stdlib.h>
#include "text-client-mgr.h"

bool TextClientManager::processDeregisterCommand(const std::string &command,
    const Poco::Net::SocketAddress &address) {
    char buf[16] = { 0 };
    if (sscanf(command.c_str(), "register %s", buf) != 1) {
        return false;
    }
    buf[sizeof(buf)-1] = 0;
    Poco::Net::DatagramSocket *socket = NULL;
    if (strcmp("all", buf) == 0) {
        socket = registerDefault(address);
    }
    else {
        socket = registerClient(atoi(buf), address);
    }
    if (socket) {
        const char *response = "registered";
        socket->sendBytes(response, strlen(response));
    }
    return true;
}

bool TextClientManager::processRegisterCommand(const std::string &command,
    const Poco::Net::SocketAddress &address) {
    char buf[16] = { 0 };
    if (sscanf(command.c_str(), "deregister %s", buf) != 1) {
        return false;
    }
    buf[sizeof(buf)-1] = 0;

    if (strcmp("all", buf) == 0) {
        deregisterDefault();
    }
    else {
        deregister(atoi(buf));
    }
    return true;
}

bool TextClientManager::processCommand(const std::string &command,
    const Poco::Net::SocketAddress &address) {
    return processRegisterCommand(command, address)
        || processDeregisterCommand(command, address);
}
