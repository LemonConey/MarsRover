#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include <stdint.h>

namespace Protocol {

#define MOVEMENT_MIN_POWER      0
#define MOVEMENT_MAX_POWER      255
#define MOVEMENT_MOTOR_COUNT    2

    struct Motor{
        int16_t     power;
        uint16_t    duration;
    } __attribute__ ((__packed__));


    struct Movement{
        Motor       motors[MOVEMENT_MOTOR_COUNT];
    } __attribute__ ((__packed__));
};

#endif // MOVEMENT_H__
