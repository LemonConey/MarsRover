#include "rover-common.h"
#include "movement.h"

static int inpin[] = {
    MOTOR_L298N_IN1, 
    MOTOR_L298N_IN2, 
    MOTOR_L298N_IN3, 
    MOTOR_L298N_IN4
};

struct motor_control {
    unsigned long expires;
};
static motor_control motors[2];

static void setup_l298n() {
    for (int i = 0; i < ARRAY_SIZE(inpin); ++i)
    {
        pinMode(inpin[i], OUTPUT);
    }
}

static void update_l298n(int index, int power) {
    int basePin = index * 2;
    if (power > 0) {
        analogWrite(inpin[basePin], power);
        analogWrite(inpin[basePin + 1], 0);
    } 
    else if (power < 0) {
        analogWrite(inpin[basePin], 0);
        analogWrite(inpin[basePin + 1], -power);
    }
    else {
        analogWrite(inpin[basePin], 0);
        analogWrite(inpin[basePin + 1], 0);
    }

    get_lcd1602()->printAt(index, "%04d", power);
}

static void movement_callback(SMessagePDU::Message *msg, void *) {
    Protocol::Movement *mm = msg->get<Protocol::Movement>();


    for (size_t i = 0; i < ARRAY_SIZE(motors); ++i) {
        motors[i].expires = millis() + mm->motors[i].duration;
        update_l298n(i, mm->motors[i].power);
    }
}


void setup_motor_control() {
    memset(motors, 0, sizeof(motors));
    setup_l298n();
    get_smessage()->onMessage(1, movement_callback);
}


void update_motor_state()
{
    for (size_t i = 0; i < ARRAY_SIZE(motors); ++i) {
        if (motors[i].expires && millis() >= motors[i].expires) {
            update_l298n(i, 0);
            motors[i].expires = 0;
        }
    }
}
