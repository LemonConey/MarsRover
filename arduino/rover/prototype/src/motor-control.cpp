#include "rover-common.h"
#include "movement.h"
#include "movement.pb.h"

static int inpin[] = {
    MOTOR_L298N_IN1, 
    MOTOR_L298N_IN2, 
    MOTOR_L298N_IN3, 
    MOTOR_L298N_IN4
};

static unsigned long motor_expires = 0;

static void setup_l298n() {
    for (int i = 0; i < ARRAY_SIZE(inpin); ++i)
    {
        pinMode(inpin[i], OUTPUT);
    }
}

static void update_l298n(int index, int power) {
    char buf[32];
    snprintf(buf, sizeof(buf), "idx:%d pwr:%d\r\n", index, power);
    s1.write(buf);

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

}

static void process_movement(Movement *movement) {
    const Movement &mm = *movement;
    get_lcd1602()->printAt(0, "%ld:%ld:%ld", 
        mm.power, mm.direction, mm.duration);

    int left, right;
    left = right = mm.power;
    if (mm.direction < 0) {
        int dir = min(abs(mm.direction), 100);
        left = left * (100 - dir) / 100;
    } else if (mm.direction > 0) {
        int dir = min(abs(mm.direction), 100);
        right = right * (100 - dir) / 100;
    }

    motor_expires = millis() + mm.duration;

    get_lcd1602()->printAt(1, "%d:%d", left, right);

    update_l298n(0, left);
    update_l298n(1, right);
}

void move(int power, int dir) {
    Movement mm;
    mm.power = power;
    mm.direction = dir;
    mm.duration = 100;
    process_movement(&mm);
}

static void movement_callback(SMessagePDU::Message *msg, void *) {
    get_lcd1602()->printAt(0, "receive %d", msg->size);

    Movement mm;
    pb_istream_t istream = pb_istream_from_buffer((uint8_t*)msg->data, msg->size);
    pb_decode(&istream, Movement_fields, &mm);

    process_movement(&mm);
}


void setup_motor_control() {
    setup_l298n();
    get_smessage()->onMessage(Movement_Message_Id, movement_callback);
}


void update_motor_state()
{
    if (motor_expires && millis() >= motor_expires) {
        update_l298n(0, 0);
        update_l298n(1, 0);
        get_lcd1602()->printAt(1, "stop all motors");
        motor_expires = 0;
    }
}
