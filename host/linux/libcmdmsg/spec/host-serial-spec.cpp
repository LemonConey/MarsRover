#include <vector>

#include "spec_helper.h"
#include "host-serial.h"

using namespace std;

TEST(HostSerial, ShouldReadDataFeedOneByOne) {
    HostSerial serial;
    serial.push(1);
    serial.push(3);
    serial.push(0);
    serial.push(0xfe);

    ASSERT_EQ(4, serial.available());

    ASSERT_EQ(1, serial.read());
    ASSERT_EQ(3, serial.read());
    ASSERT_EQ(2, serial.available());

    ASSERT_EQ(0, serial.read());
    ASSERT_EQ(0xfe, serial.read());
    ASSERT_EQ(0, serial.available());
}

TEST(HostSerial, ShouldReadDataFeedInBatch) {
    HostSerial serial;

    byte rawData[] = {1, 3, 5, 7, 9};
    
    vector<byte> dataVector;
    dataVector.push_back(2);
    dataVector.push_back(4);
    dataVector.push_back(6);
    dataVector.push_back(8);

    serial.push(rawData, rawData + sizeof(rawData));
    ASSERT_EQ(5, serial.available());

    serial.push(dataVector.begin(), dataVector.end());
    ASSERT_EQ(9, serial.available());

    char readBuf[32];

    serial.readBytes(readBuf, 9);
    ASSERT_EQ(0, serial.available());

    byte dest[] = {1, 3, 5, 7, 9, 2, 4, 6, 8};
    ASSERT_EQ(0, memcmp(dest, readBuf, 9));
}
