#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<Servo.h>

const int ledPin = 13;
const int PASSWORD_LENGTH = 4;
const int PASSWORD[PASSWORD_LENGTH]={1,2,3,4};
const int SERVO_PIN = 11;
const int BUZZER_PIN = 12;
LiquidCrystal_I2C lcd(0x27,16,2);
Servo servo;

//keypad Pins
const int ROW_PINS[4] = {2,3,4,5};
const int COL_PINS[3] = {6,7,8};

char KEYS[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
};

int passwordInput[PASSWORD_LENGTH];
int currentDigit = 0;
const char NO_KEYS = '\0';

void setup(){
  lcd.init();
  lcd.backlight();

  lcd.print("Enter Password: ");
  servo.attach(SERVO_PIN);
  for(int row = 0; row < 4; row++){
    pinMode(ROW_PINS[row],OUTPUT);
    digitalWrite(ROW_PINS[row],HIGH);
  }
  for(int col = 0; col<3; col++){
    pinMode(COL_PINS[col],INPUT_PULLUP);
  }
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(ledPin,OUTPUT);
}

void loop(){
  char key = getKey();
  if(key != NO_KEYS){
    if(key == '#'){
      if(checkPassword()){
        lcd.clear();
        lcd.print("Password OK!");
        unlockDoor();
        digitalWrite(ledPin,HIGH);
        delay(2000);
        lcd.clear();
        lcd.print("Locked Again");
        servo.write(90);
        digitalWrite(ledPin,LOW);
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password: ");
      }
      else{
        lcd.clear();
        lcd.print("Incorrect!");
        buzz(3);
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password:");
      }
      resetPassword();
    }else if(key == '*'){
      resetPassword();
      lcd.clear();
      lcd.print("Enter Password:");
    }else if(isdigit(key)){
      addDigit(key - '0');
    }
  }
}


// -------------Function starts-----------------------

char getKey(){
  char key = NO_KEYS;

  for(int row = 0; row<4; row++){
    digitalWrite(ROW_PINS[row],LOW);
    for(int col = 0; col<3; col++){
      if(digitalRead(COL_PINS[col]) == LOW){
        key = KEYS[row][col];
        while(digitalRead(COL_PINS[col])== LOW){
          delay(10);
        }
      }
    }
    digitalWrite(ROW_PINS[row],HIGH);
    }
    return key;
}

void addDigit(int digit){
  if(currentDigit < PASSWORD_LENGTH){
    passwordInput[currentDigit]= digit;
    currentDigit++;
    lcd.setCursor(currentDigit - 1,1);
    lcd.print(digit);
    buzz(1); // Optional: Provide a buzz sound when a button is pressed
    delay(200); // Delay for button debounce
  }
}

bool checkPassword(){
  for(int i=0; i< PASSWORD_LENGTH; i++){
    if(passwordInput[i] != PASSWORD[i]){
      return false;
    }
  }
  return true;
}

void resetPassword(){
  for(int i=0; i< PASSWORD_LENGTH; i++){
    passwordInput[i]=0;
  }
  currentDigit = 0;
  lcd.setCursor(0,1);
  lcd.print("     "); // Clear the password display
}

void unlockDoor(){
  servo.write(0); // Close the door
}

void buzz(int count){
  for(int i=0; i<count; i++){
    digitalWrite(BUZZER_PIN,HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN,LOW);
    delay(100);
  }
}


















