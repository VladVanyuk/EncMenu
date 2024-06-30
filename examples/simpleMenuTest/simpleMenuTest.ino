#include <Arduino.h>
#define DEBUG_MENU_SERIAL 1

#include <LiquidCrystal_I2C.h>
#include <EncButton.h>

#include <EncMenu.h>

#define BAUD_CUSTOM 9600

#define LCD_I2C_ADDRESS 0x27
#define LCD_ROW_COUNT       4    // Number of Rows
#define LCD_COL_COUNT       20

#define CLK 2
#define DT 3
#define SW 4


LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);

EncButton encB(CLK, DT, SW, INPUT_PULLUP);

Menu test_menu(&lcd);

Menu *menu_main = NULL;


void setup() 
{
  Serial.begin(BAUD_CUSTOM);
  delay(1000); // Wait a time fot serial
  // Serial.flush();
  // Serial.println();
  Serial.println("Setup");

  delay(2000);


  
  lcd.init();
  lcd.backlight();
  // backlight = true;
  // bigNum.begin(); // set up BigNumbers for lcd
  lcd.home();
  lcd.print("Initializing...");
  
  delay(1000);

  menu_main = &test_menu;
  menu_main->SetLineValues(0, 0, "Main Menu");
  menu_main->SetLineValues(0, 1, "line1");
  menu_main->SetLineValues(0, 2, "line2");

  menu_main->mFlags.menuActive_f = true;
  menu_main->MenuUpdate();
}

void loop() {
  menu_main->MenuHandler();

}

