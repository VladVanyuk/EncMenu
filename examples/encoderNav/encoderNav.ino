#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MakeMenu.h"

Menu *menu_active = nullptr;

#include "EncoderHandler.h"

enum menus
{
    BULBS_SCREEN,
    LEDS_SCREEN
};

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);

Menu menu_main(&lcd); // menu obj


uint16_t led_percentage = 0;
bool lcd_backlight;

void brigh_up_led () {
  led_percentage += 5;
  led_percentage = constrain(led_percentage, 0, 100);
}

void brigh_down_led () {
  led_percentage -= 5;
  led_percentage = constrain(led_percentage, 0, 100);
}

void lcd_backlight_toggle(){
  lcd_backlight = !lcd_backlight;
  lcd.setBacklight(lcd_backlight);
}

void lcdSetup()
{
    lcd.clear();
    lcd.init();
    lcd.backlight();
    lcd_backlight = true;
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
}

void menuSetup()
{
    menu_main.SetNames(BULBS_SCREEN, DEFAULT_LINE, "Lcd Light");
    menu_main.SetNames(BULBS_SCREEN, 1, "1.None");
    menu_main.SetNames(BULBS_SCREEN, 2, "2.Normal", &led_percentage);
    menu_main.SetNames(BULBS_SCREEN, 3, "3.Switch", &led_percentage, SWITCH);
    menu_main.SetNames(BULBS_SCREEN, 4, "4.Graph", &led_percentage, BARGRAPH);
    menu_main.SetNames(BULBS_SCREEN, 5, "5.None");
    menu_main.SetNames(BULBS_SCREEN, 6, "6.None");

    menu_main.SetFunc3(BULBS_SCREEN, 3, lcd_backlight_toggle);
    menu_main.SetFunc1(BULBS_SCREEN, 4, brigh_up_led);
    menu_main.SetFunc2(BULBS_SCREEN, 4, brigh_down_led);

    menu_main.SetNames(LEDS_SCREEN, DEFAULT_LINE, "Led Screen");
    menu_main.SetNames(LEDS_SCREEN, 1, "1.led1", &(led_percentage), BARGRAPH);

    menu_main.SetFunc1(LEDS_SCREEN, 1, brigh_up_led);
    menu_main.SetFunc2(LEDS_SCREEN, 1, brigh_down_led);

    lcd.clear();

    menu_main.mFlags.menuActive = true;
    menu_main.mFlags.menuUpdate = true;
   // menu_main.ActivateBacklightTimer();
    menu_active = &menu_main;
}

void setup()
{
  
    encoderSetup();
    lcdSetup();
    menuSetup();
}

void loop()
{
    if(enc.tick()) menu_active->SetMenuActive();
    menu_active->MenuHandler();
}