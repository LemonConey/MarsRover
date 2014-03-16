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

using namespace std;
using namespace Poco;
using namespace Poco::Net;


using namespace LibSerial;

// #define SERIAL_PORT "/dev/ttyUSB0"
#define SERIAL_PORT "/dev/ttyACM0"

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
            udpSocket->bind(SocketAddress("0.0.0.0", 4000), true);
            udpSocket->connect(SocketAddress("127.0.0.1", 5000));
        }
        catch (exception &e) {
            printf("exception %s\n", e.what());
        }
    }

    return udpSocket;
}


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#ifdef NDEBUG
#define ensure(x)   x
#else
#define ensure      assert
#endif

Thread *create_serial_thread() {
    Thread *thread = new Thread();
    thread->start([&](void *) {
        printf("serial thread in\n");
        SerialSource serialSource(get_serial_conn());
        SMessage smsgSerial(&serialSource);

        smsgSerial.onUnhandledMessage([](SMessagePDU::Message *msg) {
            printf("read msg type:%hhd size:%hhd\n", 
                msg->type, msg->size);
        });

        while (1) {
            if (smsgSerial.feed() == 0) {
                usleep(1000);
            }
        }
    });
    return thread;
}

Thread *create_udp_thread() {
    Thread *thread = new Thread();
    thread->start([&](void *) {
        printf("udp thread in\n");
        UDPSource udpSource(get_udp_conn());
        SMessage smsgUDP(&udpSource);

        smsgUDP.onUnhandledMessage([](SMessagePDU::Message *msg) {
            printf("read msg type:%hhd size:%hhd\n", 
                msg->type, msg->size);
        });

        while (1) {
            smsgUDP.feed();
        }
    });
    return thread;
}


int main(int argc, char **argv) {

    if (!get_serial_conn()) {
        printf("can't open serial %s\n", SERIAL_PORT);
        return -1;
    }

    printf("cmdmsg started\n");

    Thread *serialThread = create_serial_thread();
    Thread *udpThread = create_udp_thread();

    serialThread->join();
    udpThread->join();

    return 0;
}
