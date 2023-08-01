enum LED_CHANGE_MODE
{
    DECR = 0,
    INCR = 1
};

class LED
{
private:
    byte pin;
    int _maxPerc = 100;
    int _minPerc = 0;
    // int UINT8_MIN = 0;
    // int UINT8_MAX =255;
public:
    int value = 0;     // uint8_t
    int percValue = 0; // uint16_t
    uint16_t state = 0;
    uint8_t menuNumer = 1;
    uint8_t lineNumber = 0;

    LED(byte new_pin)
    {
        pin = new_pin;
        pinMode(pin, OUTPUT);
        value = 0;
        percValue = map(value, 0, 255, _minPerc, _maxPerc);
        analogWrite(pin, value);
    }

    LED(byte new_pin, uint8_t line)
    {
        pin = new_pin;
        pinMode(pin, OUTPUT);
        lineNumber = line;
        value = 0;
        percValue = map(value, 0, 255, _minPerc, _maxPerc);
        analogWrite(pin, value);
    }

    void brighLedChange(int change_value, int mode)
    {
        analogWrite(pin, value);
        // ebugD(F("Led %d last value = %d changed by value = %d, old percValue = %d"), pin, value, change_value, percValue);

        if (mode == DECR)
        {
            value -= change_value;
        }
        else if (mode == INCR)
        {
            value += change_value;
        }
        value = constrain(value, 0, 255);
        percValue = map(value, 0, 255, 0, 100);
        analogWrite(pin, value);
        // sFlags.json_transmit = true;

       // if (menu_main.GetScreen() == LEDS_SCREEN)
       // {

            // mFlags.menuUpdate = true;
            menu_main.MakeMenuLine(menuNumer, lineNumber);
        //}
    }

    void brigh_up_ten()
    {
        brighLedChange(15, INCR);
    }

    void brigh_down_ten()
    {
        brighLedChange(15, DECR);
    }

    void brigh_up(int change_value)
    {
        brighLedChange(change_value, INCR);
    }

    void brigh_down(int change_value)
    {
        brighLedChange(change_value, DECR);
    }

    void writeValue(int val)
    {
        analogWrite(pin, val);
    }

    void setPercValAndWrite(int val)
    {
        percValue = val;
        if (val == 0)
        {
            state = false;
        }
        else
        {
            state = true;
        }

        value = map(percValue, _minPerc, _maxPerc, 0, 255);
        analogWrite(pin, value);
    }

    static void stat_led_up(void *context)
    {
        reinterpret_cast<LED *>(context)->brigh_up_ten();
    }

    static void stat_led_down(void *context)
    {
        reinterpret_cast<LED *>(context)->brigh_down_ten();
    }
};

uint16_t LED::*led_ptr = &LED::percValue;

LED led1(PIN_LED1, 1);
LED led2(PIN_LED2, 2);
LED led3(PIN_LED3, 3);
LED led4(PIN_LED4, 4);
LED led5(PIN_LED5, 5);
LED led6(PIN_LED6, 6);