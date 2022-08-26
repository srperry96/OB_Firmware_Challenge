#include "LEDStateMachine.h"

//LEDStateMachine object handles all the button and LED logic
LEDStateMachine LSM;


//Debouncing variables
int btn_state = HIGH;
int prev_btn_state = HIGH; //start high as button press is LOW
unsigned long last_debounce_time = 0;
unsigned long debounce_period = 25; //25ms debounce time


/* Set up LED and button pins */
void setup() {
  LSM.setup_pins();
}


/* Read button (debounce the input), and pass it to the LEDStateMachine. If an LED is illuminated, we ignore any input */
void loop() {  
  //Only update the button value if the LED is not on
  if(!LSM.led_on){
    int new_btn_reading = digitalRead(BUTTON_PIN);

    //Debouncing
    //if new reading is different to the current button state, reset the debounce timer
    if(new_btn_reading != prev_btn_state){
      last_debounce_time = millis();
    }

    //if debounce_period time has passed, we can assume it is a legitimate press and update btn_state
    if(millis() - last_debounce_time > debounce_period){
      if(new_btn_reading != btn_state){
        btn_state = new_btn_reading;
      }
    }
    //store the button state for debouncing next time round
    prev_btn_state = new_btn_reading;
  }

  //pass button state to the state machine, which will handle the LED / button press logic
  LSM.state_machine(btn_state);
}