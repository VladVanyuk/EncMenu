#include "Arduino.h"
#include "EncMenu.h"

static void Menu::encIsr()
{
    this->enc->tickISR();
    this->mFlags.menuActive_f = true;
    MenuDebugPrintln("isr");
}

Menu::Menu()
{
    MenuDebugPrintln("obj new");
   
        _lcd = new LiquidCrystal_I2C(LCD_I2C_ADDRESS, LCD_COL_COUNT, LCD_ROW_COUNT);
        _lcd->clear();
        _lcd->init();
        _lcd->backlight();
        MenuDebugPrintln("lcd new");

        #if (MENU_ENC_ISR == 1)
        attachInterrupt(0, encIsr, CHANGE);  
        attachInterrupt(1, encIsr, CHANGE);
        
        enc = new EncButtonMenu(CLK, DT, SW, INPUT_PULLUP);
        #else
        enc = new EncButton(CLK, DT, SW, INPUT_PULLUP);
        #endif
        MenuDebugPrintln("enc new");
}

// ex: menu_main = new Menu(&lcd, &encB);
Menu::Menu(LiquidCrystal_I2C *lcd, EncButton *encB)
{   
   if (lcd != NULL)
    {
        _lcd = lcd;
        _lcd->clear();
        _lcd->init();
        _lcd->backlight();
    }
    
    ResetMenu();

    if (encB != NULL)
    {
        enc = encB;
    }
}

void Menu::ResetMenu()
{
    focuce = 1;
    // mFlags = {};
    
    for (uint8_t i = 0; i < Screens; i++)
    { // Resetting all arrays
        for (uint8_t j = 0; j < Lines; j++)
        {
            lines[i][j].line_text = '\0';
            lines[i][j].line_value = 0; //NULL
            lines[i][j].line_type = NO_VALUE;
            lines[i][j].val_type = NO_TYPE; 
            
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

void  Menu::SetLineValues(uint8_t scr, uint8_t line, String item_name, void* ind_val, line_value_t value_type = U16_t, line_t line_type = NORMAL)
{
    lines[scr][line].line_text = item_name;
    lines[scr][line].line_value =(value_t*)ind_val;
    lines[scr][line].val_type = value_type;
    lines[scr][line].line_type = line_type;
}


void Menu::SetLineValues(uint8_t scr, uint8_t line, String item_name, line_t line_type=NO_VALUE)
{
    lines[scr][line].line_text = item_name;
    lines[scr][line].line_value = 0; //NULL
    lines[scr][line].val_type = NO_TYPE;
    lines[scr][line].line_type = line_type;
}

void Menu::SetFunc1(uint8_t scr, uint8_t line, void *func_p)
{
   //if (func_p != NULL)
  // {
        pointf1[scr][line] = func_p;
   //}
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


void Menu::SetFunc2(uint8_t scr, uint8_t line, void *func_p)
{
   // if (func_p != NULL)
    //{
        pointf2[scr][line] = func_p;
   // }
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

void Menu::SetFunc3(uint8_t scr, uint8_t line, void *func_p)
{
    if (func_p != NULL)
    {
        pointf3[scr][line] = func_p;
    }
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

    if (lines[tmp_scr][tmp_line].line_type == NAVIGATION)
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

// void Menu::SetLineType(MenuLine_t *line, line_t line_type)
// {
    // lines -> line_type = line_type;
// }


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
    this->EncMenu(NEXT_LINE,  UPDATE_SCREEN);
}

void Menu::MenuPrevLine(){
    EncMenu(PREVIOS_LINE,  UPDATE_SCREEN);
}

void Menu::MenuNextScreen(){
    EncMenu(UPDATE_LINE,  NEXT_SCREEN);
}

void Menu::MenuPrevScreen(){
    EncMenu(UPDATE_LINE,  PREVIOS_SCREEN);
}

void Menu::MenuUpdate(){
    EncMenu(UPDATE_LINE, UPDATE_SCREEN); 
}

void Menu::MenuSwitchToScreen(uint8_t scr)
{
    if (lines[scr_num][0].line_text != '\0')
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
        // uint16_t *ind_p;
        // ind_p = (uint16_t)(lines[scr_index][line_index]->line_value);
        uint8_t line_to_print = 0;
        for (line_to_print = 0; line_to_print < rows_to_update; line_to_print++)
        {
            if (printed_lines_indicator[line_to_print] == (uint16_t)(lines[scr_index][line_index].line_value))
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

void Menu::MenuEncHandler()
{
    switch (enc->action()) {
        case EB_PRESS:
            MenuDebugPrintln("press");
            break;
        case EB_HOLD:
            MenuDebugPrintln("hold");
            break;
        case EB_STEP:
            MenuDebugPrintln("step");
            break;
        case EB_RELEASE:
            MenuDebugPrint("release. steps: ");
            MenuDebugPrint(enc->getSteps());
            MenuDebugPrint(", press for: ");
            MenuDebugPrint(enc->pressFor());
            MenuDebugPrint(", hold for: ");
            MenuDebugPrint(enc->holdFor());
            MenuDebugPrint(", step for: ");
            MenuDebugPrintln(enc->stepFor());
            break;
        case EB_CLICK:
            MenuDebugPrintln("click");
            break;
        case EB_CLICKS:
            MenuDebugPrint("clicks ");
            MenuDebugPrintln(enc->getClicks());
            break;
        case EB_TURN:
            MenuDebugPrint("turn ");
            MenuDebugPrint(enc->dir());
            MenuDebugPrint(" ");
            MenuDebugPrint(enc->fast());
            MenuDebugPrint(" ");
            MenuDebugPrintln(enc->pressing());
            break;
        case EB_REL_HOLD:
            MenuDebugPrintln("release hold");
            break;
        case EB_REL_HOLD_C:
            MenuDebugPrint("release hold clicks ");
            MenuDebugPrintln(enc->getClicks());
            break;
        case EB_REL_STEP:
            MenuDebugPrintln("release step");
            break;
        case EB_REL_STEP_C:
            MenuDebugPrint("release step clicks ");
            MenuDebugPrintln(enc->getClicks());
            break;
        //default:
           // MenuDebugPrintln();
    }
}

void Menu::MenuHandler()
{
    #if (MENU_ENC_ISR == 0)
        this->enc->tick();
    #endif    
    
    MenuEncHandler();

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
