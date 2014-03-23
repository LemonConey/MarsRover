#ifndef __SMSG_APP_H__
#define __SMSG_APP_H__

#include <stdint.h>
#include <string>
#include <Poco/Util/ServerApplication.h>

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
    Poco::Logger::Channel * getFileChannel(const std::string &path);
    std::string     m_serial_port;
    std::string     m_log_file;
    uint16_t        m_udp_port;
};

#endif // SMSG_APP_H__
