
#ifndef MENUSYMBOLS_H
#define MENUSYMBOLS_H

#include <Arduino.h>

byte bar_start_0[8]= {
    B01111,
    B11000,
    B10000,
    B10000,
    B10000,
    B10000,
    B11000,
    B01111};

byte bar_start_1[8]= {
    B01111,
    B11000,
    B10011,
    B10111,
    B10111,
    B10011,
    B11000,
    B01111};

byte bar_middle_0[8]= {
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111};

byte bar_middle_1[8]= {
    B11111,
    B00000,
    B11000,
    B11000,
    B11000,
    B11000,
    B00000,
    B11111};

byte bar_middle_2[8]= {
    B11111,
    B00000,
    B11011,
    B11011,
    B11011,
    B11011,
    B00000,
    B11111};

byte bar_end_0[8]= {
    B11110,
    B00011,
    B00001,
    B00001,
    B00001,
    B00001,
    B00011,
    B11110};

byte bar_end_1[8]= {
    B11110,
    B00011,
    B11001,
    B11101,
    B11101,
    B11001,
    B00011,
    B11110};

byte down_arrow[8]= {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b10001,
    0b01010,
    0b00100};

byte up_arrow[8]= {
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100};

#endif