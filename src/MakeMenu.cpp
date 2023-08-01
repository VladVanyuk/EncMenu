#include "MakeMenu.h"
#include "MenuSymbols.h"

#ifndef USE_STANDART_LCD
Menu::Menu(LiquidCrystal_I2C *lcd)
#else
Menu::Menu(LiquidCrystal *lcd)
#endif
{
    _lcd = lcd;

    mFlags.menuActive = false;
    mFlags.menuUpdate = false;
    mFlags.lineSelected = false;
    mFlags.bar_chars_inited = false;
    mFlags.num_chars_inited = false;
    mFlags.backlight_enabled = false;

    for (uint8_t i = 0; i < QUANTITY_SCREENS; i++)
    { // Resetting all arrays
        for (uint8_t j = 0; j < Lines; j++)
        {

#if defined(__AVR__)
            screen[i][j] = '\0';
            obj_indicator[i][j] = nullptr;
            obj_pointf1[i][j] = nullptr;
            obj_pointf2[i][j] = nullptr;
            obj_pointf3[i][j] = nullptr;
#elif defined(ESP8266)
            screen[i][j] = 0;
#endif

            indicator[i][j] = 0;
            pointf1[i][j] = 0;
            pointf2[i][j] = 0;
            pointf3[i][j] = 0;
            line_types[i][j] = NORMAL;
        }
    }
}

#if defined(__AVR__)
void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t ind)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = ind;
    line_types[scr][line] = NORMAL;
}

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t ind, MENU_LINE_TYPE line_type)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = ind;
    line_types[scr][line] = line_type;
}
#elif defined(ESP8266)
void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t *ind)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = ind;
}

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, bool *ind)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = (uint16_t *)ind;
    line_types[scr][line] = SWITCH;
}

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t *ind, MENU_LINE_TYPE line_type)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = ind;
    line_types[scr][line] = line_type;
}
#endif

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name)
{
    screen[scr][line] = item_name;
    line_types[scr][line] = NO_VALUE;
}

#if defined(__AVR__)
void Menu::SetFunc1(uint8_t scr, uint8_t line, void *p)
{
    pointf1[scr][line] = p;
}

void Menu::SetFunc1(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object)
{
    if ((obj_indicator[scr][line] != NULL) and (obj_indicator[scr][line] != object))
    {
        return;
    }
    else
    {
        obj_indicator[scr][line] = object;
    }

    obj_pointf1[scr][line] = listener;
}
#elif defined(ESP8266)
void Menu::SetFunc1(uint8_t scr, uint8_t line, std::function<void(void)> p)
{
    pointf1[scr][line] = p;
}
#endif

#if defined(__AVR__)
void Menu::SetFunc2(uint8_t scr, uint8_t line, void *p)
{
    pointf2[scr][line] = p;
}

void Menu::SetFunc2(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object)
{
    if ((obj_indicator[scr][line] != NULL) and (obj_indicator[scr][line] != object))
    {
        return;
    }
    else
    {
        obj_indicator[scr][line] = object;
    }
    obj_pointf2[scr][line] = listener;
}
#elif defined(ESP8266)
void Menu::SetFunc2(uint8_t scr, uint8_t line, std::function<void(void)> p)
{
    pointf2[scr][line] = p;
}
#endif

#if defined(__AVR__)
void Menu::SetFunc3(uint8_t scr, uint8_t line, void *p)
{
    pointf3[scr][line] = p;
}

void Menu::SetFunc3(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object)
{
    if ((obj_indicator[scr][line] != NULL) and (obj_indicator[scr][line] != object))
    {
        return;
    }

    obj_indicator[scr][line] = object;
    obj_pointf3[scr][line] = listener;
}
#elif defined(ESP8266)
void Menu::SetFunc3(uint8_t scr, uint8_t line, std::function<void(void)> p)
{
    pointf3[scr][line] = p;
}
#endif

void Menu::RunFunction1()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if (obj_pointf1[tmp_scr][tmp_line] != nullptr)
    {
        obj_pointf1[tmp_scr][tmp_line](obj_indicator[tmp_scr][tmp_line]);
    }
    else if ((pointf1[tmp_scr][tmp_line]) != 0)
    {
        pointf1[tmp_scr][tmp_line]();
    }
}

void Menu::RunFunction2()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if (obj_pointf2[tmp_scr][tmp_line] != nullptr)
    {
        obj_pointf2[tmp_scr][tmp_line](obj_indicator[tmp_scr][tmp_line]);
    }
    else if ((pointf2[tmp_scr][tmp_line]) != 0)
    {
        pointf2[tmp_scr][tmp_line]();
    }
}

bool Menu::CheckFunction3()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if (obj_pointf3[tmp_scr][tmp_line] != NULL)
    {
        return true;
    }

    if ((pointf3[tmp_scr][tmp_line]) != 0) // nullptr
    {
        return true;
    }

    return false;
}

void Menu::RunFunction3()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();
    if (obj_pointf3[tmp_scr][tmp_line] != NULL)
    {
        obj_pointf3[tmp_scr][tmp_line](obj_indicator[tmp_scr][tmp_line]);
    }
    else if ((pointf3[tmp_scr][tmp_line]) != 0) // nullptr
    {
        pointf3[tmp_scr][tmp_line]();
    }
}

void Menu::SetLineType(uint8_t scr, uint8_t line, MENU_LINE_TYPE line_type)
{
    line_types[scr][line] = line_type;
}

void Menu::setup_progressbar()
{
    _lcd->createChar(0, bar_start_0);
    _lcd->createChar(1, bar_start_1);
    _lcd->createChar(2, bar_middle_0);
    _lcd->createChar(3, bar_middle_1);
    _lcd->createChar(4, bar_middle_2);
    _lcd->createChar(5, bar_end_0);
    _lcd->createChar(6, bar_end_1);
    mFlags.bar_chars_inited = true;
    mFlags.num_chars_inited = false;
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
        remain = BAR_END_POSITION - start_col - 1;
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
                _lcd->write(6);
            }
            else
            {
                _lcd->write(5);
            }
        }
        else
        {
            if (bar_fill >= 2)
            {
                _lcd->write(4);
                bar_fill -= 2;
            }
            else if (bar_fill == 1)
            {
                _lcd->write(3);
                bar_fill -= 1;
            }
            else
            {
                _lcd->write(2);
            }
        }
    }
}

uint8_t Menu::GetScreen()
{
    return (scr_num);
}

uint8_t Menu::GetLine()
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

uint8_t Menu::GetScreensCount()
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < QUANTITY_SCREENS; i++)
    {
#if defined(__AVR__)

        if (screen[i][0] != '\0')
        {
            count++;
        }
#elif defined(ESP8266)
        if (screen[i][0] != 0)
        {
            count++;
        }
#endif
    }
    return count;
}

void MakeMenu(uint8_t f, uint8_t s)
  {
    debugI(F("MakeMenu begin"));

    if (s == 0)
    {
      focuce = 0;

      if (GetScreen() == 0)
      {
        scr_num = GetScreensCount() - 1;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
      }
      else
      {
        scr_num--;
      }
    }
    else if (s == 1)
    {
      scr_num++;
      focuce = 0;
    }
    // check if we have gone beyond the size of the array, if we have gone we return to the null element
    if (scr_num > QUANTITY_SCREENS)
    {
      scr_num = 0;
    }

    if (f == 0)
    {
      focuce--;
    }
    else if (f == 1)
    {
      focuce++;
    }
    focuce = constrain(focuce, 1, (GetLinesCount(scr_num)));

    // The null character signals the end of the list of strings/screens
    if (screen[scr_num][0] == '\0')
    {
      scr_num = 0;
      MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }

    if (screen[scr_num][focuce] == '\0')
    {
      focuce--;
      MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    }
    if (focuce < 1)
      focuce = 1;

    getActiveMenu();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(screen[scr_num][0]);

#if (CLOCK_HANDLER == 1)
    if (aMenu.clock_setup_menu_active == true)
    {
      lcd.print(currentTime.hour);
      lcd.print(':');
      lcd.print(currentTime.minute);
      lcd.print(':');
      lcd.print(currentTime.sec);
    }
#endif

    lcd.setCursor(LCD_COL_COUNT - 2, 0);
    lcd.print('#');
    lcd.print(scr_num + 1);

    int scr_lines = GetLinesCount(scr_num);
    if (scr_lines < LCD_ROW_COUNT)
      scr_lines = LCD_ROW_COUNT;
    debugI(F("scr_lines =%d"), scr_lines);

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

    uint8_t line_index = focuce - start_row_print;
    uint8_t line_to_print = 1;

    for (uint8_t i = 0; i < rows_to_update; i++)
    {
      printed_lines_indicator[i] = 0;
    }

    // display the lines in order until we hit the null character
    for (line_index = (focuce - start_row_print); line_index < (focuce + untill_last_row); line_index++)
    {
      printed_lines_indicator[line_to_print] = indicator[scr_num][line_index];
      lcd.setCursor(1, line_to_print);
      lcd.print(screen[scr_num][line_index]);

      if (aMenu.fan_menu_active || aMenu.leds_menu_active)
      {
        draw_progressbar((uint16_t)*indicator[scr_num][line_index], line_to_print, 8);
      }
      else if (aMenu.password_menu_active)
      {
        setup_lock_chars();
      }

      if ((*indicator[scr_num][line_index] == 0)
#if (CLOCK_HANDLER == 1)
          && (scr_num != CLOCK_SETUP_SCREEN)
#endif
#if (KEYPAD_HANDLER == 1)
          && (scr_num != PASSWORD_SETUP_SCREEN)
#endif
      )
      {
        lcd.setCursor(17, line_to_print);
        lcd.print("OFF");
      }
#if (BULBS_HANDLER == 1)
      else if ((scr_num == BULBS_SCREEN) && (*indicator[scr_num][line_index] >= 1))
      {
        lcd.setCursor(17, line_to_print);
        lcd.print(" ON");
      }
#endif
#if (CLOCK_HANDLER == 1)
      else if (scr_num == CLOCK_SETUP_SCREEN)
      {
        lcd.setCursor(16, line_to_print);
        lcd.print((uint16_t)*indicator[scr_num][line_index]);
      }
#endif
      else
      {
        lcd.setCursor(17, line_to_print);
        lcd.print((uint16_t)*indicator[scr_num][line_index]);
      }

      if ((screen[scr_num][focuce + 3] != '\0') && (screen[scr_num][focuce + 1] != '\0'))
      {
        lcd.createChar(7, down_arrow);
        lcd.setCursor(0, 3);
        lcd.write(7);
      }

      if (screen[scr_num][line_index + 1] == '\0')
        break;
      line_to_print++;
    }

    lcd.setCursor(0, focuce_char);
    char cursor_icon = '>';
    if (mFlags.lineSelected == true)
    {
      lcd.blink();
      cursor_icon = 0x7E; // ->
    }
    else
    {
      lcd.noBlink();
    }
    lcd.print(cursor_icon);

    
    // debugD(F("focuce=%d, focuse_char = %u, untill_last_row =%u, start_row_print =%d, line_index=%u"), focuce, focuce_char, untill_last_row, start_row_print, line_index);
    debugI(F("MakeMenu end"));
  }

  void MenuClearLine(uint8_t row)
  {
    lcd.setCursor(1, row);
    for (uint8_t i = 1; i < LCD_COL_COUNT; i++)
    {
      lcd.print(" ");
    }
  }

  void MakeMenuLine(uint8_t scr, uint8_t line_index)
  {
    if (scr == scr_num)
    {
      uint16_t *ind_p = indicator[scr][line_index];
      uint8_t line_to_print = 0;
      for (line_to_print = 0; line_to_print < rows_to_update; line_to_print++)
      {
        if (printed_lines_indicator[line_to_print] == ind_p)
        {
          MenuClearLine(line_to_print);
          break;
        }
      }

      lcd.setCursor(1, line_to_print);
      lcd.print(screen[scr_num][line_index]);

      if (aMenu.fan_menu_active || aMenu.leds_menu_active)
      {
        draw_progressbar((uint16_t)*indicator[scr_num][line_index], line_to_print, 8);
      }
      else if (aMenu.password_menu_active)
      {
        setup_lock_chars();
      }

      if ((*indicator[scr_num][line_index] == 0)
#if (CLOCK_HANDLER == 1)
          && (scr_num != CLOCK_SETUP_SCREEN)
#endif
#if (KEYPAD_HANDLER == 1)
          && (scr_num != PASSWORD_SETUP_SCREEN)
#endif
      )
      {
        lcd.setCursor(17, line_to_print);
        lcd.print("OFF");
      }
#if (BULBS_HANDLER == 1)
      else if ((scr_num == BULBS_SCREEN) && (*indicator[scr_num][line_index] >= 1))
      {
        lcd.setCursor(17, line_to_print);
        lcd.print(" ON");
      }
#endif
#if (CLOCK_HANDLER == 1)
      else if (scr_num == CLOCK_SETUP_SCREEN)
      {
        lcd.setCursor(16, line_to_print);
        lcd.print((uint16_t)*indicator[scr_num][line_index]);
      }
#endif
      else
      {
        lcd.setCursor(17, line_to_print);
        lcd.print((uint16_t)*indicator[scr_num][line_index]);
      };
    }
  }

  void MakeMenuCurrentLine()
  {
    MakeMenuLine(scr_num, focuce);
    lcd.setCursor(0, focuce);
    char cursor_icon = '>';
    if (mFlags.lineSelected == true)
    {
      lcd.blink();
      cursor_icon = 0x7E; // ->
    }
    else
    {
      lcd.noBlink();
    }
    lcd.print(cursor_icon);
  }

/*
void Menu::MenuClearLine(uint8_t row)
{
    _lcd->setCursor(1, row);
    for (uint8_t i = 0; i < LCD_COL_COUNT; i++)
    {
        _lcd->print(" ");
    }
}

void Menu::PrintOneLine(uint8_t scr, uint8_t line_index, uint8_t line_to_print)
{

    printed_lines_indicator[line_to_print] = indicator[scr][line_index];
    _lcd->setCursor(1, line_to_print);
    _lcd->print(screen[scr][line_index]);

    if (line_types[scr][line_index] == SWITCH) // if SWITCH type value
    {
        if (*indicator[scr][line_index] == 0)
        {
            _lcd->setCursor(IND_POSITION, line_to_print);
            _lcd->print("OFF");
        }
        else if (*indicator[scr][line_index] >= 1)
        {
            _lcd->setCursor(IND_POSITION, line_to_print);
            _lcd->print(" ON");
        }
    }
    else if (line_types[scr][line_index] == BARGRAPH) // if progress bar type
    {
        draw_progressbar((uint16_t)*indicator[scr][line_index], line_to_print, START_BAR_POSITION);
        _lcd->setCursor(IND_POSITION, line_to_print);
        _lcd->print((uint16_t)*indicator[scr][line_index]);
    }
    else if (line_types[scr][line_index] == NORMAL)
    {
        _lcd->setCursor(IND_POSITION, line_to_print);
        _lcd->print((uint16_t)*indicator[scr][line_index]);
    }
}

void Menu::DisplayFocuceLines()
{
    int scr_lines = GetLinesCount(scr_num);
    if (scr_lines < LCD_ROW_COUNT)
        scr_lines = LCD_ROW_COUNT;

    uint8_t untill_last_row = scr_lines - focuce;
    int8_t start_row_print = ROWS_TO_UPDATE - untill_last_row;
    int focuce_char = LCD_ROW_COUNT - untill_last_row;
    if (start_row_print < 0)
    {
        start_row_print = 0;
    }

    if (untill_last_row > ROWS_TO_UPDATE)
    {
        untill_last_row = ROWS_TO_UPDATE;
        focuce_char = 1;
    }

    uint8_t line_index = focuce - start_row_print;
    uint8_t line_to_print = 1;

    // display the lines in order until we hit the null character
    for (line_index = (focuce - start_row_print); line_index < (focuce + untill_last_row); line_index++)
    {
        PrintOneLine(scr_num, line_index, line_to_print);


#if LCD_ROW_COUNT > 2
#if defined(__AVR__)
    if ((screen[scr_num][focuce + 3] != '\0') && (screen[scr_num][focuce + 1] != '\0'))
    {
        _lcd->createChar(7, down_arrow);
        _lcd->setCursor(0, 3);
        _lcd->write(7);
    }
#elif defined(ESP8266)
    if ((screen[scr_num][focuce + 3] != 0) && (screen[scr_num][focuce + 1] != 0))
    {
        _lcd->createChar(7, down_arrow);
        _lcd->setCursor(0, 3);
        _lcd->write(7);
    }
#endif
#endif

#if defined(__AVR__)
        if (screen[scr_num][line_index + 1] == '\0')
            break;
#elif defined(ESP8266)
        if (screen[scr_num][line_index + 1] == 0)
            break;
#endif

        line_to_print++;
    }

    _lcd->setCursor(0, focuce_char);
    if (mFlags.lineSelected == true)
    {
        _lcd->blink();
        cursor_icon = 0x7E;
    }
    else
    {
        _lcd->noBlink();
        cursor_icon = '>';
    }
    //_lcd->print(mFlags.lineSelected ? edit_cursor_icon : cursor_icon);
    _lcd->print(cursor_icon);
}

void Menu::MakeMenu(LINE_TARGET_TYPE line_target, MENU_TARGET_TYPE screen_target)
{
    if (screen_target == PREVIOS_SCREEN)
    {
        focuce = 1;
        if (GetScreen() == 0)
        {
            scr_num = GetScreensCount() - 1;
            MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
            return;
        }
        else
        {
            scr_num--;
        }
    }
    else if (screen_target == NEXT_SCREEN)
    {
        scr_num++;
        focuce = 1;
    }
    // check if we have gone beyond the size of the array, if we have gone we return to the null element
    if (scr_num > QUANTITY_SCREENS)
    {
        scr_num = 0;
    }

    if (line_target == PREVIOS_LINE)
    {
        focuce--;
    }
    else if (line_target == NEXT_LINE)
    {
        focuce++;
    }
    focuce = constrain(focuce, 1, (GetLinesCount(scr_num)));

// The null character signals the end of the list of strings/screens
#if defined(__AVR__)
    if (screen[scr_num][0] == '\0')
    {
        scr_num = 0;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
        return;
    }
#elif defined(ESP8266)
    if (screen[scr_num][0] == 0)
    {
        scr_num = 0;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
        return;
    }
#endif

#if defined(__AVR__)
    if (screen[scr_num][focuce] == '\0')
    {
        focuce--;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
        return;
    }
#elif defined(ESP8266)
    if (screen[scr_num][focuce] == 0)
    {
        focuce--;
        MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
        return;
    }
#endif

    if (focuce < 1)
        focuce = 1;

    _lcd->clear();
    _lcd->setCursor(0, 0);
    _lcd->print(screen[scr_num][0]);
    _lcd->setCursor(LCD_COL_COUNT - 2, 0);
    _lcd->print(screen_ind_icon);
    _lcd->print(scr_num + 1);

    for (uint8_t i = 0; i < ROWS_TO_UPDATE; i++)
    {
        printed_lines_indicator[i] = 0;
    }

    DisplayFocuceLines();
}

void Menu::MakeMenuLine(uint8_t scr, uint8_t line_index)
{
    if (scr == scr_num)
    {
        uint16_t *ind_p = indicator[scr][line_index];
        uint8_t line_to_print = 0;
        for (line_to_print = 0; line_to_print < ROWS_TO_UPDATE; line_to_print++)
        {
            if (printed_lines_indicator[line_to_print] == ind_p)
            {
                MenuClearLine(line_to_print);
                PrintOneLine(scr, line_index, line_to_print);
                SetMenuActive();
                break;
            }
        }
    }
}

void Menu::MakeMenuCurrentLine()
{
    MakeMenuLine(scr_num, focuce);
}

*/

void Menu::ActivateBacklightTimer()
{
    mFlags.backlight_enabled = true;
    backlight_timer = millis();
}

void Menu::SetBacklightTimeout(uint32_t timeout)
{
    ActivateBacklightTimer();
    backlight_timeout = timeout;
}

void Menu::setup_bigNumbers()
{
    _lcd->createChar(0, leftSide);
    _lcd->createChar(1, upperBar);
    _lcd->createChar(2, rightSide);
    _lcd->createChar(3, leftEnd);
    _lcd->createChar(4, lowerBar);
    _lcd->createChar(5, rightEnd);
    _lcd->createChar(6, middleBar);
    _lcd->createChar(7, lowerEnd);
    mFlags.num_chars_inited = true;
    mFlags.bar_chars_inited = false;
}

void Menu::displayLargeInt(int n, byte x, byte y, byte digits, bool leading)
{
    if (mFlags.num_chars_inited == false)
    {
        setup_bigNumbers();
    }

    boolean isNegative = false;
    if (n < 0)
    {
        isNegative = true;
        n = abs(n);
    }
    byte numString[digits];
    byte index = digits - 1;
    while (index)
    {
        numString[index] = n % 10;
        n /= 10;
        index--;
    }
    numString[0] = n % 10;

    for (int i = 0; i < digits; i++)
    {
        if (numString[i] == 0 && !leading && i < digits - 1)
        {
            clearLargeNumber((i * 3) + x, y);
        }
        else
        {
            displayLargeNumber(numString[i], (i * 3) + x, y);
            leading = true;
        }
    }
}

void Menu::clearLargeNumber(byte x, byte y) /* x is column of upper left corner for large character y is row of upper left corner for large character */
{
    _lcd->setCursor(x, y);
    _lcd->print(F("   "));
    _lcd->setCursor(x, y + 1);
    _lcd->print(F("   "));
}

void Menu::displayLargeNumber(byte n, byte x, byte y) /* n is number to display, x is column of upper left corner for large character
y is row of upper left corner for large character */
{

    if (mFlags.num_chars_inited == false)
    {
        setup_bigNumbers();
    }

    switch (n)
    {
    case 0:
    {
        _lcd->setCursor(x, y);
        _lcd->write(byte(0));
        _lcd->write(1);
        _lcd->write(2);
        _lcd->setCursor(x, y + 1);
        _lcd->write(byte(0));
        _lcd->write(4);
        _lcd->write(2);
        break;
    }
    case 1:
    {
        _lcd->setCursor(x, y);
        _lcd->write(char(254));
        _lcd->write(char(254));
        _lcd->write(2);
        _lcd->setCursor(x, y + 1);
        _lcd->write(char(254));
        _lcd->write(char(254));
        _lcd->write(2);
        break;
    }
    case 2:
    {
        _lcd->setCursor(x, y);
        _lcd->write(3);
        _lcd->write(6);
        _lcd->write(2);
        _lcd->setCursor(x, y + 1);
        _lcd->write(byte(0));
        _lcd->write(4);
        _lcd->write(4);
        break;
    }
    case 3:
    {
        _lcd->setCursor(x, y);
        _lcd->write(3);
        _lcd->write(6);
        _lcd->write(2);
        _lcd->setCursor(x, y + 1);
        _lcd->write(7);
        _lcd->write(4);
        _lcd->write(2);
        break;
    }
    case 4:
    {
        _lcd->setCursor(x, y);
        _lcd->write(byte(0));
        _lcd->write(4);
        _lcd->write(2);
        _lcd->setCursor(x, y + 1);
        _lcd->write(char(254));
        _lcd->write(char(254));
        _lcd->write(2);
        break;
    }
    case 5:
    {
        _lcd->setCursor(x, y);
        _lcd->write(byte(0));
        _lcd->write(6);
        _lcd->write(5);
        _lcd->setCursor(x, y + 1);
        _lcd->write(7);
        _lcd->write(4);
        _lcd->write(2);
        break;
    }
    case 6:
    {
        _lcd->setCursor(x, y);
        _lcd->write(byte(0));
        _lcd->write(6);
        _lcd->write(5);
        _lcd->setCursor(x, y + 1);
        _lcd->write(byte(0));
        _lcd->write(4);
        _lcd->write(2);
        break;
    }
    case 7:
    {
        _lcd->setCursor(x, y);
        _lcd->write(1);
        _lcd->write(1);
        _lcd->write(2);
        _lcd->setCursor(x, y + 1);
        _lcd->write(char(254));
        _lcd->write(char(254));
        _lcd->write(2);
        break;
    }
    case 8:
    {
        _lcd->setCursor(x, y);
        _lcd->write(byte(0));
        _lcd->write(6);
        _lcd->write(2);
        _lcd->setCursor(x, y + 1);
        _lcd->write(byte(0));
        _lcd->write(4);
        _lcd->write(2);
        break;
    }
    case 9:
    {
        _lcd->setCursor(x, y);
        _lcd->write(byte(0));
        _lcd->write(6);
        _lcd->write(2);
        _lcd->setCursor(x, y + 1);
        _lcd->write(7);
        _lcd->write(4);
        _lcd->write(2);
        break;
    }
    }
}

void Menu::SetMenuActive()
{
    mFlags.menuActive = true;
    if (mFlags.backlight_enabled == true)
    {
        _lcd->setBacklight(HIGH);
        backlight_timer = millis();
    }
}

void Menu::MenuHandler()
{
    if (mFlags.menuActive == true)
    {
        if (mFlags.menuUpdate == true)
        {
            MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
            SetMenuActive();
            mFlags.menuUpdate = false;
        }

        if ((mFlags.backlight_enabled == true) and (millis() - backlight_timer > backlight_timeout))
        {
            _lcd->setBacklight(LOW);
            backlight_timer = 0;
        }
    }
}
