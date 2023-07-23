#include "MakeMenu.h"


byte START_DIV_0_OF_1[8] = {
    B01111,
    B11000,
    B10000,
    B10000,
    B10000,
    B10000,
    B11000,
    B01111};

byte START_DIV_1_OF_1[8] = {
    B01111,
    B11000,
    B10011,
    B10111,
    B10111,
    B10011,
    B11000,
    B01111};

byte DIV_0_OF_2[8] = {
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111};

byte DIV_1_OF_2[8] = {
    B11111,
    B00000,
    B11000,
    B11000,
    B11000,
    B11000,
    B00000,
    B11111};

byte DIV_2_OF_2[8] = {
    B11111,
    B00000,
    B11011,
    B11011,
    B11011,
    B11011,
    B00000,
    B11111};

byte END_DIV_0_OF_1[8] = {
    B11110,
    B00011,
    B00001,
    B00001,
    B00001,
    B00001,
    B00011,
    B11110};

byte END_DIV_1_OF_1[8] = {
    B11110,
    B00011,
    B11001,
    B11101,
    B11101,
    B11001,
    B00011,
    B11110};


Menu::Menu(LiquidCrystal_I2C* lcd)
{
    _lcd = lcd;

    mFlags.bar_chars_inited = false;
    mFlags.lineSelected = false;
    mFlags.menuActive = false;
    mFlags.menuUpdate = false;

    for (uint8_t i = 0; i < QUANTITY_SCREENS; i++)
    { // Resetting all arrays
        for (uint8_t j = 0; j < Lines; j++)
        {
            #if defined(ESP8266)
            screen[i][j] = 0;
            #elif defined(__AVR__)
            screen[i][j] = '\0';
            #endif
        
            indicator[i][j] = 0;
            pointf1[i][j] = 0;
            pointf2[i][j] = 0;
            pointf3[i][j] = 0;

            drawBar[i][j] = false;
            boolValue[i][j] = false;
        }
    }
}


#if defined(__AVR__)
void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t ind)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = ind;
}
#elif defined(ESP8266)
void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t* ind)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = ind;
}

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, bool* ind)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = (uint16_t*)ind;
}
#endif

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name)
{
    screen[scr][line] = item_name;
}


#if defined(__AVR__)
void Menu::SetFunc1(uint8_t scr, uint8_t line, void *p)
{
    pointf1[scr][line] = p;
}
#elif defined(ESP8266)
void Menu::SetFunc1(uint8_t scr, uint8_t line, std::function<void(void)>p)
{
    pointf1[scr][line] = p;
}
#endif

#if defined(__AVR__)
void Menu::SetFunc2(uint8_t scr, uint8_t line, void *p)
{
    pointf2[scr][line] = p;
}
#elif defined(ESP8266)
void Menu::SetFunc2(uint8_t scr, uint8_t line, std::function<void(void)>p)
{
    pointf2[scr][line] = p;
}
#endif

#if defined(__AVR__)
void Menu::SetFunc3(uint8_t scr, uint8_t line, void *p)
{
    pointf3[scr][line] = p;
}
#elif defined(ESP8266)
void Menu::SetFunc3(uint8_t scr, uint8_t line, std::function<void(void)>p)
{
    pointf3[scr][line] = p;
}
#endif

void Menu::SetProgressBarLine(uint8_t scr, uint8_t line, bool state)
{
    drawBar[scr][line] = state;
}

void Menu::SetBoolValueLine(uint8_t scr, uint8_t line, bool state)
{
    boolValue[scr][line] = state;
}

void Menu::setup_progressbar()
{
    _lcd->createChar(0, START_DIV_0_OF_1);
    _lcd->createChar(1, START_DIV_1_OF_1);
    _lcd->createChar(2, DIV_0_OF_2);
    _lcd->createChar(3, DIV_1_OF_2);
    _lcd->createChar(4, DIV_2_OF_2);
    _lcd->createChar(5, END_DIV_0_OF_1);
    _lcd->createChar(6, END_DIV_1_OF_1);
    mFlags.bar_chars_inited = true;
}

void Menu::draw_progressbar(uint16_t percent, byte start_row, byte start_col)
{
    byte remain;
    if (start_col > LCD_COL_COUNT)
    { // BAR_END
        return;
    }
    else
    {
        remain = BAR_END_POSITION - start_col-1;
    }

    if (!mFlags.bar_chars_inited)
    {
        setup_progressbar();
    }

    _lcd->setCursor(start_col, start_row);
    byte bar_fill = map(percent, 0, 100, 0, remain * 2 - 2);

    for (byte i = 0; i < remain; ++i)
    {
        if (i == 0)
        {
            if (bar_fill > 0)
            {
                _lcd->write(1);
                bar_fill -= 1;
            }
            else
            {
                _lcd->write((byte)0); // debut
            }
        }
        else if (i == remain - 1)
        {
            if (bar_fill > 0)
            {
                _lcd->write(6); // Char fin 1 / 1
            }
            else
            {
                _lcd->write(5); // Char fin 0 / 1
            }
        }
        else
        {
            if (bar_fill >= 2)
            {
                _lcd->write(4); // Char div 2 / 2
                bar_fill -= 2;
            }
            else if (bar_fill == 1)
            {
                _lcd->write(3); // Char div 1 / 2
                bar_fill -= 1;
            }
            else
            {
                _lcd->write(2); // Char div 0 / 2
            }
        }
    }
}

uint8_t Menu::GetScreen()
{
    return (scr_num);
}

uint8_t Menu::GetString()
{
    return (focuce);
}

uint8_t Menu::GetLinesCount(uint8_t scr)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < Lines; i++)
    {
        #if defined(__AVR__)
        if (screen[scr][i] != '\0')
        {
            count++;
        }
        #elif defined(ESP8266)
        if (screen[scr][i] != 0) 
        {
            count++;
        }
        #endif
    }
    return count;
}

void Menu::RunFunction1()
{
    if ((pointf1[GetScreen()][GetString()]) != 0)
    {
        pointf1[GetScreen()][GetString()]();
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
}


void Menu::RunFunction2()
{
    if ((pointf2[GetScreen()][GetString()]) != 0)
    {
        pointf2[GetScreen()][GetString()]();
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
}

bool Menu::CheckFunction3()
{
    if ((pointf3[GetScreen()][GetString()]) != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Menu::RunFunction3()
{
    if ((pointf3[GetScreen()][GetString()]) != 0)
    {
        pointf3[GetScreen()][GetString()]();
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
}

void Menu::DisplayFocuceLines()
{
    int scr_lines = GetLinesCount(scr_num);
    if (scr_lines < LCD_ROW_COUNT)
        scr_lines = LCD_ROW_COUNT;

    uint8_t untill_last_row = scr_lines - focuce;
    int8_t start_row_print = rows_to_update - untill_last_row;
    int focuce_char = LCD_ROW_COUNT - untill_last_row;
    if (start_row_print < 0)
    {
        start_row_print = 0;
    }

    if (untill_last_row > rows_to_update)
    {
        untill_last_row = rows_to_update;
        focuce_char = 1;
    }

    _lcd->setCursor(0, focuce_char);
    _lcd->print('>');

    uint8_t line_index = focuce - start_row_print;
    uint8_t line_to_print = 1;

    // display the lines in order until we hit the null character
    for (line_index = (focuce - start_row_print); line_index < (focuce + untill_last_row); line_index++)
    {
        _lcd->setCursor(1, line_to_print);
        _lcd->print(screen[scr_num][line_index]);

        // if BOOL value
        if (boolValue[scr_num][line_index])
        {
            if (*indicator[scr_num][line_index] == 0)
            {
                _lcd->setCursor(IND_POSITION, line_to_print);
                _lcd->print("OFF");
            }
            else if (*indicator[scr_num][line_index] >= 1)
            {
                _lcd->setCursor(IND_POSITION, line_to_print);
                _lcd->print(" ON");
            }
        }
        else
        {
            _lcd->setCursor(IND_POSITION, line_to_print);
            _lcd->print((uint16_t)*indicator[scr_num][line_index]);
        }

        if (drawBar[scr_num][line_index])
        {
            draw_progressbar((uint16_t)*indicator[scr_num][line_index], line_to_print, START_BAR_POSITION);
        }

        #if defined(__AVR__)
        if (screen[scr_num][line_index + 1] == '\0')
            break;
        #elif defined(ESP8266)
        if (screen[scr_num][line_index + 1] == 0) 
            break;
        #endif
        line_to_print++;
    }
}

void Menu::MakeMenu(uint8_t f, uint8_t s)
{
    if (f == 0)
    {
        focuce--;
    }
    else if (f == 1)
    {
        focuce++;
    }
    focuce = constrain(focuce, 1, (GetLinesCount(scr_num)));
    if (s == 0)
    {
        scr_num--;
    }
    else if (s == 1)
    {
        scr_num++;
    }

    // check if we have gone beyond the size of the array, if we have gone we return to the null element
    if (scr_num > QUANTITY_SCREENS)
    {
        scr_num = 0;
    }

    // The null character signals the end of the list of strings/screens
    #if defined(__AVR__)
    if (screen[scr_num][0] == '\0')
    {
        scr_num = 0;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
    #elif defined(ESP8266)
    if (screen[scr_num][0] == 0) 
    {
        scr_num = 0;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
    #endif

    #if defined(__AVR__)
    if (screen[scr_num][focuce] == '\0')
    {
        focuce--;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
    #elif defined(ESP8266)
    if (screen[scr_num][focuce] == 0)
    {
        focuce--;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
    #endif

    if (focuce < 1)
        focuce = 1;

    _lcd->clear();
    _lcd->setCursor(0, 0);
    _lcd->print(screen[scr_num][0]);
    _lcd->setCursor(18, 0);
    _lcd->print('#');
    _lcd->print(scr_num + 1);

    DisplayFocuceLines();
}
