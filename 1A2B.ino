#include <LiquidCrystal_I2C.h>

enum states{
  IDLE,
  SET,
  GUESS,
  WRONG,
  CORRECT,
} state;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int OK_BTN_PIN = 1;

int ok, pre_ok;

int A = 0, B;

void setup() {
  pinMode(OK_BTN_PIN, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();

  state = IDLE;
  lcd_print("State:", "IDLE");
  delay(50);
}

void loop() {
  pre_ok = ok;
  ok = digitalRead(OK_BTN_PIN);

  switch(state){
    case IDLE:
    if(!ok && pre_ok){
      state = SET;
      lcd_print("State:", "SET");
    }
    break;

    case SET:
    if(!ok && pre_ok){
      state = GUESS;
      lcd_print("State:", "GUESS");
    }
    break;

    case GUESS:
    if(!ok && pre_ok){
      if(A == 4){ // if 4a0b
      lcd_print("State:", "CORRECT");
        state = CORRECT;
      }
      else{
        state = WRONG;
        lcd_print("State:", "WRONG");
      }
    }
    break;

    case WRONG:
    if(!ok && pre_ok){
      state = GUESS;
      lcd_print("State:", "GUESS");
    }
    break;

    case CORRECT:
    if(!ok && pre_ok){
      state = IDLE;
      lcd_print("State:", "IDLE");
    }
    break;
  }

  delay(20);
}

void lcd_print(char* str1, char* str2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(str1);
  lcd.setCursor(0, 1);
  lcd.print(str2);
}
