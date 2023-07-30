#ifndef MakeMenu_h
#define MakeMenu_h

// #pragma once
#include <Arduino.h>
#include <Wire.h>

#ifndef USE_STANDART_LCD
#include <LiquidCrystal_I2C.h>
#define LCD_I2C_ADDRESS 0x27
#else
#include <LiquidCrystal.h>
#endif

#ifndef LCD_ROW_COUNT
#define LCD_ROW_COUNT 4 // Number of Rows
#endif

#ifndef LCD_COL_COUNT
#define LCD_COL_COUNT 20
#endif

#ifndef QUANTITY_SCREENS // 5 screens, 6 lines each
#define QUANTITY_SCREENS 5
#endif
#ifndef QUANTITY_LINES
#define QUANTITY_LINES 6
#endif
#define DEFAULT_LINE 0

#define START_BAR_POSITION 8               // to do
#define BAR_END_POSITION LCD_COL_COUNT - 2 // 18
#define IND_POSITION LCD_COL_COUNT - 3     // 17 //to do
#define ROWS_TO_UPDATE LCD_ROW_COUNT - 1

#define DEFAULT_BACKLIGHT_TIMEOUT 30000 // 30 sec

typedef enum
{
    PREVIOS_LINE,
    NEXT_LINE,
    UPDATE_LINE
} LINE_TARGET_TYPE;

typedef enum
{
    PREVIOS_SCREEN,
    NEXT_SCREEN,
    UPDATE_SCREEN
} MENU_TARGET_TYPE;

typedef enum
{
    NORMAL,
    SWITCH,
    BARGRAPH,
    NO_VALUE
} MENU_LINE_TYPE;

struct MenuFlags
    {
        bool menuActive : 1;
        bool menuUpdate : 1;
        bool lineSelected : 1;
        bool bar_chars_inited : 1;
        bool num_chars_inited : 1;
        bool backlight_enabled : 1;
    };

/**
 * @class Menu
 */
class Menu
{
private:
#ifndef USE_STANDART_LCD
    LiquidCrystal_I2C *_lcd;
#else
    LiquidCrystal *_lcd;
#endif

    uint8_t const static Lines = (QUANTITY_LINES + 1);
    uint32_t backlight_timer = 0;
    uint32_t backlight_timeout = DEFAULT_BACKLIGHT_TIMEOUT; // 30 sec default
    uint8_t focuce = 1, scr_num = 0;                       // Focus variables(active line), screen numbers,
    String screen[QUANTITY_SCREENS][Lines];                 // Array of menu titles
    uint16_t *indicator[QUANTITY_SCREENS][Lines];           // Array of indicator pointers (changing value at the end of the line)
    uint16_t *printed_lines_indicator[3] = {0};             // Array of pointers to current printed values
    MENU_LINE_TYPE line_types[QUANTITY_SCREENS][Lines];
    char cursor_icon = '>';
    char edit_cursor_icon = 0x7E; //->
    char screen_ind_icon = '#';

public:
    // typedef void (*pointf_t[QUANTITY_SCREENS][Lines]) ();
    MenuFlags mFlags;

#if defined(__AVR__)
    void (*pointf1[QUANTITY_SCREENS][Lines])(); // Array of pointers to the first attached function
    void (*pointf2[QUANTITY_SCREENS][Lines])(); // Array of pointers to the second attached function
    void (*pointf3[QUANTITY_SCREENS][Lines])(); // Array of pointers to the third attached function for encoder press button

    void *obj_indicator[QUANTITY_SCREENS][Lines];
    void (*obj_pointf1[QUANTITY_SCREENS][Lines])(void *);
    void (*obj_pointf2[QUANTITY_SCREENS][Lines])(void *);
    void (*obj_pointf3[QUANTITY_SCREENS][Lines])(void *);

#elif defined(ESP8266)
    std::function<void(void)> pointf1[QUANTITY_SCREENS][Lines];
    std::function<void(void)> pointf2[QUANTITY_SCREENS][Lines];
    std::function<void(void)> pointf3[QUANTITY_SCREENS][Lines];
#endif

#ifndef USE_STANDART_LCD
    Menu(LiquidCrystal_I2C *lcd);
#else
    Menu(LiquidCrystal *lcd);
#endif

/**
 * @fn SetNames
 * @brief Set the screen number, line, menu item name and variable to display at the end of the line
 * @param scr  screen number
 * @param line screen line
 * @param item_name menu item name
 * @param ind  variable to display
 */
#if defined(ESP8266)
    void SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t *ind);
    void SetNames(uint8_t scr, uint8_t line, String item_name, bool *ind);
    void SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t *ind, MENU_LINE_TYPE line_type);
#elif defined(__AVR__)
    void SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t ind);
    void SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t ind, MENU_LINE_TYPE line_type);
#endif

    /**
     * @fn SetNames
     * @brief Set the screen number, line, menu item name
     * @param scr  screen number
     * @param line screen line
     * @param item_name menu item name
     */
    void SetNames(uint8_t scr, uint8_t line, String item_name);

/**
 * @fn SetFunc1
 * @brief Installing the first attached function
 * @param scr  screen number
 * @param line screen line
 * @param p function to attach
 */
#if defined(__AVR__)
    void SetFunc1(uint8_t scr, uint8_t line, void *p);
    void SetFunc1(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object);
#elif defined(ESP8266)
    void SetFunc1(uint8_t scr, uint8_t line, std::function<void(void)> p);
#endif

/**
 * @fn SetFunc2
 * @brief Installing the second attached function
 * @param scr  screen number
 * @param line screen line
 * @param p function to attach
 */
#if defined(__AVR__)
    void SetFunc2(uint8_t scr, uint8_t line, void *p);
    void SetFunc2(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object);
#elif defined(ESP8266)
    void SetFunc2(uint8_t scr, uint8_t line, std::function<void(void)> p);
#endif

/**
 * @fn SetFunc3
 * @brief Installing the third attached function
 * @param scr  screen number
 * @param line screen line
 * @param p function to attach
 */
#if defined(__AVR__)
    void SetFunc3(uint8_t scr, uint8_t line, void *p);
    void SetFunc3(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object);
#elif defined(ESP8266)
    void SetFunc3(uint8_t scr, uint8_t line, std::function<void(void)> p);
#endif

    /**
     * @fn ActivateBacklightTimer
     * @brief Activate lcd backlight timer for menu
     */
    void ActivateBacklightTimer();

    /**
     * @fn ActivateBacklightTimer
     * @brief Activate lcd backlight timer for menu and set its timeout.
     * Default timeout is:
     *          DEFAULT_BACKLIGHT_TIMEOUT 30000
     */
    void SetBacklightTimeout(uint32_t timeout);

    /**
     * @fn SetLineType
     * @brief Set line type to change its display settings
     * @param scr  screen number
     * @param line screen line to change type
     * @param line_type new line type
     *
     * Possible types:
     *      NORMAL = 0; - by default
     *      BOOL = 1;   - showing only "ON" or "OFF"
     *      BARGRAPH = 2; - percentage progress bar
     *      NO_VALUE = 3; - for lines without indicator value
     */
    void SetLineType(uint8_t scr, uint8_t line, MENU_LINE_TYPE line_type);

    void setup_progressbar();
    void setup_bigNumbers();

    // prints an integer to the display using large characters
    // Parameters: n - the integer to display
    //             x - column of upper left corner of first large character
    //             digits - number of digits of the integer (specifying this allows the digit positions to remain constant when printing out ints of various lengths)
    //             leading - sets if leading zeros are printed or not (false = no, true = yes)

    /**
     * @fn displayLargeInt
     * @brief prints an integer to the display using large characters
     * @param n  the integer to display
     * @param x column of upper left corner of first large character
     * @param y
     * @param digits  number of digits of the integer (specifying this allows the digit positions to remain constant when printing out ints of various lengths)
     * @param leading   sets if leading zeros are printed or not (false = no, true = yes)
     */
    void displayLargeInt(int n, byte x, byte y, byte digits, bool leading);
    void clearLargeNumber(byte x, byte y);
    void displayLargeNumber(byte n, byte x, byte y);

    /**
     * @fn draw_progressbar progression.
     *
     * @param percent percentage value to be shown as bar
     * @param start_row which element of menu
     * @param start_col start point of bar
     */
    void draw_progressbar(uint16_t percent, byte start_row, byte start_col);

    /**
     * @fn GetScreen
     * @brief Return screen number
     * @return scr_num
     */
    uint8_t GetScreen();

    /**
     * @fn GetLine
     * @brief Return current menu line
     * @return focuce
     */
    uint8_t GetLine();

    /**
     * @fn GetLinesCount
     * @brief Return number of current menu lines
     * @param scr current menu
     * @return focuce
     */
    uint8_t GetLinesCount(uint8_t scr);

    uint8_t GetScreensCount();

    /**
     * @fn RunFunction1
     * @brief Run the first attached function (if it exist)
     */
    void RunFunction1();

    /**
     * @fn RunFunction2
     * @brief Run the second attached function (if it exist)
     */
    void RunFunction2();

    /**
     * @fn CheckFunction3
     * @brief Return true if the third attached function exist
     */
    bool CheckFunction3();

    /**
     * @fn RunFunction3
     * @brief Run the third attached function (if it exist)
     */
    void RunFunction3();

    /**
     * @fn MenuClearLine
     * @brief Clearing only one lcd line
     * @param row lcd line to be cleared
     */
    void MenuClearLine(uint8_t row);

    /**
     * @fn PrintOneLine
     * @brief Printing only one lcd line
     * @param scr   screen number index
     * @param line_index  screen line index
     * @param line_to_print lcd line number to print
     */
    void PrintOneLine(uint8_t scr, uint8_t line_index, uint8_t line_to_print);

    void DisplayFocuceLines();

    /**
     * @fn SetMenuActive
     * @brief Activate update menu flag
     */
    void SetMenuActive();

    /**
     * @fn MakeMenu
     * @brief Drawing the menu on the screen, processing the internal logic of the menu.
     *
     * @param line_target  responsible for switching the focus row (active line)
     * @param screen_target  for switching the current screen
     *
     * @example Example:
     * @(1-switching to the next screen/line, 0-previous screen/line, 2-updating the current screen without changes)
     * MakeMenu (1, 2); - move to next line, MakeMenu (0, 2); - move to previous line,
     * MakeMenu (2, 1); - move to the next screen, MakeMenu (2, 0); - move to the previous screen.
     */
    void MakeMenu(LINE_TARGET_TYPE line_target, MENU_TARGET_TYPE screen_target);

    /**
     * @fn MakeMenuLine
     * @brief Drawing the menu line on the screen, processing the internal logic of the menu.
     * @param scr   screen number index
     * @param line_index  screen line index
     */
    void MakeMenuLine(uint8_t scr, uint8_t line_index);

    void MakeMenuCurrentLine();

    void MenuHandler();
};

#endif
