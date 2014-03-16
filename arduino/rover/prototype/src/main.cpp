#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

#include <SoftwareSerial.h>
#include <CmdMessenger.h>
#include "lcd1602.h"
#include "movement.h"
#include "smessage.h"
#include "arduino-serial-source.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define S1TX    52
#define S1RX    53

LCD1602 lcd;
SoftwareSerial s2(S1RX, S1TX);
#define s1      Serial

SerialSource serial_source(&s1);
SMessage smsg(&serial_source);

void init_serial() {
    s1.begin(9600);
    s2.begin(9600);
    s2.println("hello");
    lcd.printAt(0, "ready for serial!");
}

int inpin[] = {2, 3, 4, 5};
int outval[4] = {0};

void setup_l298n() {
    for (int i = 0; i < ARRAY_SIZE(inpin); ++i)
    {
        pinMode(inpin[i], OUTPUT);
    }
}

void movement_callback(SMessagePDU::Message *msg, void *) {
    Protocol::Movement *mm = msg->get<Protocol::Movement>();

    lcd.printAt(0, "%d %u", mm->motors[0].power, mm->motors[0].duration);
    lcd.printAt(1, "%d %u", mm->motors[1].power, mm->motors[1].duration);
}


void unhandled_message(SMessagePDU::Message *msg, void *) {
    lcd.printAt(1, "type:%02hhu size:%02hhu", msg->type, msg->size);
}

void setup_smsg() {
    smsg.onMessage(1, movement_callback);
    smsg.onUnhandledMessage(unhandled_message);
}

void setup()
{
    init_serial();
    setup_l298n();
    setup_smsg();
}


#define set_interval(interval, proc) ({         \
    static unsigned long __expires = millis();  \
    if (millis() >= __expires) {                \
        (proc);                                 \
        __expires = millis() + interval;        \
    }                                           \
})

unsigned int count = 0;
void loop()
{
    set_interval(1000, {
        const char *str = "hello kitty";
        smsg.send(2, str, strlen(str));
    });

    //set_interval(100, {
    //    lcd.printAt(0, "%02d bytes on wire", s1.available());
    //});

    //if (s1.available()) {
        smsg.feed();
    //}
    
}
