#include <stddef.h>
#include <assert.h>
#include <functional>
#include <SerialStream.h>
#include <SerialPort.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Thread.h>

#include "movement.h"

#include "smessage.h"
#include "serial-source.h"
#include "udp-source.h"
#include "text-client-mgr.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;


using namespace LibSerial;

// #define SERIAL_PORT "/dev/ttyUSB0"
#define SERIAL_PORT         "/dev/ttyACM0"
#define UDP_LOCAL_PORT      4000
#define UDP_REMOTE_PORT     5000
//#define DEBUG

SerialPort *get_serial_conn() {
    static SerialPort *serial = NULL;
    if (!serial) {
        serial = new SerialPort(SERIAL_PORT);
        serial->Open(SerialPort::BAUD_9600,
            SerialPort::CHAR_SIZE_8,
            SerialPort::PARITY_NONE,
            SerialPort::STOP_BITS_1,
            SerialPort::FLOW_CONTROL_NONE);
        if (!serial->IsOpen()) {
            delete serial;
            serial = NULL;
        }
    }
    return serial;
}

DatagramSocket *get_udp_conn() {
    static DatagramSocket *udpSocket = NULL;
    if (udpSocket == NULL) {
        udpSocket = new DatagramSocket();
        try {
            udpSocket->bind(SocketAddress("0.0.0.0", UDP_LOCAL_PORT), true);
        }
        catch (exception &e) {
            printf("exception %s\n", e.what());
        }
    }

    return udpSocket;
}
SMessage *get_serial_smsg() {
    static SMessage *msg = new SMessage(new SerialSource(get_serial_conn()));
    return msg;
}

SMessage *get_udp_smsg() {
    static SMessage *msg = new SMessage(new UDPSource(get_udp_conn()));
    return msg;
}

Thread *create_serial_thread() {
    Thread *thread = new Thread();
    thread->start([](void * arg) {
        get_serial_smsg()->onUnhandledMessage([](SMessagePDU::Message *msg, void *) {
            printf("[serial]read msg type:%hhd size:%hhd\n", 
                msg->type, msg->size);
            try {
                //get_udp_conn()->sendBytes(msg, msg->size + sizeof(*msg));
            }
            catch (std::exception &e) {
                printf("send error:%s\n", e.what());
            }
            
        });

        while (1) {
            get_serial_smsg()->feed();
        }
    });
    return thread;
}

Thread *create_udp_thread() {
    Thread *thread = new Thread();
    thread->start([](void * arg) {
        DatagramSocket *sock = get_udp_conn();
        printf("start udp thread\n");
        while (1) {
            char buffer[256];
            SocketAddress address;
            int ret = sock->receiveFrom(buffer, sizeof(buffer) - 1, address);
            if (ret > 0) {
                buffer[ret] = 0;
                if (TextClientManager::getInstance()->processCommand(buffer, address)) {
                    printf("handled [%s]\n", buffer);
                    continue;
                }
                
                SMessagePDU::Message *msg = (SMessagePDU::Message *)buffer;
                if ((size_t)ret != msg->size + sizeof(*msg)) {
                    printf("[udp] invalid msg type:%hhu size:%hhu ret:%d\n",
                        msg->type, msg->size, ret);
                }
                else {
                    printf("[udp] read msg type:%hhu size:%hhu\n",
                        msg->type, msg->size);
                    get_serial_smsg()->send(msg->type, msg->data, msg->size);
                }
                
            }
            
        }
    }, NULL);
    return thread;
}

int main(int argc, char **argv) {
    try {
        throw exception();
    }
    catch (std::exception &e) {
        printf("exception warmup?\n");
    }
//    create_udp_thread()->join();

    if (!get_serial_conn()) {
        printf("can't open serial %s\n", SERIAL_PORT);
        return -1;
    }
    if (!get_udp_conn()) {
        printf("can't create udp port\n");
        return -1;
    }

    printf("cmdmsg started\n");


    Thread *serialThread = create_serial_thread();
    Thread *udpThread = create_udp_thread();
    serialThread->join();
    udpThread->join();

    return 0;
}
