#include <Poco/FileChannel.h>
#include <Poco/Thread.h>

#include "smsg-common.h"
#include "smsg-app.h"
#include "serial-source.h"
#include "udp-source.h"
#include "text-client-mgr.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;

SMessageApp::SMessageApp()
{
    m_serial_port = SMSGD_DEFAULT_SERIAL_PORT;
    m_udp_port = SMSGD_DEFAULT_UDP_PORT;
    m_log_file = SMSGD_DEFAULT_LOG_PATH;

    m_udp_socket = NULL;
    m_serial_msg = NULL;
}

int SMessageApp::main( const std::vector < std::string > & args )
{
    std::for_each(m_threads.begin(), m_threads.end(), [](Thread *thread) {
        thread->join();
    });

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
        logdebug("write log to %s", m_log_file.c_str());
        DEFAULT_LOGGER.setChannel(getFileChannel(m_log_file));
    }

    loginfo("log level is %d", DEFAULT_LOGGER.getLevel());
    loginfo("using serial port %s", m_serial_port.c_str());
    loginfo("using udp port %hu", m_udp_port);

    if (!setupSerial()) {
        logerror("can't connect to %s", m_serial_port.c_str());
        exit(EXIT_IOERR);
    }

    if (!setupUdpConn()) {
        logerror("can't create udp socket bind on %hu", m_udp_port);
        exit(EXIT_IOERR);
    }

    if (!createUdpThread()) {
        logerror("can't create udp thread");
        exit(EXIT_IOERR);
    }
    
    if (!createSerialThread()) {
        logerror("can't create serial thread");
        exit(EXIT_IOERR);
    }
    
}

void SMessageApp::defineOptions( OptionSet & options )
{
    ServerApplication::defineOptions(options);
    options.addOption(Option("udp-port", "p").argument("udp-port"));
    options.addOption(Option("serial-port", "s").argument("serial-port"));
    options.addOption(Option("logfile", "l").argument("logfile"));
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
    else if (name == "logfile") {
        m_log_file = value;
    }
}

bool SMessageApp::setupSerial()
{
    SerialPort *serial_port = new SerialPort(m_serial_port);
    serial_port->Open(SerialPort::BAUD_9600,
        SerialPort::CHAR_SIZE_8,
        SerialPort::PARITY_NONE,
        SerialPort::STOP_BITS_1,
        SerialPort::FLOW_CONTROL_NONE);

    if (!serial_port->IsOpen()) {
        delete serial_port;
        return false;
    }
    
    m_serial_msg = new SMessage(new SerialSource(serial_port));
    return true;
}

bool SMessageApp::setupUdpConn()
{
    m_udp_socket = new DatagramSocket();
    try {
        m_udp_socket->bind(SocketAddress("0.0.0.0", m_udp_port), true);
    }
    catch (Exception &e) {
        DEFAULT_LOGGER.log(e);
        return false;
    }
    return true;
}

bool SMessageApp::createSerialThread()
{
    Thread *thread = new Thread();
    thread->start([](void * arg) {
        SMessageApp *self = (SMessageApp *)arg;
        loginfo("serial thread started");
        self->m_serial_msg->onUnhandledMessage([](SMessagePDU::Message *msg, void *) {
            logtrace("[serial]read msg type:%hhd size:%hhd", 
                msg->type, msg->size);
            try {
                TextClientManager::getInstance()->send(msg->type, 
                    (const char*)msg, msg->size + sizeof(*msg));
            }
            catch (std::exception &e) {
                logerror("send error:%s", e.what());
            }

        });

        while (1) {
            self->m_serial_msg->feed();
        }
    }, this);

    m_threads.push_back(thread);

    return true;
}

bool SMessageApp::createUdpThread()
{
    Thread *thread = new Thread();
    thread->start([](void * arg) {
        SMessageApp *self = (SMessageApp *)arg;
        loginfo("udp thread started");
        while (1) {
            char buffer[256];
            SocketAddress address;
            int ret = self->m_udp_socket->receiveFrom(buffer, sizeof(buffer) - 1, address);
            if (ret > 0) {
                buffer[ret] = 0;
                if (TextClientManager::getInstance()->processCommand(buffer, address)) {
                    logtrace("handled [%s]", buffer);
                    continue;
                }

                SMessagePDU::Message *msg = (SMessagePDU::Message *)buffer;
                if ((size_t)ret != msg->size + sizeof(*msg)) {
                    logtrace("[udp] invalid msg type:%hhu size:%hhu ret:%d",
                        msg->type, msg->size, ret);
                }
                else {
                    logtrace("[udp] read msg type:%hhu size:%hhu",
                        msg->type, msg->size);
                    self->m_serial_msg->send(msg->type, msg->data, msg->size);
                }

            }

        }
    }, this);
    m_threads.push_back(thread);
    return true;
}

int main(int argc, char **argv) {

    try {
        throw exception();
    }
    catch (std::exception &e) {
        logdebug("hehe");
    }

    return SMessageApp().run(argc, argv);
}