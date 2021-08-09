#include <HID-Project.h>
#include <HID-Settings.h>
#include <Keypad.h>

const byte ROWS = 3;
const byte COLS = 5;
const byte LAYERS = 2;

int curr_lyr = 0;
int rgb_pins[] = {3, 6, 5};

int layer_colors[LAYERS][3] = {{255, 0, 0}, {0, 255, 0}};

char keys[ROWS][COLS] = {
    {'A','B','C','D','E'},
    {'F','G','H','I','J'},
    {'K','L','M','N','O'},
};

void EMPTY(int state) {
  Serial.println("empty");
}

void N_LYR(int state) {
  if (state == PRESSED) {
    if (curr_lyr >= LAYERS-1) {
      curr_lyr = 0;
    } else {
      curr_lyr++;
    }
    setRGB(layer_colors[curr_lyr]);
    Serial.println(curr_lyr);
  }
}


void (*Macros[LAYERS][ROWS*COLS])(int state) = {
  { // WINDOWS
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, N_LYR
  },
  { // LINUX
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, N_LYR
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
  Macros[0][key-65](keypad.getState());
}

void setRGB(int rgb[3]) {
  for (int i=0; i<3; i++) {
    analogWrite(rgb_pins[i], rgb[i]);
  }
}
