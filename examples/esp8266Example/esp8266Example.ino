#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "GyverEncoder.h"   // https://github.com/GyverLibs/GyverEncoder
#include <MakeMenu.h>

#ifndef LED_BUILTIN // For compatibility
	#define LED_BUILTIN BUILTIN_LED
#endif

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);
Menu menu_main(&lcd); // menu obj

#define CLK D5
#define DT D6
#define SW D7
Encoder enc1(CLK, DT, SW);

uint16_t led_bright = 0;
uint16_t led_percentage = 0;
bool lcd_backlight;
uint16_t restart_flag = 0;

void brigh_up_led () {
  led_bright += 10;
  led_bright = constrain (led_bright, 0, 255);
  led_percentage = map(led_bright, 0, 255, 0, 100);
  analogWrite(LED_BUILTIN, led_bright);
}

void brigh_down_led () {
  led_bright -= 10;
  led_bright = constrain (led_bright, 0, 255);
  led_percentage = map(led_bright, 0, 255, 0, 100);
  analogWrite(LED_BUILTIN, led_bright);
}

void noBacklight(){
  lcd_backlight = LOW;
  lcd.setBacklight(lcd_backlight);
}

void backlight(){
  lcd_backlight = HIGH;
  lcd.setBacklight(lcd_backlight);
}

void restartNow(){
  ESP.restart();
}

void lcdSetup(){
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd_backlight = HIGH;
  lcd.print("Initializing...");
}

void menuSetup(){
  //***************First screen************
  menu_main.SetNames(0, DEFAULT_LINE, "Light Screen");
  menu_main.SetNames(0, 1, "1.Led1", &led_percentage);
  menu_main.SetNames(0, 2, "2.BacklightLcd", &lcd_backlight);
  menu_main.SetNames(0, 3, "3.Restart", &restart_flag);

  menu_main.SetProgressBarLine(0, 1, true);
  menu_main.SetBoolValueLine(0, 2, true);

  menu_main.SetFunc1(0, 1, brigh_up_led);
  menu_main.SetFunc2(0, 1, brigh_down_led);

  menu_main.SetFunc1(0, 2, noBacklight);
  menu_main.SetFunc2(0, 2, backlight);

  menu_main.SetFunc3(0, 3, restartNow);

  lcd.clear();
  menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN); // Update information on the screen
  menu_main.mFlags.menuActive = true;
}

void setup()
{
  lcdSetup();

  attachInterrupt(0, isrCLK, CHANGE);    
  attachInterrupt(1, isrDT, CHANGE);   

  pinMode(LED_BUILTIN, OUTPUT);
  analogWrite(LED_BUILTIN, led_bright);

  menuSetup();
}

IRAM_ATTR void isrCLK() {
  enc1.tick(); 
}

IRAM_ATTR void isrDT() {
  enc1.tick();  
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

void loop()
{
  encoderHandler();
}