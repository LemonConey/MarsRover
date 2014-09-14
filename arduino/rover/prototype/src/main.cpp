#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"
#include "rover-common.h"
#include <SoftwareSerial.h>

#include "heartbeat.pb.h"

#include "joystick-control.h"

LCD1602 * get_lcd1602()
{
    static LCD1602 lcd;
    return &lcd;
}

void init_serial() {
    s1.begin(9600);
    sjoy.begin(9600);
}

extern void move(int power, int dir);
void joystick_callback(JoystickControl::Message *message, void *arg) {
    char buf[64];
    snprintf(buf, sizeof(buf), "id:%hhd val:%hhd\r\n", 
        message->id, message->value);
    s1.write(buf);
    int power = 150;
    switch (message->id)
    {
    case JoystickControl::Message::Up:
        move(power, 0);
        break;
    case JoystickControl::Message::Down:
        move(-power, 0);
        break;
    case JoystickControl::Message::Left:
        move(power, -80);
        break;
    case JoystickControl::Message::Right:
        move(power, 80);
        break;
    default:
        return;
    }
}

JoystickControl *get_joystick() {
    static JoystickControl *js = NULL;
    if (!js) {
        js = new JoystickControl(joystick_callback,NULL);
    }

    return js;
}

void setup()
{
    init_serial();
    init_smessage(&s1);
    setup_motor_control();

    get_lcd1602()->printAt(0, "Rover MX01 Ready!");
    get_joystick();

}

Heartbeat heartbeat;



void loop()
{
    ++heartbeat.ticks;
    set_interval(1000, {
        heartbeat.millis = millis();
        //        send_protobuf_msg(Heartbeat, heartbeat);
        heartbeat.ticks = 0;
    });

    set_interval(10, {
        update_motor_state();
    });

    if (s1.available()) {
        get_smessage()->feed();
    }

    if (sjoy.available()) {
        char buf[64];
        int len;
        do 
        {
            len = sjoy.readBytes(buf, min(sjoy.available(), sizeof(buf)));
            get_joystick()->readFromSerial(buf, len);
        } while (len);
    }
}
