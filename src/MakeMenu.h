#ifndef MakeMenu_h
#define MakeMenu_h

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#pragma once

#define LCD_I2C_ADDRESS 0x27
#define LCD_ROW_COUNT 4 // Number of Rows
#define LCD_COL_COUNT 20

#ifndef QUANTITY_SCREENS // 5 screens, 6 lines each
#define QUANTITY_SCREENS 5
#endif
#ifndef QUANTITY_LINES
#define QUANTITY_LINES 6
#endif
#define DEFAULT_LINE 0

#define START_BAR_POSITION 8
#define BAR_END_POSITION LCD_COL_COUNT - 3 // 17
#define IND_POSITION LCD_COL_COUNT - 3     // 17

enum LINE_TARGET_E
{
    PREVIOS_LINE = 0,
    NEXT_LINE = 1,
    UPDATE_LINE = 2
};

enum MENU_TARGET_E
{
    PREVIOS_SCREEN = 0,
    NEXT_SCREEN = 1,
    UPDATE_SCREEN = 2
};

class Menu
{
private:
    LiquidCrystal_I2C *_lcd;
    uint8_t const static Lines = (QUANTITY_LINES + 1);
    uint8_t rows_to_update = LCD_ROW_COUNT - 1;
    String screen[QUANTITY_SCREENS][Lines];       // Array of menu titles
    uint16_t focuce = 1, scr_num = 0;             // Focus variables(active line), screen numbers,
    uint16_t *indicator[QUANTITY_SCREENS][Lines]; // Array of indicator pointers (changing value at the end of the line)
public:
    // typedef void (*pointf_t[QUANTITY_SCREENS][Lines]) ();
    struct MenuFlags
    {
        bool menuActive : 1;
        bool lineSelected : 1;
        bool menuUpdate : 1;
        bool bar_chars_inited : 1;
    } mFlags;
    
    void (*pointf1[QUANTITY_SCREENS][Lines])(); // Array of pointers to the first attached function
    void (*pointf2[QUANTITY_SCREENS][Lines])(); // Array of pointers to the second attached function
    void (*pointf3[QUANTITY_SCREENS][Lines])(); // Array of pointers to the third attached function for encoder press button
    bool drawBar[QUANTITY_SCREENS][Lines];
    bool boolValue[QUANTITY_SCREENS][Lines];
    Menu(LiquidCrystal_I2C *lcd);

    /**
     * SetNames
     * Set the screen number, line, menu item name and variable to display at the end of the line
     * @param scr  screen number
     * @param line screen line
     * @param item_name menu item name
     * @param ind  variable to display
     */
    void SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t ind);

    /**
     * SetNames
     * Set the screen number, line, menu item name
     * @param scr  screen number
     * @param line screen line
     * @param item_name menu item name
     */
    void SetNames(uint8_t scr, uint8_t line, String item_name);

    /**
     * SetFunc1
     * Installing the first attached function
     * @param scr  screen number
     * @param line screen line
     * @param p function to attach
     */
    void SetFunc1(uint8_t scr, uint8_t line, void *p);

    /**
     * SetFunc2
     * Installing the second attached function
     * @param scr  screen number
     * @param line screen line
     * @param p function to attach
     */
    void SetFunc2(uint8_t scr, uint8_t line, void *p);

    /**
     * SetFunc3
     * Installing the third attached function
     * @param scr  screen number
     * @param line screen line
     * @param p function to attach
     */
    void SetFunc3(uint8_t scr, uint8_t line, void *p);

    void SetProgressBarLine(uint8_t scr, uint8_t line, bool state);

    void SetBoolValueLine(uint8_t scr, uint8_t line, bool state);

    void setup_progressbar();

    /**
     * draw_progressbar progression.
     *
     * @param percent percentage value to be shown as bar
     * @param start_row which element of menu
     * @param start_col start point of bar
     */
    void draw_progressbar(uint16_t percent, byte start_row, byte start_col);

    /**
     * GetScreen
     * Return screen number
     * @return scr_num
     */
    uint8_t GetScreen();

    /**
     * GetString
     * Return current menu line
     * @return focuce
     */
    uint8_t GetString();

    /**
     * GetLinesCount
     * Return number of current menu lines
     * @param scr current menu
     * @return focuce
     */
    uint8_t GetLinesCount(uint8_t scr);

    /**
     * RunFunction1
     * Run the first attached function (if it exist)
     */
    void RunFunction1();

    /**
     * RunFunction2
     * Run the second attached function (if it exist)
     */
    void RunFunction2();

    /**
     * CheckFunction3
     * Return true if the third attached function exist
     */
    bool CheckFunction3();

    /**
     * RunFunction3
     * Run the third attached function (if it exist)
     */
    void RunFunction3();

    void DisplayFocuceLines();

    /**
     * MakeMenu
     * Drawing the menu on the screen, processing the internal logic of the menu.
     *
     * @param f  responsible for switching the focus row (active line)
     * @param s  for switching the current screen
     *
     * (1-switching to the next screen/line, 0-previous screen/line, 2-updating the current screen without changes)
     *
     * Example: MakeMenu (1, 2); - move to next line, MakeMenu (0, 2); - move to previous line,
     * MakeMenu (2, 1); - move to the next screen, MakeMenu (2, 0); - move to the previous screen.
     */
    void MakeMenu(uint8_t f, uint8_t s);
};

#endif
