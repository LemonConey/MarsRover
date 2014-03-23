#ifndef __ROVER_COMMON_H__
#define __ROVER_COMMON_H__

#include <Arduino.h>
#include "lcd1602.h"
#include "smessage.h"
#include "pb_encode.h"
#include "pb_decode.h"

#include "pin-def.h"

#define set_interval(interval, proc) ({         \
    static unsigned long __expires = millis();  \
    if (millis() >= __expires) {                \
    (proc);                                     \
    __expires = millis() + interval;            \
    }                                           \
})

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define LITERAL_CONCAT(x, y)    x##y
#define send_protobuf_msg(type, msg)  ({                            \
    char buffer[LITERAL_CONCAT(type, _size)];                       \
    pb_ostream_t stream = pb_ostream_from_buffer((uint8_t*)buffer,  \
    sizeof(buffer));                                            \
    pb_encode(&stream, LITERAL_CONCAT(type, _fields), &msg);        \
    get_smessage()->send(LITERAL_CONCAT(type, _Message_Id),         \
    buffer, stream.bytes_written);                              \
})


//////////////////////////////////////////////////////////////////////////
// Decleration
void setup_motor_control();
void update_motor_state();

void init_smessage(Stream *comm);
SMessage *get_smessage();
LCD1602 *get_lcd1602();

#endif // ROVER_COMMON_H__
