#include "EncButton.h"

#define CLK 2
#define DT 3
#define SW 18

EncButton<EB_CALLBACK, CLK, DT, SW> enc;

void isr() {
  enc.tickISR();
}

void turnRight() {
  if (menu_active->mFlags.lineSelected == 0) {
    menu_active->MakeMenu(NEXT_LINE, UPDATE_SCREEN);
  } else {
    menu_active->RunFunction1();  // menu func1 in action
    menu_active->MakeMenuCurrentLine();
  }
}

void turnLeft() {
  if (menu_active->mFlags.lineSelected == 0) {
    menu_active->MakeMenu(PREVIOS_LINE, UPDATE_SCREEN);
  } else {
    menu_active->RunFunction2();  // menu func1 in action
    menu_active->MakeMenuCurrentLine();
  }
}

void turnRightH() {
  if (menu_active->mFlags.lineSelected == 0) {
    menu_active->MakeMenu(UPDATE_LINE, NEXT_SCREEN);
  }
}

void turnRightL() {
  if (menu_active->mFlags.lineSelected == 0) {
    menu_active->MakeMenu(UPDATE_LINE, PREVIOS_SCREEN);
  }
}

void click() {
  if (menu_active->mFlags.lineSelected == 0) {
    if (menu_active->CheckFunction3()) {
      menu_active->RunFunction3();  // menu func3 in action
    } else {
      menu_active->mFlags.lineSelected = 1;
      menu_active->MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
  } else {
    menu_active->mFlags.lineSelected = 0;
    menu_active->MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
  }
}

void encoderSetup() {
  enc.setEncReverse(true);

  enc.attach(RIGHT_HANDLER, turnRight);
  enc.attach(LEFT_HANDLER, turnLeft);
  enc.attach(RIGHT_H_HANDLER, turnRightH);
  enc.attach(LEFT_H_HANDLER, turnRightL);
  enc.attach(CLICK_HANDLER, click);

  attachInterrupt(0, isr, CHANGE);  // Interrupts on encoder pins
  attachInterrupt(1, isr, CHANGE);
  attachInterrupt(5, isr, CHANGE);
}

