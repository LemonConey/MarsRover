#include <stddef.h>
#include <assert.h>
#include <functional>
#include <SerialStream.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Thread.h>
#include "host-serial.h"
#include "CmdMessenger.h"
#include "udp-message.h"

#include "movement.h"

using namespace std;
using namespace std::tr1::placeholders;
using namespace Poco;
using namespace Poco::Net;


using namespace LibSerial;

// #define SERIAL_PORT "/dev/ttyUSB0"
#define SERIAL_PORT "/dev/ttyACM0"

//#define DEBUG

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
            realSerial = NULL;
        }
    }

    return realSerial;
}

HostSerial *get_host_serial() {
    static HostSerial *hostSerial = new HostSerial([&](byte data) {
        printf("send %02hhX(%c)\n", data, isprint(data) ? data : '?');
#ifndef DEBUG
        get_serial_conn()->put(data);
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

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#ifdef NDEBUG
#define ensure(x)   x
#else
#define ensure      assert
#endif


int main(int argc, char **argv) {

    if (!get_serial_conn()) {
        printf("can't open serial %s\n", SERIAL_PORT);
        return -1;
    }

    printf("cmdmsg started\n");

    Thread udpThread, serialThread;

    udpThread.start([&](void *){
        while (1)
        {
            char buf[4096];
            int ret = get_udp_conn()->receiveBytes(buf, sizeof(buf));
            assert(ret > 0);
            UdpMessage msg(buf, ret);
            printf("type:%hhu size:%hhu chechsum:%hu\n",
                msg.getHeader()->type,
                msg.getHeader()->size,
                msg.getHeader()->chechsum);         
            Protocol::Movement *mm = msg.getMessage<Protocol::Movement>();
            for (size_t i = 0; i < ARRAY_SIZE(mm->motors); ++i) {
                printf("motor[%zu] - power:%hd duration:%hu\n",
                    i, mm->motors[i].power, mm->motors[i].duration);
            }

#ifndef DEBUG
            // send the command to arduino
            ensure(get_cmd_messenger()->sendBinCmd(
                msg.getHeader()->type, *mm));
#endif
        }
        printf("udp thread out\n");
    }, NULL);

    // don't care the upstream now
    //serialThread.start([&](void *){
    //    while (1)
    //    {
    //        byte data = get_serial_conn()->get();
    //        get_host_serial()->push(data);
    //        //printf("read %02hhx len:%zd\n", data, get_host_serial()->available());
    //        printf("%c", get_host_serial()->read());
    //    }
    //    printf("serial thread out\n");
    //}, NULL);

    udpThread.join();
    serialThread.join();
    return 0;
}
