#define LED_SINGLE_PIN 7
#define LED_DOUBLE_PIN 6
#define LED_SHORT_PIN 5
#define LED_LONG_PIN 4
#define BUTTON_PIN 2

#define LIMIT_DOUBLE_PRESS 800
#define LIMIT_SHORT_HOLD 700
#define LIMIT_LONG_HOLD 1400
#define LIMIT_LED_ON 2000


unsigned long btn_timer_start;
unsigned long led_on_start;
bool led_on = false;
uint8_t active_led;
uint8_t state = 0;
uint8_t button_state = 0;


/* Set up button and LED pins */
void setup() {
  //Push Button Pin (connected to GND so must be INPUT_PULLUP)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //LED Pins
  pinMode(LED_SINGLE_PIN, OUTPUT);
  pinMode(LED_DOUBLE_PIN, OUTPUT);
  pinMode(LED_SHORT_PIN, OUTPUT);
  pinMode(LED_LONG_PIN, OUTPUT);

  //Make sure all LEDs are off to start with
  turn_off_led(LED_SINGLE_PIN);
  turn_off_led(LED_DOUBLE_PIN);
  turn_off_led(LED_SHORT_PIN);
  turn_off_led(LED_LONG_PIN);
}


/* Turn on a given LED */
void turn_on_led(uint8_t pin_num){
  digitalWrite(pin_num, HIGH);
  led_on = true;
}

/* Turn off a given LED */
void turn_off_led(uint8_t pin_num){
  digitalWrite(pin_num, LOW);
  led_on = false;
}


/* State machine containing all the logic for the challenge. Determines type of button input (Single Press, Double Press, Short Hold, Long Hold) and
turns LEDs on accordingly. */
void state_machine(){
/* States as follows:
  0: Waiting for input
  1: Short Hold / First Press Detection 
  2: Long Hold Detection
  3: Double Press Detection
  4: LED on for 2 secs, input is ignored during this time */

  //State 0 - Waiting for input
  if(state == 0){
    //At the first falling edge (button press), start our timer and move into the next state
    if(button_state == LOW){
      btn_timer_start = millis();
      state = 1;
    }


  //State 1 - Short hold / first press detection
  }else if(state == 1){
    //if the short hold time limit has been reached, go to long hold detection
    if(millis() - btn_timer_start > LIMIT_SHORT_HOLD){
      state = 2;
    //else if the button has been released quickly (just a press, not a hold), go to double press detection
    }else if(button_state == HIGH){
      state = 3;
    }


  //State 2 - Long hold detection
  }else if(state == 2){
    //if button has been released, determine if it was a long or short hold and move onto the next state
    if(button_state == HIGH){
      //if time limit was not reached, we have detected a short hold
      if(millis() - btn_timer_start < LIMIT_LONG_HOLD){
        active_led = LED_SHORT_PIN;
      //else the time limit was reached, so we have detected a long hold
      }else{
        active_led = LED_LONG_PIN;
      }
      //move to LED On state
      state = 4;
    }


  //State 3 - Double press detection
  }else if(state == 3){
    //if button has been pressed for a second time (the first was in state 1), we've detected a double press and can move onto the next state
    if(button_state == LOW){
      active_led = LED_DOUBLE_PIN;
      state = 4;
    //else if enough time passes, we conclude it was only a single press and move on to the next state
    }else if(millis() - btn_timer_start > LIMIT_DOUBLE_PRESS){
      active_led = LED_SINGLE_PIN;
      state = 4;
    }
  

  //State 4 - LED on for 2 seconds wait
  }else if(state == 4){
    //if button is still held, we will wait in this state doing nothing until it is released
    if(button_state == HIGH){
      //if LED isn't on yet, turn it on and take note of the time (start the 2 second timer)
      if(!led_on){
        turn_on_led(active_led);
        led_on_start = millis();
      //else wait for the 2 second timer to finish, then move back to the starting state
      }else{
        if(millis() - led_on_start > LIMIT_LED_ON){
          turn_off_led(active_led);
          state = 0;
        }
      }
    }
  }
}


void loop() {
  //only update the button value if the LED is not active
  if(!led_on){
    button_state = digitalRead(BUTTON_PIN);
  }

  state_machine();
}