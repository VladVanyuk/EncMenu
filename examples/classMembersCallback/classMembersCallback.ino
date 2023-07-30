#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MakeMenu.h"
#include "Bulb.h"
#include "Led.h"
#include "encoder.h"


#define PIN_BULB1 35
#define PIN_BULB2 36
#define PIN_BULB3 37
#define PIN_BULB4 38

#define PIN_LED1 41
#define PIN_LED2 42
#define PIN_LED3 43
#define PIN_LED4 44
#define PIN_LED5 45
#define PIN_LED6 46


enum menus {BULBS_SCREEN , LEDS_SCREEN};
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);

Menu menu_main(&lcd); // menu obj

#include "Bulb.h"
#include "Led.h"



#include "GyverEncoder.h" // https://github.com/GyverLibs/GyverEncoder
#include "encoder.h"

void lcdSetup()
{
    lcd.clear();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
}

void menuSetup()
{

    menu_main.SetNames(BULBS_SCREEN, DEFAULT_LINE, "Light Screen");
    menu_main.SetNames(BULBS_SCREEN, bulb1.lineNumber, "1.Bulb1", &(bulb1.*bulb_ptr), SWITCH);
    menu_main.SetNames(BULBS_SCREEN, bulb2.lineNumber, "2.Bulb2", &(bulb2.*bulb_ptr), SWITCH);
    menu_main.SetNames(BULBS_SCREEN, bulb3.lineNumber, "3.Bulb3", &(bulb3.*bulb_ptr), SWITCH);
    menu_main.SetNames(BULBS_SCREEN, bulb4.lineNumber, "4.Bulb4", &(bulb4.*bulb_ptr), SWITCH);

    menu_main.SetObjFunc1(BULBS_SCREEN, bulb1.lineNumber, BULB::stat_bulb, (void *)&bulb1);
    menu_main.SetObjFunc1(BULBS_SCREEN, bulb2.lineNumber, BULB::stat_bulb, (void *)&bulb2);
    menu_main.SetObjFunc1(BULBS_SCREEN, bulb3.lineNumber, BULB::stat_bulb, (void *)&bulb3);
    menu_main.SetObjFunc1(BULBS_SCREEN, bulb4.lineNumber, BULB::stat_bulb, (void *)&bulb4);

    menu_main.SetNames(LEDS_SCREEN, DEFAULT_LINE, "Led Screen");
    menu_main.SetNames(LEDS_SCREEN, led1.lineNumber, "1.led1", &(led1.*led_ptr), BARGRAPH);
    menu_main.SetNames(LEDS_SCREEN, led2.lineNumber, "2.Led2", &(led2.*led_ptr), BARGRAPH);
    menu_main.SetNames(LEDS_SCREEN, led3.lineNumber, "3.led3", &(led3.*led_ptr), BARGRAPH);
    menu_main.SetNames(LEDS_SCREEN, led4.lineNumber, "4.Led4", &(led4.*led_ptr), BARGRAPH);
    menu_main.SetNames(LEDS_SCREEN, led5.lineNumber, "5.led5", &(led5.*led_ptr), BARGRAPH);
    menu_main.SetNames(LEDS_SCREEN, led6.lineNumber, "6.Led6", &(led6.*led_ptr), BARGRAPH);


    menu_main.SetObjFunc1(LEDS_SCREEN, led1.lineNumber, LED::stat_led_up, (void *)&led1);
    menu_main.SetObjFunc1(LEDS_SCREEN, led2.lineNumber, LED::stat_led_up, (void *)&led2);
    menu_main.SetObjFunc1(LEDS_SCREEN, led3.lineNumber, LED::stat_led_up, (void *)&led3);
    menu_main.SetObjFunc1(LEDS_SCREEN, led4.lineNumber, LED::stat_led_up, (void *)&led4);
    menu_main.SetObjFunc1(LEDS_SCREEN, led5.lineNumber, LED::stat_led_up, (void *)&led5);
    menu_main.SetObjFunc1(LEDS_SCREEN, led6.lineNumber, LED::stat_led_up, (void *)&led6);

    menu_main.SetObjFunc2(LEDS_SCREEN, led1.lineNumber, LED::stat_led_down, (void *)&led1);
    menu_main.SetObjFunc2(LEDS_SCREEN, led2.lineNumber, LED::stat_led_down, (void *)&led2);
    menu_main.SetObjFunc2(LEDS_SCREEN, led3.lineNumber, LED::stat_led_down, (void *)&led3);
    menu_main.SetObjFunc2(LEDS_SCREEN, led4.lineNumber, LED::stat_led_down, (void *)&led4);
    menu_main.SetObjFunc2(LEDS_SCREEN, led5.lineNumber, LED::stat_led_down, (void *)&led5);
    menu_main.SetObjFunc2(LEDS_SCREEN, led6.lineNumber, LED::stat_led_down, (void *)&led6);

    lcd.clear();


   menu_main.mFlags.menuActive = true;
    menu_main.mFlags.menuUpdate = true;
}

void setup()
{
    encoderSetup();
    lcdSetup();
    menuSetup();
}

void loop()
{
    encoderHandler();

   menu_main.MenuHandler();
}