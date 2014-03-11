#include "spec_helper.h"
#include "arduino-stream.h"

class MockStream : public Stream {
public:
    MOCK_METHOD0(read, byte());
    MOCK_METHOD0(available, size_t());
    MOCK_METHOD1(print, int(byte data));
};

TEST(Stream, ShouldPrintWordsFromString) {
    MockStream ms;
    const char *str = "hello world";
    for (unsigned int i = 0; i < strlen(str); ++i)
    {
        EXPECT_CALL(ms, print(str[i])).RetiresOnSaturation();
    }

    ms.Stream::print("hello world");
}

TEST(Stream, ShouldPrintCRLFOnPrintLn)  {
    MockStream ms;
    EXPECT_CALL(ms, print('\r'));
    EXPECT_CALL(ms, print('\n'));

    ms.println();
}


static int increaser(int *val) {
    return (*val)++;
}

TEST(Stream, ShouldCopySingleDataToBufferOnReadBytes) {
    MockStream ms;
    char readbuf[] = "1234567890abcdef";
    char destbuf[32] = { 0 };

    // it's ugly, but seems that WillRepeatedly doesn;t work
    EXPECT_CALL(ms, read())
        .WillOnce(Return('1'))
        .WillOnce(Return('2'))
        .WillOnce(Return('3'))
        .WillOnce(Return('4'))
        .WillOnce(Return('5'))
        .WillOnce(Return('6'))
        .WillOnce(Return('7'))
        .WillOnce(Return('8'))
        .WillOnce(Return('9'))
        .WillOnce(Return('0'))
        .WillOnce(Return('a'));

    ASSERT_EQ(11, ms.readBytes(destbuf, 11));
    EXPECT_EQ(0, memcmp(readbuf, destbuf, 11));
}