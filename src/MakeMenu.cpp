#include <MakeMenu.h>

Menu::Menu(LiquidCrystal_I2C *lcd)
{
    _lcd = lcd;
    ResetMenu();
}

void Menu::ResetMenu()
{
    focuce = 1;
    // mFlags = {};
    
    for (uint8_t i = 0; i < Screens; i++)
    { // Resetting all arrays
        for (uint8_t j = 0; j < Lines; j++)
        {
            screen[i][j] = '\0';
            indicator[i][j] = 0;
            line_types[i][j] = NO_VALUE;
            pointf1[i][j] = NULL;
            pointf2[i][j] = NULL;
            pointf3[i][j] = NULL;

            obj_indicator[i][j] = NULL;
            obj_pointf1[i][j] = NULL;
            obj_pointf2[i][j] = NULL;
            obj_pointf3[i][j] = NULL;
        }
    }
}

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, uint16_t* ind, MENU_LINE_TYPE line_type=NORMAL)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = ind;
    line_types[scr][line] = line_type;
}

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, uint8_t* ind, MENU_LINE_TYPE line_type=NORMAL)
{
    screen[scr][line] = item_name;
    indicator[scr][line] = (uint16_t)&ind;
    line_types[scr][line] = line_type;
}

void Menu::SetNames(uint8_t scr, uint8_t line, String item_name, MENU_LINE_TYPE line_type=NO_VALUE)
{
    screen[scr][line] = item_name;
    line_types[scr][line] = line_type;
}

void Menu::SetFunc1(uint8_t scr, uint8_t line, void *p)
{
    pointf1[scr][line] = p;
}

void Menu::SetFunc1(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object)
{
    if ((obj_indicator[scr][line] != NULL) && (obj_indicator[scr][line] != object))
    {
        return;
    }
    else
    {
        obj_indicator[scr][line] = object;
    }
    obj_pointf1[scr][line] = listener;
}


void Menu::SetFunc2(uint8_t scr, uint8_t line, void *p)
{
    pointf2[scr][line] = p;
}

void Menu::SetFunc2(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object)
{
    if ((obj_indicator[scr][line] != NULL) && (obj_indicator[scr][line] != object))
    {
        return;
    }
    else
    {
        obj_indicator[scr][line] = object;
    }
    obj_pointf2[scr][line] = listener;
}

void Menu::SetFunc3(uint8_t scr, uint8_t line, void *p)
{
    pointf3[scr][line] = p;
}

void Menu::SetFunc3(uint8_t scr, uint8_t line, void (*listener)(void *object), void *object)
{
    if ((obj_indicator[scr][line] != NULL) && (obj_indicator[scr][line] != object))
    {
        return;
    }

    obj_indicator[scr][line] = object;
    obj_pointf3[scr][line] = listener;
}

bool Menu::CheckFunction1()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if ((pointf1[tmp_scr][tmp_line]) != 0) return true;

    return false;
}

void Menu::RunFunction1()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if (obj_pointf1[tmp_scr][tmp_line] != NULL)
    {
        obj_pointf1[tmp_scr][tmp_line](obj_indicator[tmp_scr][tmp_line]);
        MakeMenuCurrentLine();
    }
    else if ((pointf1[tmp_scr][tmp_line]) != 0)
    {
        pointf1[tmp_scr][tmp_line]();
        MakeMenuCurrentLine();
    }
    mFlags.menuUpdate_f = false;
}


bool Menu::CheckFunction2()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if ((pointf2[tmp_scr][tmp_line]) != 0) return true;

    return false;
}

void Menu::RunFunction2()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if (obj_pointf2[tmp_scr][tmp_line] != NULL)
    {
        obj_pointf2[tmp_scr][tmp_line](obj_indicator[tmp_scr][tmp_line]);
        MakeMenuCurrentLine();
    }
    else if ((pointf2[tmp_scr][tmp_line]) != 0)
    {
        pointf2[tmp_scr][tmp_line]();
        MakeMenuCurrentLine();
    }
    mFlags.menuUpdate_f = false;
}

bool Menu::CheckFunction3()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if (obj_pointf3[tmp_scr][tmp_line] != NULL) return true;
    if ((pointf3[tmp_scr][tmp_line]) != 0) return true;

    return false;
}

bool Menu::RunFunction3()
{
    uint8_t tmp_scr = GetScreen();
    uint8_t tmp_line = GetLine();

    if (line_types[tmp_scr][tmp_line] == NAVIGATION)
    {
        MenuSwitchToScreen(tmp_line);
        return true;
    }

    if ((obj_pointf3[tmp_scr][tmp_line] == NULL) && ((pointf3[tmp_scr][tmp_line]) == 0))
    {
        return false;
    }
    else if (obj_pointf3[tmp_scr][tmp_line] != NULL)
    {
        obj_pointf3[tmp_scr][tmp_line](obj_indicator[tmp_scr][tmp_line]);
    }
    else if ((pointf3[tmp_scr][tmp_line]) != 0) // NULL
    {
        pointf3[tmp_scr][tmp_line]();
    }
    
    if (mFlags.menuActive_f == true){
        MakeMenuCurrentLine();
    }
    mFlags.menuUpdate_f = false;
    return true;
}

void Menu::SetLineType(uint8_t scr, uint8_t line, MENU_LINE_TYPE line_type)
{
    line_types[scr][line] = line_type;
}


uint8_t Menu::GetScreen()
{
    return (scr_num);
}

uint8_t Menu::GetLine()
{
    return (focuce);
}
//*************************************************************************************************************************

void Menu::MenuNextLine(){
    this->MakeMenu(NEXT_LINE,  UPDATE_SCREEN);
}

void Menu::MenuPrevLine(){
    MakeMenu(PREVIOS_LINE,  UPDATE_SCREEN);
}

void Menu::MenuNextScreen(){
    MakeMenu(UPDATE_LINE,  NEXT_SCREEN);
}

void Menu::MenuPrevScreen(){
    MakeMenu(UPDATE_LINE,  PREVIOS_SCREEN);
}

void Menu::MenuUpdate(){
    MakeMenu(UPDATE_LINE, UPDATE_SCREEN); 
}

void Menu::MenuSwitchToScreen(uint8_t scr)
{
    if (screen[scr_num][0] != '\0')
    {
        scr_num = scr;
        focuce = 1;
    }
    MenuUpdate();
}

//*************************************************************************************************************************

void Menu::MakeMenuLine(uint8_t scr_index, uint8_t line_index)
{
    if (scr_index == scr_num)
    {
        uint16_t *ind_p;
        ind_p = indicator[scr_index][line_index];
        uint8_t line_to_print = 0;
        for (line_to_print = 0; line_to_print < rows_to_update; line_to_print++)
        {
            if (printed_lines_indicator[line_to_print] == indicator[scr_index][line_index])
            {
                MenuClearLine(line_to_print+1);
                PrintOneLine(scr_index, line_index, line_to_print+1);
                SetMenuActive();
                return;
            }
        }
        
    }
}

void Menu::MakeMenuCurrentLine()
{
    MakeMenuLine(scr_num, focuce);
    DisplayCurrentCursor(active_focuce);
}

void Menu::SetMenuActive()
{
    mFlags.menuActive_f = true;
    if (mFlags.backlight_enabled == true)
    {
        //_lcd->setBacklight(HIGH);
        backlight_timer = millis();
    }
}

void Menu::SetMenuUpdate()
{
    mFlags.menuUpdate_f = true;
}

void Menu::MenuHandler()
{
    if (mFlags.menuActive_f == true)
    {
        if (mFlags.menuUpdate_f == true)
        {
            MenuUpdate();
            SetMenuActive();
            mFlags.menuUpdate_f = false;
        }

        if ((mFlags.backlight_enabled == true) && (millis() - backlight_timer > backlight_timeout))
        {
            //_lcd->setBacklight(LOW);
            //backlight_timer = 0;
        }
    }
}
