
#define CLK 2
#define DT 3
#define SW 4
Encoder enc1(CLK, DT, SW);

void isr()
{
  enc1.tick();
}

void encoderSetup()
{
  enc1.setType(TYPE2); 
  attachInterrupt(0, isr, CHANGE); // Interrupts on encoder pins
  attachInterrupt(1, isr, CHANGE);
}


void encoderHandler()
{
  enc1.tick();
  if (menu_active->mFlags.lineSelected == 0)
  {
    if (enc1.isRight()) //|| enc1.isFastR())
    {
      menu_active->MakeMenu(NEXT_LINE, UPDATE_SCREEN);
    };
    if (enc1.isLeft()) //|| enc1.isFastL())
    {
      menu_active->MakeMenu(PREVIOS_LINE, UPDATE_SCREEN);
    };
    if (enc1.isClick())
    {
      if (menu_active->CheckFunction3())
      {
        menu_active->RunFunction3(); // menu func3 in action
      }
      else
      {
         menu_active->mFlags.lineSelected = 1;
         menu_active->MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
        menu_active->mFlags.menuActive = true;
      
      }
    };

    if (enc1.isRightH())
    {
      menu_active->MakeMenu(UPDATE_LINE, NEXT_SCREEN); // next screen menu (RIGHT pressed turn)
    };
    if (enc1.isLeftH())
    {
      menu_active->MakeMenu(UPDATE_LINE, PREVIOS_SCREEN); // prev screen menu (LEFT pressed turn)
    };
    
  }
  else if (menu_active->mFlags.lineSelected == 1)
  {
    if (enc1.isRight())
    {
      menu_active->RunFunction1(); // menu func1 in action
    };
    if (enc1.isLeft())
    {
      menu_active->RunFunction2(); ////menu func2 in action
    };
    if (enc1.isClick())
    {
      menu_active->mFlags.lineSelected = 0;
      menu_active->MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    };
  }
  if (enc1.isTurn() or enc1.isPress() or enc1.isRelease() or enc1.isClick())
  {
    menu_active->SetUpdateActive();
  }
}