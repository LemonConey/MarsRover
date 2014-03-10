#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

#include <SoftwareSerial.h>
#include <CmdMessenger.h>
#include "lcd1602.h"


#define S1TX    52
#define S1RX    53

LCD1602 lcd;
SoftwareSerial s1(S1RX, S1TX);
CmdMessenger cmdMessenger = CmdMessenger(s1);

void setup()
{
    lcd.printAt(0, "ready for serial!");
    s1.begin(9600);
    s1.println("hello");
    cmdMessenger.printLfCr();   
    cmdMessenger.sendCmd(0,"Arduino has /;,started!");
}

void loop()
{
    lcd.printAt(0, "%d   ", s1.available());
    if (s1.available())
    {
        String str = s1.readStringUntil('\0');
        if (str.length())
        {
            lcd.printAt(1, "%s", str.c_str());
        }
    }
}
