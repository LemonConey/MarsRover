#include "rover-common.h"
#include "movement.h"

static int inpin[] = {
    MOTOR_L298N_IN1, 
    MOTOR_L298N_IN2, 
    MOTOR_L298N_IN3, 
    MOTOR_L298N_IN4
};

static void setup_l298n() {
    for (int i = 0; i < ARRAY_SIZE(inpin); ++i)
    {
        pinMode(inpin[i], OUTPUT);
    }
}

static void movement_callback(SMessagePDU::Message *msg, void *) {
    Protocol::Movement *mm = msg->get<Protocol::Movement>();
    get_lcd1602()->printAt(0, "%d %u", mm->motors[0].power,
        mm->motors[0].duration);
    get_lcd1602()->printAt(1, "%d %u", mm->motors[1].power, 
        mm->motors[1].duration);
}

void setup_motor_control() {
    setup_l298n();
    get_smessage()->onMessage(1, movement_callback);
}