#include <assert.h>

#include <functional>

#include "host-serial.h"
#include "blocking-reader.h"
#include "CmdMessenger.h"

#include <Poco/Net/DatagramSocket.h>
#include <Poco/Thread.h>


using namespace std;
using namespace std::tr1::placeholders;
using namespace Poco;
using namespace Poco::Net;



using namespace LibSerial;

// #define SERIAL_PORT "/dev/ttyUSB0"
#define SERIAL_PORT "/dev/ttyACM0"


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
        get_serial_conn()->put(data);
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
int main(int argc, char **argv) {

    assert(get_serial_conn());

    printf("cmdmsg started\n");

    Thread udpThread, serialThread;

    udpThread.start([&](void *){
        while (1)
        {
            char buf[4096];
            int ret = get_udp_conn()->receiveBytes(buf, sizeof(buf));
            assert(ret > 0);
            {
                buf[ret] = 0;
                char *newline = strchr(buf, '\n');
                if (newline){
                    *newline = 0;
                }

            }
            printf("send command [%s]\n", buf);

            for_each(buf, buf + ret,[](char data){
                get_host_serial()->print(data);
            });
        }
    }, NULL);

    serialThread.start([&](void *){
        while (1)
        {
            byte data = get_serial_conn()->get();
            get_host_serial()->push(data);
            //printf("read %02hhx len:%zd\n", data, get_host_serial()->available());
            printf("%c", get_host_serial()->read());
        }
    }, NULL);

    udpThread.join();
    serialThread.join();
    return 0;
}
