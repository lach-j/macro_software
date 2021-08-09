#include <HID-Project.h>
#include <HID-Settings.h>
#include <Keypad.h>

const byte ROWS = 3;
const byte COLS = 5;
const byte LAYERS = 2;
char keys[ROWS][COLS] = {
    {'a','b','c','d','e'},
    {'f','g','h','i','j'},
    {'k','l','m','n','o'},
};

void EMPTY(int state) {
}



void (*Macros[LAYERS][ROWS*COLS])(int state) = {
  {
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
  },
  {
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
  }
};

byte rowPins[ROWS] = {7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 16, 14, 15, A0}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  Consumer.begin();
  keypad.addEventListener(keypadEvent);
  keypad.setHoldTime(1000);
}

void loop(){
  char key = keypad.getKey();
}

void keypadEvent(KeypadEvent key){
  Serial.println(key);
}
