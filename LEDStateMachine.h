#ifndef __LEDSTATEMACHINE_H__
#define __LEDSTATEMACHINE_H__

#include "Arduino.h"

//LED and Button pins 
#define LED_SINGLE_PIN 7
#define LED_DOUBLE_PIN 6
#define LED_SHORT_PIN 5
#define LED_LONG_PIN 4
#define BUTTON_PIN 2

//Various time limits in ms
#define LIMIT_DOUBLE_PRESS 800
#define LIMIT_SHORT_HOLD 700
#define LIMIT_LONG_HOLD 1400
#define LIMIT_LED_ON 2000


class LEDStateMachine{
  public:
    LEDStateMachine();
    void setup_pins();
    void state_machine(int button_state);

    bool led_on = false;

  private:
    void turn_on_led(uint8_t pin_num);
    void turn_off_led(uint8_t pin_num);

    unsigned long btn_timer_start;
    unsigned long led_on_start;
    uint8_t state = 0;
    uint8_t active_led;
};

#endif