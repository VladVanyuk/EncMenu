#pragma once
#include <Arduino.h>

 #include "EncCore/VirtButton.h"
 #include "EncCore/VirtEncoder.h"
 #include "EncCore/VirtEncButton.h"
 #include "EncCore/Button.h"
 #include "EncCore/Encoder.h"
#include "EncCore/EncButton.h"

#define CLK 2
#define DT 4
#define SW 3

/*
#ifndef EB_NO_CALLBACK

enum eb_callback {
    TURN_HANDLER,       // 0
    LEFT_HANDLER,       // 1
    RIGHT_HANDLER,      // 2
    LEFT_H_HANDLER,     // 3
    RIGHT_H_HANDLER,    // 4
    CLICK_HANDLER,      // 5
    HOLDED_HANDLER,     // 6
    STEP_HANDLER,       // 7
    PRESS_HANDLER,      // 8
    CLICKS_HANDLER,     // 9
    RELEASE_HANDLER,    // 10
    HOLD_HANDLER,       // 11
    TURN_H_HANDLER,     // 12
};
#endif

class EncButtonMenu : public VirtEncButton {
   public:
    // настроить пины (энк, энк, кнопка, pinmode энк, pinmode кнопка)
    EncButtonMenu(uint8_t encA = 0, uint8_t encB = 0, uint8_t btn = 0, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        init(encA, encB, btn, modeEnc, modeBtn, btnLevel);
    }

    // настроить пины (энк, энк, кнопка, pinmode энк, pinmode кнопка)
    void init(uint8_t encA = 0, uint8_t encB = 0, uint8_t btn = 0, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP, uint8_t btnLevel = LOW) {
        e0 = encA;
        e1 = encB;
        b = btn;
        pinMode(e0, modeEnc);
        pinMode(e1, modeEnc);
        pinMode(b, modeBtn);
        setBtnLevel(btnLevel);
        initEnc(readEnc());
    }
    
    // ===================================== CALLBACK =====================================
    void attach_call(eb_callback type, void (*handler)()) {
        _callback[type] = *handler;
    }

    void exec(uint8_t num) {
        if (*_callback[num]) _callback[num]();
    }

    void checkCallback() 
    {
        #ifndef EB_NO_CALLBACK
        if (turn()) exec(TURN_HANDLER);
        if (turnH()) exec(TURN_H_HANDLER);
        if (right()) exec(RIGHT_HANDLER);
        if (rightH()) exec(RIGHT_H_HANDLER);
        if (left()) exec(LEFT_HANDLER);
        if (leftH()) exec(LEFT_H_HANDLER);
        if (press()) exec(PRESS_HANDLER);
        if (hold()) exec(HOLD_HANDLER);
        if (click()) exec(CLICK_HANDLER);
        if (release()) exec(RELEASE_HANDLER);
        #endif
    }

    // ====================== TICK ======================
    // функция обработки для вызова в прерывании энкодера
    int8_t tickISR() {
        return VirtEncButton::tickISR(readEnc());
    }

    // функция обработки, вызывать в loop
    bool tick() {
        if (read_ef(EB_EISR)) return VirtEncButton::tick(EBread(b));
        else return VirtEncButton::tick(readEnc(), EBread(b));
    }

    // функция обработки без сброса событий
    bool tickRaw() {
        if (read_ef(EB_EISR)) return VirtEncButton::tickRaw(EBread(b));
        else return VirtEncButton::tickRaw(readEnc(), EBread(b));
    }

    // ====================== READ ======================
    // прочитать значение кнопки
    bool readBtn() {
        return EBread(b) ^ read_bf(EB_INV);
    }

    // прочитать значение энкодера
    int8_t readEnc() {
        return EBread(e0) | (EBread(e1) << 1);
    }

    // ===================== PRIVATE =====================
   private:
    uint8_t e0, e1, b;
#ifndef EB_NO_CALLBACK
    void (*_callback[14])() = {};
#endif
};

*/