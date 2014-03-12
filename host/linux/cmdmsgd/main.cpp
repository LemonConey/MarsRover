#include "host-serial.h"
#include "serial-reader.h"
#include "CmdMessenger.h"

using namespace std;
using namespace std::tr1::placeholders;

using namespace LibSerial;

int main(int argc, char **argv) {
    SerialStream realSerial("/dev/ttyUSB0",
        SerialStreamBuf::BAUD_9600,
        SerialStreamBuf::CHAR_SIZE_8,
        SerialStreamBuf::PARITY_NONE,
        1,
        SerialStreamBuf::FLOW_CONTROL_NONE);

    HostSerial *hostSerial = new HostSerial([&](byte data) {
        realSerial.put(data);
        return 0;
    });

    SerialReader serialReader(&realSerial, [&](byte data) {
        hostSerial->push(data);
        printf("read %02hhx len:%zd\n", data, hostSerial->available());
    });

    serialReader.run();
}
