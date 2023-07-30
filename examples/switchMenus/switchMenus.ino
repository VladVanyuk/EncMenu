#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MakeMenu.h>

Menu *menu_active;



LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);

Menu menu_main(&lcd); // menu obj
Menu menu_config(&lcd);
void switchMenu()
{
    if (menu_active == &menu_main)
    {
        menu_active->mFlags.menuActive = false;
        menu_active = &menu_config;
        menu_active->mFlags.menuActive = true;
    }else if (menu_active == &menu_config)
    {
        menu_active->mFlags.menuActive = false;
        menu_active = &menu_main;
        menu_active->mFlags.menuActive = true;
    }

    menu_active->mFlags.menuUpdate = true;
    
    
}

#include "GyverEncoder.h"   // https://github.com/GyverLibs/GyverEncoder
#include "encoder.h"

uint16_t ledBright1 = 10;
uint16_t ledBright2 = 20;


void lcdSetup(){
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
}

void menuSetup(){
  //***************MAIN menu screen************
  menu_main.SetNames(0, DEFAULT_LINE, "Menu1 Screen");
  menu_main.SetNames(0, 1, "1.Led1", &ledBright1, BARGRAPH);

    menu_main.SetFunc3(0, 1, switchMenu);

    menu_main.ActivateBacklightTimer();


  //***************CONFIG menu screen************
  menu_config.SetNames(0, DEFAULT_LINE, "Conf Menu");
  menu_config.SetNames(0, 1, "1.Led2", &ledBright2, BARGRAPH);
  menu_config.SetFunc3(0, 1, switchMenu);


    menu_config.SetBacklightTimeout(7000);
  lcd.clear();

  menu_active = &menu_main;

  menu_active->mFlags.menuActive = true;
  menu_active->mFlags.menuUpdate = true;
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

    menu_active->MenuHandler();
  
  
}