//#include <HID-Project.h>
//#include <HID-Settings.h>
#include <Keypad.h>
#include <Keyboard.h>

const byte ROWS = 3;
const byte COLS = 5;
const byte LAYERS = 2;

int curr_lyr = 0;
int rgb_pins[] = {3, 6, 5};

Event *queue[10];

int layer_colors[LAYERS][3] = {{255, 0, 0}, {255, 0, 255}};

char keys[ROWS][COLS] = {
    {'A','B','C','D','E'},
    {'F','G','H','I','J'},
    {'K','L','M','N','O'},
};

void EMPTY(int state) {
  Serial.println("empty");
}

void MUTEM(int state){
  if (state == PRESSED) {
    Keyboard.write(KEY_F15);
  }
}


void DRV_L(int state) {
  if (state == PRESSED) {
    Serial.println("DRIVE");
    LIN_RUN("dolphin gdrive:/google1/");
  }
}

void DRV_W(int state) {
  
}

void LSPOT(int state) {
  if (state == PRESSED) {
    LIN_RUN("spotify");
  }
}

void WSPOT(int state) {
  if (state == PRESSED) {
    WIN_RUN("spotify.exe");
  }
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

void TERMI(int state) {
  if (state == PRESSED) {
    BASH();
  }
}

void (*Macros[LAYERS][ROWS*COLS])(int state) = {
  { // WINDOWS
    DRV_W, WSPOT, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, MUTEM, EMPTY, N_LYR
  },
  { // LINUX
    DRV_L, LSPOT, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, MUTEM, EMPTY, N_LYR
  }
};

byte rowPins[ROWS] = {7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 16, 14, 15, A0}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  int color[] = {255, 255, 255};
  setRGB(color);
  Serial.begin(9600);
//  Consumer.begin();
  Keyboard.begin();
  keypad.addEventListener(keypadEvent);
  keypad.setHoldTime(1000);
  setRGB(layer_colors[curr_lyr]);
}

void loop(){
  char key = keypad.getKey();
  for (int i = 0; i<10; i++) {
    if (queue[i]->time < millis()) {
      queue[i]->functocall(1);
    }
  }
}

void keypadEvent(KeypadEvent key){
  queue[0]->functocall = Macros[curr_lyr][key-65](keypad.getState());
  queue[0]->time = millis() + 1000;
}

void setRGB(int rgb[3]) {
  for (int i=0; i<3; i++) {
    analogWrite(rgb_pins[i], rgb[i]);
  }
}
 
void BASH() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
}

void LIN_RUN(String command) {
  BASH();
  delay(500);
  Keyboard.print(command);
  Keyboard.println(" & exit");
}

void WIN_RUN(String command) {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.println(command);
}


class Event {
  public:
    int time;
    void (*functocall)(int);
};