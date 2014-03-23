#include <Poco/FileChannel.h>
#include "smsg-common.h"
#include "smsg-app.h"
using namespace Poco::Util;

SMessageApp::SMessageApp()
{
    m_serial_port = SMSGD_DEFAULT_SERIAL_PORT;
    m_udp_port = SMSGD_DEFAULT_UDP_PORT;
    m_log_file = SMSGD_DEFAULT_LOG_PATH;
}

int SMessageApp::main( const std::vector < std::string > & args )
{
    while (1) {
        logdebug("hehe");
        sleep(1);
    }
    return EXIT_OK;
}

Poco::Logger::Channel * SMessageApp::getFileChannel(const std::string &path)
{
    Poco::FileChannel *channel = new Poco::FileChannel(path);
    channel->setProperty("rotation", "4 M");
    return channel;
}


void SMessageApp::initialize( Application & self )
{
    ServerApplication::initialize(self);
    DEFAULT_LOGGER.setLevel(Poco::Message::PRIO_TRACE);

    if (config().getBool("application.runAsDaemon", false)) {
        logdebug("write log to %s\n", m_log_file.c_str());
        DEFAULT_LOGGER.setChannel(getFileChannel(m_log_file));
    }

    loginfo("log level is %d", DEFAULT_LOGGER.getLevel());
    loginfo("using serial port %s", m_serial_port.c_str());
    loginfo("using udp port %hu", m_udp_port);
}

void SMessageApp::defineOptions( OptionSet & options )
{
    ServerApplication::defineOptions(options);
    options.addOption(Option("udp-port", "p").argument("udp-port"));
    options.addOption(Option("serial-port", "s").argument("serial-port"));
    options.addOption(Option("log-file", "l").argument("log-file"));
}

void SMessageApp::handleOption( const std::string & name, const std::string & value )
{
    ServerApplication::handleOption(name, value);

    if (name == "serial-port") {
        m_serial_port = value;
    } 
    else if (name == "udp-port") {
        m_udp_port = atoi(value.c_str());
    }
    else if (name == "log-file") {
        m_log_file = value;
    }
}
