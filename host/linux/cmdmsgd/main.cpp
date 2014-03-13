#include <assert.h>
#include <functional>
#include <SerialStream.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Thread.h>

#include "host-serial.h"
#include "CmdMessenger.h"
#include "udp-message.h"



using namespace std;
using namespace std::tr1::placeholders;
using namespace Poco;
using namespace Poco::Net;



using namespace LibSerial;

// #define SERIAL_PORT "/dev/ttyUSB0"
#define SERIAL_PORT "/dev/ttyACM0"

#define DEBUG

SerialStream *get_serial_conn() {
    static SerialStream *realSerial = NULL;
    if (!realSerial) {
        realSerial = new SerialStream(SERIAL_PORT,
            SerialStreamBuf::BAUD_9600,
            SerialStreamBuf::CHAR_SIZE_8,
            SerialStreamBuf::PARITY_NONE,
            1,
            SerialStreamBuf::FLOW_CONTROL_NONE);
        if (!realSerial->good()) {
            delete realSerial;
        }
    }

    return realSerial;
}

HostSerial *get_host_serial() {
    static HostSerial *hostSerial = new HostSerial([&](byte data) {
#ifndef DEBUG
        get_serial_conn()->put(data);
#else
        printf("%c", data);
        fflush(stdout);
#endif // DEBUG

        return 0;
    });
    return hostSerial;
}

DatagramSocket *get_udp_conn() {
    static DatagramSocket *udpSocket = NULL;
    if (udpSocket == NULL) {
        udpSocket = new DatagramSocket();
        try {
            udpSocket->bind(SocketAddress("0.0.0.0", 1314), true);
        }
        catch (exception &e) {
            printf("exception %s\n", e.what());
        }
    }

    return udpSocket;
}

CmdMessenger *get_cmd_messenger() {
    static CmdMessenger *msg = new CmdMessenger(*get_host_serial());
    return msg;
}

int main(int argc, char **argv) {

    if (!get_serial_conn()) {
        printf("can't open serial %s\n", SERIAL_PORT);
        return -1;
    }

    printf("cmdmsg started\n");

    Thread udpThread, serialThread;

    //get_cmd_messenger()->attach(1, [] {
    //    printf("%s\n", get_cmd_messenger()->readStringArg());
    //});

    udpThread.start([&](void *){
        while (1)
        {
            char buf[4096];
            int ret = get_udp_conn()->receiveBytes(buf, sizeof(buf));
            assert(ret > 0);
            UdpMessage msg(buf, ret);
            printf("%hhu %hhu %d\n",
                msg.getHeader()->type,
                msg.getHeader()->size,
                msg.isVaild());
        }
        printf("udp thread out\n");
    }, NULL);

    //{
    //    get_cmd_messenger()->sendCmd('1', "hehe");
    //    string cmd = "1,aa/a/haha;";
    //    get_host_serial()->push(cmd.begin(), cmd.end());
    //    get_cmd_messenger()->feedinSerialData();
    //}

    serialThread.start([&](void *){
        while (1)
        {
            byte data = get_serial_conn()->get();
            get_host_serial()->push(data);
            //printf("read %02hhx len:%zd\n", data, get_host_serial()->available());
            printf("%c", get_host_serial()->read());
        }
        printf("serial thread out\n");
    }, NULL);

    udpThread.join();
    serialThread.join();
    return 0;
}
