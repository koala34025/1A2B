#include <LiquidCrystal_I2C.h>

enum states{
  IDLE,
  SET,
  GUESS,
  WRONG,
  CORRECT,
} state;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int OK_BTN_PIN = 0;
const int RIGHT_BTN_PIN = 1;

const int LED0_PIN = A0;
const int LED1_PIN = A1;
const int LED2_PIN = A2;
const int LED3_PIN = A3;

int ok, pre_ok;
int right, pre_right;

int LED_state[4];

int A = 0, B;

void setup() {
  pinMode(OK_BTN_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BTN_PIN, INPUT_PULLUP);

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

  delay(50);
}

void loop() {
  read_pin();
  
  switch(state){
    case IDLE:
    if(!ok && pre_ok){
      state = SET;
      lcd_print("State:", "SET");
      led_update(1, 0, 0, 0);
    }
    break;

    case SET:
    if(!ok && pre_ok){
      state = GUESS;
      lcd_print("State:", "GUESS");
      led_update(1, 0, 0, 0);
    }
    else if(!right && pre_right){
      led_update(LED_state[3], LED_state[0], LED_state[1], LED_state[2]);
    }
    break;

    case GUESS:
    if(!ok && pre_ok){
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
    }
    else if(!right && pre_right){
      led_update(LED_state[3], LED_state[0], LED_state[1], LED_state[2]);
    }
    break;

    case WRONG:
    if(!ok && pre_ok){
      state = GUESS;
      lcd_print("State:", "GUESS");
      led_update(1, 0, 0, 0);
    }
    break;

    case CORRECT:
    if(!ok && pre_ok){
      state = IDLE;
      lcd_print("State:", "IDLE");
      led_update(0, 0, 0, 0);
    }
    break;
  }

  delay(20);
}

void read_pin(){
  pre_ok = ok;
  ok = digitalRead(OK_BTN_PIN);
  pre_right = right;
  right = digitalRead(RIGHT_BTN_PIN);
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
