#ifndef __LCD1602_H__
#define __LCD1602_H__
// from arduino lib
#include <LiquidCrystal.h>

// override the following values before include this file

#ifndef LCD1602_RS
#define LCD1602_RS      8
#endif
#ifndef LCD1602_EN
#define LCD1602_EN      9
#endif
#ifndef LCD1602_D4
#define LCD1602_D4      10
#endif
#ifndef LCD1602_D5
#define LCD1602_D5      11
#endif
#ifndef LCD1602_D6
#define LCD1602_D6      12
#endif
#ifndef LCD1602_D7
#define LCD1602_D7      13
#endif

#define LCD1602_ROWS    2
#define LCD1602_COLS    16
#define LCD1602_PRINT_BUFSIZE   (32 + 1)   
#define LCD1602_CALL_PRINTF()   ({ va_list vlist;   \
    va_start(vlist, fmt);                           \
    printf(fmt, vlist);                             \
    va_end(vlist);})

class LCD1602 : public LiquidCrystal {

public:
    LCD1602(int rs = LCD1602_RS, int en = LCD1602_EN, int d4 = LCD1602_D4,
        int d5 = LCD1602_D5, int d6 = LCD1602_D6, int d7 = LCD1602_D7) :
        LiquidCrystal(rs, en, d4, d5, d6, d7) {
            LiquidCrystal::begin(LCD1602_COLS, LCD1602_ROWS);
    }

    void printAt(int row, int col, const char *fmt, ...) {
        LiquidCrystal::setCursor(col, row);
        LCD1602_CALL_PRINTF();
    }

    void printAt(int row, const char *fmt, ...) {
        LiquidCrystal::setCursor(0, row);
        LCD1602_CALL_PRINTF();
    }

    void print(const char *fmt, ...) {
        LCD1602_CALL_PRINTF();
    }

public:
    static LCD1602 *CreateDefaultInstance() {
        return new LCD1602();
    }

private:
    void printf(const char *fmt, va_list vlist) {
        char buf[LCD1602_PRINT_BUFSIZE];
        vsnprintf(buf, sizeof(buf), fmt, vlist);
        LiquidCrystal::print(buf);
    }
};
#endif
