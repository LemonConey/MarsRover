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
            udpSocket->connect(SocketAddress("127.0.0.1", UDP_REMOTE_PORT));
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

Thread *create_message_thread(SMessage *reader, SMessage *writer) {
    Thread *thread = new Thread();
    SMessage *smsgs[2] = {reader, writer};

    // can't directly pass the args through [&]
    thread->start([](void * arg) {
        SMessage **smsgs = (SMessage **)arg;
        SMessage *reader = smsgs[0];
        SMessage *writer = smsgs[1];

        reader->onUnhandledMessage([&](SMessagePDU::Message *msg, void *arg) {
            SMessage *writer = (SMessage *)arg;
            printf("[%s]read msg type:%hhd size:%hhd\n", 
                writer == get_serial_smsg() ? "udp" : "serial",
                msg->type, msg->size);
            writer->send(msg->type, msg->data, msg->size);
        }, writer);

        while (1) {
            if (reader->feed() == 0) {
                usleep(1000);
            }
        }
    }, smsgs);
    sleep(1);
    return thread;
}

int main(int argc, char **argv) {

    if (!get_serial_conn()) {
        printf("can't open serial %s\n", SERIAL_PORT);
        return -1;
    }
    if (!get_udp_conn()) {
        printf("can't create udp port\n");
        return -1;
    }

    printf("cmdmsg started\n");

    try {
        throw exception();
    }
    catch (std::exception &e) {
        printf("exception warmup?\n");
    }

    Thread *serialThread = create_message_thread(get_serial_smsg(), get_udp_smsg());
    Thread *udpThread = create_message_thread(get_udp_smsg(), get_serial_smsg());

    serialThread->join();
    udpThread->join();

    return 0;
}
