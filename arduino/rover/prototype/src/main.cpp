#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"
#include "rover-common.h"
#include <SoftwareSerial.h>

#include "heartbeat.pb.h"

LCD1602 * get_lcd1602()
{
    static LCD1602 lcd;
    return &lcd;
}

void init_serial() {
    s1.begin(9600);
}

void setup()
{
    init_serial();
    init_smessage(&s1);
    setup_motor_control();

    get_lcd1602()->printAt(0, "Rover MX01 Ready!");

}

Heartbeat heartbeat;

void loop()
{
    ++heartbeat.ticks;
    set_interval(1000, {
        heartbeat.millis = millis();
        send_protobuf_msg(Heartbeat, heartbeat);
        heartbeat.ticks = 0;
    });

    set_interval(1, {
        update_motor_state();
    });

    if (s1.available()) {
        get_smessage()->feed();
    }
    
}
