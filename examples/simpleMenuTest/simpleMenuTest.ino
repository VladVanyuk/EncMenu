#include <Arduino.h>
#define DEBUG_MENU_SERIAL 1
#include <EncMenu.h>

#define BAUD_CUSTOM 9600

// LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);
// EncButton encB(CLK, DT, SW, INPUT_PULLUP);
Menu *menu_main;


void setup() 
{
  Serial.begin(BAUD_CUSTOM);
  delay(500); // Wait a time fot serial
  Serial.println("Setup");


  // menu_main = new Menu(&lcd, &encB);
  menu_main = new Menu();
  menu_main->SetLineValues(0, 0, "Main Menu");
  menu_main->SetLineValues(0, 1, "line1");
  menu_main->SetLineValues(0, 2, "line2");

  menu_main->mFlags.menuActive_f = true;
  menu_main->MenuUpdate();
}

void loop() {
  menu_main->MenuHandler();

}

