#include "LEDStateMachine.h"

LEDStateMachine LSM;

int new_button_state = 0;

void setup() {
  //Set up LED and button pins
  LSM.setup_pins();
}


void loop() {
  //only update the button value if the LED is not active
  if(!LSM.led_on){
    new_button_state = digitalRead(BUTTON_PIN);
  }

  LSM.state_machine(new_button_state);
}