#include <LiquidCrystal_I2C.h>
#include "SevSeg.h"

enum states{
  IDLE,
  SET,
  GUESS,
  WRONG,
  CORRECT,
} state;

LiquidCrystal_I2C lcd(0x27, 16, 2);
SevSeg sevseg;

const int OK_BTN_PIN = 0;
const int RIGHT_BTN_PIN = 1;
const int UP_BTN_PIN = 2;

const int LED0_PIN = A0;
const int LED1_PIN = A1;
const int LED2_PIN = A2;
const int LED3_PIN = A3;

int ok, pre_ok;
int right, pre_right;
int up, pre_up;

int LED_state[4];

int A, B;
int answer[4], guess[4];

unsigned long prevMillis = 0;
const long interval = 50;

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {3, 4, 5, 6};
  byte segmentPins[] = {7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);

  pinMode(OK_BTN_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BTN_PIN, INPUT_PULLUP);
  pinMode(UP_BTN_PIN, INPUT_PULLUP);

  pinMode(LED0_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  // init
  state = IDLE;
  lcd_print("State:", "IDLE");
  led_update(0, 0, 0, 0);
  sevseg.setChars("----");

  //delay(50);
}

void loop() {
  sevseg.refreshDisplay();

  unsigned long curMillis = millis();
  if(curMillis - prevMillis < interval){
    return;
  }
  prevMillis = curMillis;

  read_pin();

  switch(state){
    case IDLE:
    if(!ok && pre_ok){
      state = SET;
      lcd_print("State:", "SET");
      led_update(1, 0, 0, 0);
      sevseg.setNumber(answer[0]*1000 + answer[1]*100 + answer[2]*10 + answer[3]);
    }
    break;

    case SET:
    if(!ok && pre_ok){
      state = GUESS;
      lcd_print("State:", "GUESS");
      led_update(1, 0, 0, 0);
      sevseg.setNumber(guess[0]*1000 + guess[1]*100 + guess[2]*10 + guess[3]);
    }
    else if(!right && pre_right){
      led_update(LED_state[3], LED_state[0], LED_state[1], LED_state[2]);
    }
    else if(!up && pre_up){
      answer_update();
      sevseg.setNumber(answer[0]*1000 + answer[1]*100 + answer[2]*10 + answer[3]);
    }
    break;

    case GUESS:
    if(!ok && pre_ok){
      verify_correctness();
      if(A == 4){ // if 4a0b
        state = CORRECT;
        lcd_print("State:", "CORRECT");
        led_update(1, 1, 1, 1);
      }
      else{
        state = WRONG;
        lcd_print("State:", "WRONG");
        led_update(0, 0, 0, 0);
      }
      char str[5] = {A+'0', 'A', B+'0', 'B', '\0'};
      sevseg.setChars(str);
    }
    else if(!right && pre_right){
      led_update(LED_state[3], LED_state[0], LED_state[1], LED_state[2]);
    }
    else if(!up && pre_up){
      guess_update();
      sevseg.setNumber(guess[0]*1000 + guess[1]*100 + guess[2]*10 + guess[3]);
    }
    break;

    case WRONG:
    if(!ok && pre_ok){
      state = GUESS;
      lcd_print("State:", "GUESS");
      led_update(1, 0, 0, 0);
      sevseg.setNumber(guess[0]*1000 + guess[1]*100 + guess[2]*10 + guess[3]);
    }
    break;

    case CORRECT:
    if(!ok && pre_ok){
      state = IDLE;
      lcd_print("State:", "IDLE");
      led_update(0, 0, 0, 0);
      sevseg.setChars("----");
      answer[0] = answer[1] = answer[2] = answer[3] = 0;
      guess[0] = guess[1] = guess[2] = guess[3] = 0;
    }
    break;
  }

  //delay(20);
}

void guess_update(){
  guess[0] = (guess[0] + LED_state[0]) % 10;
  guess[1] = (guess[1] + LED_state[1]) % 10;
  guess[2] = (guess[2] + LED_state[2]) % 10;
  guess[3] = (guess[3] + LED_state[3]) % 10;
}

void answer_update(){
  answer[0] = (answer[0] + LED_state[0]) % 10;
  answer[1] = (answer[1] + LED_state[1]) % 10;
  answer[2] = (answer[2] + LED_state[2]) % 10;
  answer[3] = (answer[3] + LED_state[3]) % 10;
}

void verify_correctness(){
  A = (answer[0] == guess[0]) + (answer[1] == guess[1]) + (answer[2] == guess[2]) + (answer[3] == guess[3]);
  B = (answer[0] == guess[1]) + (answer[0] == guess[2]) + (answer[0] == guess[3]) + 
      (answer[1] == guess[0]) + (answer[1] == guess[2]) + (answer[1] == guess[3]) +
      (answer[2] == guess[0]) + (answer[2] == guess[1]) + (answer[2] == guess[3]) +
      (answer[3] == guess[0]) + (answer[3] == guess[1]) + (answer[3] == guess[2]);
}

void read_pin(){
  pre_ok = ok;
  ok = digitalRead(OK_BTN_PIN);
  pre_right = right;
  right = digitalRead(RIGHT_BTN_PIN);
  pre_up = up;
  up = digitalRead(UP_BTN_PIN);
}

void led_update(int a, int b, int c, int d){
  LED_state[0] = a;
  LED_state[1] = b;
  LED_state[2] = c;
  LED_state[3] = d;
  digitalWrite(LED0_PIN, LED_state[0]);
  digitalWrite(LED1_PIN, LED_state[1]);
  digitalWrite(LED2_PIN, LED_state[2]);
  digitalWrite(LED3_PIN, LED_state[3]);
}

void lcd_print(char* str1, char* str2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(str1);
  lcd.setCursor(0, 1);
  lcd.print(str2);
}
