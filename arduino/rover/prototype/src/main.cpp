#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"
#include "rover-common.h"
#include <SoftwareSerial.h>


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

unsigned int count = 0;
void loop()
{
    set_interval(1000, {
        const char *str = "hello kitty";
        get_smessage()->send(2, str, strlen(str));
    });

    set_interval(1, {
        update_motor_state();
    });

    if (s1.available()) {
        get_smessage()->feed();
    }
    
}
