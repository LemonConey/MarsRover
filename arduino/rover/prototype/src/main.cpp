#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

#include <SoftwareSerial.h>
#include <CmdMessenger.h>
#include "lcd1602.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define S1TX    52
#define S1RX    53

LCD1602 lcd;
//SoftwareSerial s1(S1RX, S1TX);
#define s1      Serial
CmdMessenger cmdMessenger = CmdMessenger(s1);


void init_serial() {
    s1.begin(9600);
    s1.println("hello");
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

void setup()
{
    init_serial();
    setup_l298n();
}


void loop()
{
    lcd.printAt(0, "%02d bytes on wire", s1.available());
    if (s1.available())
    {
        String str = s1.readStringUntil('\0');
        if (str.length())
        {
            sscanf(str.c_str(), "%d %d %d %d", outval, outval + 1,
                outval + 2, outval + 3);

            lcd.printAt(1, "%03d %03d %03d %03d", outval[0], outval[1], 
                outval[2], outval[3]);
            for (int i = 0; i < ARRAY_SIZE(inpin); ++i)
            {
                analogWrite(inpin[i], min(outval[i], 255));
            }
        }
    }
}
