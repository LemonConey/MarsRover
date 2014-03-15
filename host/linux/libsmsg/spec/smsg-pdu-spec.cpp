#include <vector>
#include <map>

#include "spec_helper.h"
#include "smsg-pdu.h"
using namespace std;

class SMessagePDUSpec : public Test {
protected:
    void SetUp() {
        msgpdu = new SMessagePDU(&coder);
    }
    void TearDown() {
        delete msgpdu;
    }

    template <class T>
    void getEncodedBuffer(uint8_t type, const T *data, vector<char> &buffer) {
        size_t size = 256;
        buffer.resize(size);
        msgpdu->encode<T>(type, data, &buffer[0], &size);
        buffer.resize(size);
    }

    void feedBuffer(const vector<char> &buffer) {
        for (size_t i = 0; i < buffer.size(); i++) {
            msgpdu->feed(buffer[i]);
        }
    }

    SMessagePDU     *msgpdu;
    SMessageCoder   coder;
};

SMessagePDU::Message *message = NULL;
void MessageContentCallback(SMessagePDU::Message *msg) {
    message = msg;
}
TEST_F(SMessagePDUSpec, ShouldCallTheMessageProcWithMessageHeader) {
    msgpdu->onMessage(0, MessageContentCallback);

    int data = 0x9527;
    vector<char> buffer;

    getEncodedBuffer(0, &data, buffer);
    feedBuffer(buffer);

    ASSERT_NE((void *)NULL, message);
    EXPECT_EQ(0, message->type);
    EXPECT_EQ(sizeof(data), message->size);
    EXPECT_EQ(data, *(int*)message->data);
}

map<int, unsigned int> callbackCount;
void MessageCountCallback(SMessagePDU::Message *msg) {
    ++callbackCount[msg->type];
}

TEST_F(SMessagePDUSpec, ShouldCallTheMessageProcByType) {
    msgpdu->onMessage(0, MessageCountCallback);
    msgpdu->onMessage(1, MessageCountCallback);

    int data = 0;
    vector<char> buffer;

    getEncodedBuffer(0, &data, buffer);
    feedBuffer(buffer);
    feedBuffer(buffer);

    getEncodedBuffer(1, &data, buffer);
    feedBuffer(buffer);

    EXPECT_EQ(2U, callbackCount[0]);
    EXPECT_EQ(1U, callbackCount[1]);
}

TEST_F(SMessagePDUSpec, ShouldCallTheDefaultProcForUnhandledMessage) {
    msgpdu->onUnhandledMessage(MessageCountCallback);

    int data = 0;
    vector<char> buffer;

    getEncodedBuffer(3, &data, buffer);
    feedBuffer(buffer);

    EXPECT_EQ(1U, callbackCount[3]);
}