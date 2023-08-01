
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
  if (menu_main.mFlags.lineSelected == 0)
  {
    if (enc1.isRight()) //|| enc1.isFastR())
    {
      // menu_main.MakeMenu(NEXT_LINE, UPDATE_SCREEN);
      menu_main.MakeMenu(UPDATE_LINE, NEXT_SCREEN);
    };
    if (enc1.isLeft()) //|| enc1.isFastL())
    {
      menu_main.MakeMenu(NEXT_LINE, UPDATE_SCREEN);
    };
    if (enc1.isClick())
    {
      menu_main.mFlags.lineSelected = 1;
      //menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
     // menu_main.mFlags.menuActive = true;
       mFlags.menuUpdate = true;

    };

    if (enc1.isRightH())
    {
      menu_main.MakeMenu(UPDATE_LINE, NEXT_SCREEN); // next screen menu (RIGHT pressed turn)
    };
    if (enc1.isLeftH())
    {
      menu_main.MakeMenu(UPDATE_LINE, PREVIOS_SCREEN); // prev screen menu (LEFT pressed turn)
    };
  }

  else if (menu_main.mFlags.lineSelected == 1)
  {
    if (enc1.isRight())
    {
      // menu_main.RunFunction1(); // menu func1 in action
      menu_main.RunObjFunc1();
    };
    if (enc1.isLeft())
    {
      menu_main.RunObjFunc2(); ////menu func2 in action
    };
    if (enc1.isClick())
    {
      menu_main.mFlags.lineSelected = false;
      menu_main.mFlags.menuUpdate =true;
      //menu_main.MakeMenu(UPDATE_LINE, UPDATE_SCREEN);
    };
  }
  if (enc1.isTurn() or enc1.isPress() or enc1.isRelease() or enc1.isClick())
  {
    menu_main.SetMenuActive();
  }
}