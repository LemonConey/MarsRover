#ifndef __ROVER_COMMON_H__
#define __ROVER_COMMON_H__

#include <Arduino.h>
#include "lcd1602.h"
#include "smessage.h"

#include "pin-def.h"

#define set_interval(interval, proc) ({         \
    static unsigned long __expires = millis();  \
    if (millis() >= __expires) {                \
    (proc);                                     \
    __expires = millis() + interval;            \
    }                                           \
})

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

//////////////////////////////////////////////////////////////////////////
// Decleration
void setup_motor_control();
void update_motor_state();

void init_smessage(Stream *comm);
SMessage *get_smessage();
LCD1602 *get_lcd1602();

#endif // ROVER_COMMON_H__
