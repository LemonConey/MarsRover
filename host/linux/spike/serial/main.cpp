#include <stdio.h>
#include <string.h>
#include <SerialStream.h>
using namespace LibSerial;

int main(int argc, char **argv) {
    SerialStream my_serial_stream("/dev/ttyUSB0",
        SerialStreamBuf::BAUD_9600,
        SerialStreamBuf::CHAR_SIZE_8,
        SerialStreamBuf::PARITY_NONE,
        1,
        SerialStreamBuf::FLOW_CONTROL_NONE) ;

    
    const char *str = "0123\r\n";
    for (unsigned int i = 0; i < strlen(str); ++i)
    {
        my_serial_stream.put(str[i]);
    }

    printf("start listening\n");

    while (1)
    {
        char data;
        my_serial_stream.get(data);
        printf("%c", data);
    }
    return 0;
}