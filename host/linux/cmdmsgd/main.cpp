#include "host-serial.h"
#include "serial-reader.h"
#include "CmdMessenger.h"

using namespace std;
using namespace LibSerial;


// seems that lambda won't work here in this version
class SerialStreamSenderContainer {
public:
    SerialStreamSenderContainer(SerialStream *serialStream) {
        m_serialStream = serialStream;
    }
    int operator()(byte data) {
        m_serialStream->put(data);
        return 0;
    }

private:
    SerialStream *m_serialStream;
};

int main(int argc, char **argv) {
    SerialStream realSerial("/dev/ttyUSB0",
        SerialStreamBuf::BAUD_9600,
        SerialStreamBuf::CHAR_SIZE_8,
        SerialStreamBuf::PARITY_NONE,
        1,
        SerialStreamBuf::FLOW_CONTROL_NONE);

    // hehe here
    HostSerial *hostSerial = new HostSerial(SerialStreamSenderContainer(&realSerial));
    

    SerialReader serialReader(&realSerial, [=](byte data) {
        hostSerial = 0;
        printf("read %02hhx len:%d\n", 1, 1);

    });

    serialReader.run();
}
