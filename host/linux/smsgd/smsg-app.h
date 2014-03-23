#ifndef __SMSG_APP_H__
#define __SMSG_APP_H__

#include <stdint.h>
#include <string>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/DatagramSocket.h>
#include <SerialPort.h>
#include <list>
#include "smessage.h"

class SMessageApp : public Poco::Util::ServerApplication {
public:
    SMessageApp();
protected:
    void initialize(Application & self);
    virtual int main(const std::vector < std::string > & args);
    virtual void defineOptions(Poco::Util::OptionSet & options);
    virtual void handleOption(const std::string & name,
        const std::string & value);

private:
    bool setupSerial();
    bool setupUdpConn();
    bool createSerialThread();
    bool createUdpThread();

    Poco::Logger::Channel * getFileChannel(const std::string &path);
    std::string                 m_serial_port;
    std::string                 m_log_file;
    uint16_t                    m_udp_port;

    Poco::Net::DatagramSocket   *m_udp_socket;
    SMessage                    *m_serial_msg;

    std::list<Poco::Thread *>        m_threads;
};

#endif // SMSG_APP_H__
