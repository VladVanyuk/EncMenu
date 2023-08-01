
class BULB
{
private:
  byte pin;
  bool _toggleState = false;
  bool _state = false;
  // uint32_t signal_duration = IMP_RELAY_SIGNAL
public:
  uint16_t state = 0;
  uint8_t menuNumer = BULBS_SCREEN;
  uint8_t lineNumber = 0;
  
  BULB(byte new_pin)
  {
    pin = new_pin;
    state = 0;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  BULB(byte new_pin, uint8_t line)
  {
    pin = new_pin;
    state = 0;
    lineNumber = line;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  void setOn()
  {
    if (!state)
    {
      toggleBulb();
    }
  }

  void setOff()
  {
    if (state)
    {
      toggleBulb();
    }
  }

  void toggleBulb()
  {
    _toggleState = true;
    if (_toggleState == true)
    {
      digitalWrite(pin, HIGH);
      delayMicroseconds(50);
      digitalWrite(pin, LOW);

      if (state == 0)
      {
        state = 1;
      }
      else
      {
        state = 0;
      }
      _toggleState = false;
      if (menu_main.GetScreen == BULBS_SCREEN)
      {
       
        menu_main.MakeMenuLine(menuNumer, lineNumber);

      }
    }
  }

  static void stat_bulb(void *context){
    reinterpret_cast<BULB *>(context)->toggleBulb();
  }

};

uint16_t BULB::*bulb_ptr = &BULB::state;

BULB bulb1(PIN_BULB1, 1);
BULB bulb2(PIN_BULB2, 2);
BULB bulb3(PIN_BULB3, 3);
BULB bulb4(PIN_BULB4, 4);