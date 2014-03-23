#include "smsg-common.h"
#include "smsg-app.h"

using namespace Poco::Util;

SMessageApp::SMessageApp()
{
    m_serial_port = SMSGD_DEFAULT_SERIAL_PORT;
    m_udp_port = SMSGD_DEFAULT_UDP_PORT;
}

int SMessageApp::main( const std::vector < std::string > & args )
{
    while (1) {
        printf("hehe\n");
        sleep(1);
    }
    printf("init with %zu args\n", args.size());
    return EXIT_OK;
}


void SMessageApp::initialize( Application & self )
{
    printf("using serial port %s\n", m_serial_port.c_str());
    printf("using udp port %hu\n", m_udp_port);
}

void SMessageApp::defineOptions( OptionSet & options )
{
    options.addOption(Option("udp-port", "p").argument("udp-port"));
    options.addOption(Option("serial-port", "s").argument("serial-port"));
}

void SMessageApp::handleOption( const std::string & name, const std::string & value )
{
    if (name == "serial-port") {
        m_serial_port = value;
    } 
    else if (name == "udp-port") {
        m_udp_port = atoi(value.c_str());
    }
}
