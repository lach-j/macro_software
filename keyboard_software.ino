/*
* Software for ATmega32u4 based keypads for custom HID macros.
*
* <https://github.com/lach-j/macro_software>
*/

#define RGB_ENABLED

#include <Keypad.h>
#include <Keyboard.h>

const byte ROWS = 3;
const byte COLS = 5;
const byte LAYERS = 2;

int curr_lyr = 0;

#ifdef RGB_ENABLED
int rgb_pins[] = {3, 6, 5};
int layer_colors[LAYERS][3] = {{255, 0, 0}, {0, 255, 0}};
#endif

// Define Keymap.
char keys[ROWS][COLS] = {
    {'A','B','C','D','E'},
    {'F','G','H','I','J'},
    {'K','L','M','N','O'},
};

void EMPTY(int state) {
  // Placeholder function.
}

void MUTEM(int state){
  // Triggers F15 key, used to toggle Discord mute.
  if (state == PRESSED) { 
    Keyboard.write(KEY_F15);
  }
}

void DRIVE(int state) {
  // Opens file explorer and navigates to current semester.
  if (state == PRESSED) {
    switch (curr_lyr)
    {
      case 0:
        WIN_RUN("G:\\My Drive\\# Uni\\Year 2\\Semester 2");
        break;
      case 1:
        LIN_RUN("dolphin gdrive:/google1/");
        break;
    }
  }
}

void SPTFY(int state) {
  // Launch spotify
  if (state == PRESSED) {
    switch (curr_lyr)
    {
      case 0:
        WIN_RUN("spotify.exe");
        break;
      case 1:
        LIN_RUN("spotify");
        break;
    }
  }
}

void SNIPN (int state) {
  // Opens rectangular selection snipping tool.
  if (state == PRESSED) {
    switch (curr_lyr)
    {
      case 0:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('s');
        Keyboard.releaseAll();
        break;
      case 1:
        Keyboard.press(KEY_LEFT_GUI);
        Keyboard.press('x');
        Keyboard.releaseAll();
        break;
    }
  }
}

void N_LYR(int state) {
  // Change to next layer, if RGB_ENABLED is defined,
  // each layer has a defined colour code.
  if (state == PRESSED) {
    if (curr_lyr >= LAYERS-1) {
      curr_lyr = 0;
    } else {
      curr_lyr++;
    }
    #ifdef RGB_ENABLED
    setRGB(layer_colors[curr_lyr]);
    #endif
  }
}

void TERMI(int state) {
  // Open DOS/Bash terminal.
  if (state == PRESSED) {
    switch (curr_lyr)
    {
    case 0:
      WIN_RUN("cmd");
      break;
    case 1:
      BASH();
    default:
      break;
    }
  }
}

void BBORD(int state) {
  // Open UON blackboard with default browser.
  if (state == PRESSED) {
    switch (curr_lyr)
    {
    case 0:
      WIN_RUN("https://uonline.newcastle.edu.au/");
      break;
    case 1:
      LIN_RUN("xdg-open https://uonline.newcastle.edu.au/");
      break;
    default:
      break;
    }
  }
}


void VSCOD(int state) {
  // Launch Visual Studio Code.
  if (state == PRESSED) {
    switch (curr_lyr)
    {
    case 0:
      WIN_RUN("code");
      break;
    case 1:
      LIN_RUN("code");
      break;
    default:
      break;
    }
  }
}

void (*Macros[LAYERS][ROWS*COLS])(int state) = {
  { // WINDOWS
    DRIVE, SPTFY, VSCOD, EMPTY, EMPTY,
    TERMI, BBORD, EMPTY, EMPTY, EMPTY,
    SNIPN, EMPTY, MUTEM, EMPTY, N_LYR
  },
  { // LINUX
    DRIVE, SPTFY, VSCOD, EMPTY, EMPTY,
    TERMI, BBORD, EMPTY, EMPTY, EMPTY,
    SNIPN, EMPTY, MUTEM, EMPTY, N_LYR
  }
};


byte rowPins[ROWS] = {7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 16, 14, 15, A0}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // Create Keypad object

void setup() {
  Keyboard.begin();
  keypad.addEventListener(keypadEvent);
  keypad.setHoldTime(1000); // Hold event time in ms
  #ifdef RGB_ENABLED
  setRGB(layer_colors[curr_lyr]);
  #endif
}

void loop(){
  char key = keypad.getKey();
}

void keypadEvent(KeypadEvent key){
  // On keypress, call Macro function.
  Macros[curr_lyr][key-65](keypad.getState()); 
}

#ifdef RGB_ENABLED
void setRGB(int rgb[3]) {
  for (int i=0; i<3; i++) {
    analogWrite(rgb_pins[i], rgb[i]);
  }
}
#endif

void BASH() {
  // Open bash terminal.
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
}

void LIN_RUN(String command) {
  // Start process on linux.
  BASH();
  delay(500);
  Keyboard.print(command);
  Keyboard.println(" & exit");
}

void WIN_RUN(String command) {
  // Run command on windows (WIN + R)
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(200);
  Keyboard.println(command);
}