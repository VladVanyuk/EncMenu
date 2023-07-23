#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MakeMenu.h>

#include "GyverEncoder.h"   // https://github.com/GyverLibs/GyverEncoder

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);
Menu menu_main(&lcd); // menu obj

#define CLK 2
#define DT 3
#define SW 4
Encoder enc1(CLK, DT, SW);

void isr()
{
  enc1.tick();
}

void encoderSetup()
{
  enc1.setType(TYPE2); 
  attachInterrupt(0, isr, CHANGE); // Interrupts on encoder pins
  attachInterrupt(1, isr, CHANGE);
}

void encoderHandler()
{
  enc1.tick();
  if (menu_main.mFlags.lineSelected == 0)
  {
    if (enc1.isRight()) //|| enc1.isFastR())
    {
      menu_main.MakeMenu(NEXT_LINE, UPDATE_SCREEN);
    };
    if (enc1.isLeft()) //|| enc1.isFastL())
    {
      menu_main.MakeMenu(PREVIOS_LINE, UPDATE_SCREEN);
    };
    if (enc1.isClick())
    {
      if (menu_main.CheckFunction3())
      {
        menu_main.RunFunction3(); // menu func3 in action
      }
      else
      {
        menu_main.mFlags.lineSelected = 1;
        menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
      }
    };

    if (enc1.isRightH())
    {
      menu_main.MakeMenu(UPDATE_LINE, NEXT_SCREEN); // next screen menu (RIGHT pressed turn)
    };
    if (enc1.isLeftH())
    {
      menu_main.MakeMenu(UPDATE_LINE, PREVIOS_SCREEN); // prev screen menu (LEFT pressed turn)
    };
    
  }
  else if (menu_main.mFlags.lineSelected == 1)
  {
    if (enc1.isRight())
    {
      menu_main.RunFunction1(); // menu func1 in action
    };
    if (enc1.isLeft())
    {
      menu_main.RunFunction2(); ////menu func2 in action
    };
    if (enc1.isClick())
    {
      menu_main.mFlags.lineSelected = 0;
      menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    };
  }
  if (enc1.isTurn() or enc1.isPress() or enc1.isRelease() or enc1.isClick())
  {
    menu_main.mFlags.menuActive = true;
  }
}

#define RELAY_PIN1 31
#define RELAY_PIN2 32

#define LED_PIN1 40
#define LED_PIN2 41
#define LED_PIN3 42
#define LED_PIN4 43

uint16_t state_relay1 = LOW; 
uint16_t state_relay2 = LOW; 

uint16_t ledBright1 = 0;
uint16_t ledBright2 = 0;
uint16_t ledBright3 = 0;
uint16_t ledBright4 = 0;

unsigned long scan_period = 0; 
uint16_t analogFan1 = 0;
uint16_t analogFan2 = 0;
uint16_t analogFan3 = 0;
uint16_t analogFan4 = 0;

uint16_t percentageFan1 = 0; 
uint16_t percentageFan2 = 0;
uint16_t percentageFan3 = 0;
uint16_t percentageFan4 = 0;

void lcdSetup(){
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
}

void toggle_relay1 () {
  state_relay1 = !state_relay1;
  digitalWrite(RELAY_PIN1, state_relay1);
}

void toggle_relay2 () {
  state_relay2 = !state_relay2;
  digitalWrite(RELAY_PIN2, state_relay2);
}

void brigh_up_led (byte pin, uint16_t bright) {
  bright += 10;
  bright = constrain (bright, 0, 255);
  analogWrite(pin, bright);
}

void brigh_down_led (byte pin, uint16_t bright) {
  bright -= 10;
  bright = constrain (bright, 0, 255);
  analogWrite(pin, bright);
}

void brigh_up_led1 () {
  brigh_up_led(LED_PIN1, ledBright1);
}

void brigh_down_led1 () {
  brigh_down_led(LED_PIN1, ledBright1);
}

void brigh_up_led2 () {
  brigh_up_led(LED_PIN2, ledBright2);
}

void brigh_down_led2 () {
  brigh_down_led(LED_PIN2, ledBright2);
}

void brigh_up_led3 () {
  brigh_up_led(LED_PIN3, ledBright3);
}

void brigh_down_led3 () {
  brigh_down_led(LED_PIN3, ledBright3);
}

void brigh_up_led4 () {
  brigh_up_led(LED_PIN4, ledBright4);
}

void brigh_down_led4 () {
  brigh_down_led(LED_PIN4, ledBright4);
}


void analogHandler(){
  if(menu_main.GetScreen() == 1){ //Fan menu
    if (millis() - scan_period > 1000) { //one second
      if(analogFan1 != analogRead(A1)){
        analogFan1 = analogRead(A1);
        menu_main.mFlags.menuUpdate = true;
      }else if (analogFan2 != analogRead(A2)) {
        analogFan2 != analogRead(A2);
        menu_main.mFlags.menuUpdate = true;
      }else if (analogFan3 != analogRead(A3)) {
        analogFan3 != analogRead(A3);
        menu_main.mFlags.menuUpdate = true;
      }else if (analogFan4 != analogRead(A4)) {
        analogFan4 != analogRead(A4);
        menu_main.mFlags.menuUpdate = true;
      }
      scan_period = millis();
    }
  }
}


void menuSetup(){
  //***************Led screen************
  menu_main.SetNames(0, DEFAULT_LINE, "Light Screen");
  menu_main.SetNames(0, 1, "1.Led1", &ledBright1);
  menu_main.SetNames(0, 2, "2.Led2", &ledBright2);
  menu_main.SetNames(0, 3, "3.Led3", &ledBright3);
  menu_main.SetNames(0, 4, "4.Led4", &ledBright4);

  menu_main.SetFunc1 (0, 1, brigh_up_led1);
  menu_main.SetFunc2 (0, 1, brigh_down_led1);

  menu_main.SetFunc1 (0, 2, brigh_up_led2);
  menu_main.SetFunc2 (0, 2, brigh_down_led2);

  menu_main.SetFunc1 (0, 3, brigh_up_led3);
  menu_main.SetFunc2 (0, 3, brigh_down_led3);

  menu_main.SetFunc1 (0, 4, brigh_up_led4);
  menu_main.SetFunc2 (0, 4, brigh_down_led4);

  //***************Fan screen************
  menu_main.SetNames(1, DEFAULT_LINE, "Analog Screen");
  menu_main.SetNames(1, 1, "1.Fan1", &percentageFan1);
  menu_main.SetNames(1, 2, "2.Fan2", &percentageFan2);
  menu_main.SetNames(1, 3, "3.Fan3", &percentageFan3);
  menu_main.SetNames(1, 4, "4.Fan4", &percentageFan4);

  menu_main.SetProgressBarLine(1, 1, true);
  menu_main.SetProgressBarLine(1, 2, true);
  menu_main.SetProgressBarLine(1, 3, true);
  menu_main.SetProgressBarLine(1, 4, true);

  scan_period = millis();

  //***************Relay screen************
  menu_main.SetNames(2, DEFAULT_LINE, "Relay Screen");
  menu_main.SetNames(2, 1, "1.Relay1", &state_relay1);
  menu_main.SetNames(2, 2, "2.Relay2", &state_relay2);
  menu_main.SetBoolValueLine(2, 1, true);
  menu_main.SetBoolValueLine(2, 2, true);

  menu_main.SetFunc3(2, 1, toggle_relay1);
  menu_main.SetFunc3(2, 2, toggle_relay2);


  lcd.clear();
  menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN); // Update information on the screen
  menu_main.mFlags.menuActive = true;
}

void setup()
{
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);

  digitalWrite(RELAY_PIN1, state_relay1);
  digitalWrite(RELAY_PIN2, state_relay2);

  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(LED_PIN4, OUTPUT);

  analogWrite(LED_PIN1, ledBright1);
  analogWrite(LED_PIN2, ledBright2);
  analogWrite(LED_PIN3, ledBright3);
  analogWrite(LED_PIN4, ledBright4);

  analogFan1 = analogRead(A1);
  percentageFan1 =  map(analogFan1, 0, 1023, 0, 100);
  analogFan2 = analogRead(A2);
  percentageFan2 =  map(analogFan2, 0, 1023, 0, 100);
  analogFan3 = analogRead(A3);
  percentageFan3 =  map(analogFan3, 0, 1023, 0, 100);
  analogFan4 = analogRead(A4);
  percentageFan4 =  map(analogFan4, 0, 1023, 0, 100);

  lcdSetup();
  menuSetup();
}

void loop()
{
  encoderHandler();
  analogHandler();

  if(menu_main.mFlags.menuUpdate == true){
    menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    menu_main.mFlags.menuUpdate = false;
  }
  
}