#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MakeMenu.h>

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);
Menu menu_main(&lcd); // menu obj

#define RELAY_PIN1 31
#define RELAY_PIN2 32

#define LED_PIN1 40
#define LED_PIN2 41
#define LED_PIN3 42
#define LED_PIN4 43

#define JOYSTICK_BTN 34

uint16_t state_relay1 = LOW; 
uint16_t state_relay2 = LOW; 

uint16_t ledBright1 = 0;
uint16_t ledBright2 = 0;
uint16_t ledBright3 = 0;
uint16_t ledBright4 = 0;

int analogX = 0;
int analogY = 0;
int joystick_bnt_val = 0;

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

void brigh_up_led1 () {
  ledBright1 += 10;
  constrain (ledBright1, 0, 255);
  analogWrite(LED_PIN1, ledBright1);
}

void brigh_down_led1 () {
  ledBright1 -= 10;
  constrain (ledBright1, 0, 255);
  analogWrite(LED_PIN1, ledBright1);
}

void brigh_up_led2 () {
  ledBright2 += 10;
  constrain (ledBright2, 0, 255);
  analogWrite(LED_PIN2, ledBright2);
}

void brigh_down_led2 () {
  ledBright2 -= 10;
  constrain (ledBright2, 0, 255);
  analogWrite(LED_PIN2, ledBright2);
}

void brigh_up_led3 () {
  ledBright3 += 10;
  constrain (ledBright3, 0, 255);
  analogWrite(LED_PIN3, ledBright3);
}

void brigh_down_led3 () {
  ledBright3 -= 10;
  constrain (ledBright3, 0, 255);
  analogWrite(LED_PIN3, ledBright3);
}

void brigh_up_led4 () {
  ledBright4 += 10;
  constrain (ledBright4, 0, 255);
  analogWrite(LED_PIN4, ledBright4);
}

void brigh_down_led4 () {
  ledBright4 -= 10;
  constrain (ledBright4, 0, 255);
  analogWrite(LED_PIN4, ledBright4);
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

  //***************Relay screen************
  menu_main.SetNames(1, DEFAULT_LINE, "Relay Screen");
  menu_main.SetNames(1, 1, "1.Relay1", &state_relay1);
  menu_main.SetNames(1, 2, "2.Relay2", &state_relay2);
  menu_main.SetBoolValueLine(2, 1, true);
  menu_main.SetBoolValueLine(2, 2, true);

  menu_main.SetFunc3(2, 1, toggle_relay1);
  menu_main.SetFunc3(2, 2, toggle_relay2);


  lcd.clear();
  menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN); // Update information on the screen
  menu_main.mFlags.menuActive = true;
}

void joystickHandler(){
  analogX = analogRead(A1);
  analogY = analogRead(A2);

  analogX = map(analogX, 0, 1023, -512, 512);
  analogY = map(analogX, 0, 1023, -512, 512);

  if (analogY < (-400))
  {
    menu_main.MakeMenu(UPDATE_LINE, PREVIOS_SCREEN);
  }else if (analogY > 400)
  {
    menu_main.MakeMenu(UPDATE_LINE, NEXT_SCREEN);
  }

  if (analogX < (-400))
  {
    menu_main.MakeMenu(NEXT_LINE, UPDATE_SCREEN);
  }else if (analogX > 400)
  {
    menu_main.MakeMenu(PREVIOS_LINE, UPDATE_SCREEN);
  }

  if (digitalRead(34))
  {
    /* code */
  }
   
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

  joystick_bnt_val = digitalRead(34);
  analogX = analogRead(A1);
  analogY = analogRead(A2);

  analogX = map(analogX, 0, 1023, -512, 512);
  analogY = map(analogX, 0, 1023, -512, 512);

  lcdSetup();
  menuSetup();
}

void loop()
{
  encoderHandler();

  if(menu_main.mFlags.menuUpdate == true){
    menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    menu_main.mFlags.menuUpdate = false;
  }
  
}