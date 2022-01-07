/**
 * The code here is a mixture of my style and the logic from baldengineer. I strongly suggest to check out his blog post about this: 
 * https://www.baldengineer.com/arduino-keyboard-matrix-tutorial.html
 */

#include <Keyboard.h>

#define SHIFT KEY_LEFT_CTRL

byte rowPins[] = {2, 3, 4, 5};
const int rows = sizeof(rowPins)/sizeof(rowPins[0]);

byte colPins[] = {6, 10, 8, 9};
const int cols = sizeof(colPins)/sizeof(colPins[0]);

byte keys[cols][rows];
int actions[cols][rows] = {
  KEY_F13, KEY_F14, KEY_F15, KEY_F16,
  KEY_F17, KEY_F18, KEY_F19, KEY_F20,
  KEY_F13, KEY_F14, KEY_F15, KEY_F16,
  KEY_F17, KEY_F18, KEY_F19, KEY_F20
};
 
void setup() {
    Serial.begin(115200);
        
    for(int x=0; x<rows; x++) {
        pinMode(rowPins[x], INPUT);
    }
 
    for (int x=0; x<cols; x++) {
        pinMode(colPins[x], INPUT_PULLUP);
    }

    Keyboard.begin();
}
 
void readMatrix() {
    for (int j=0; j < cols; j++) {
        byte col = colPins[j];
        pinMode(col, OUTPUT);
        digitalWrite(col, LOW);
        for (int i=0; i < rows; i++) {
            byte row = rowPins[i];
            pinMode(row, INPUT_PULLUP);
            bool input = digitalRead(row);
            if (!input && !keys[j][i]) {
              if (j == cols -1 || j == cols - 2) {
                Keyboard.press(SHIFT);
              }
              Keyboard.press(actions[j][i]);
              keys[j][i] = true;
            } else if (input && keys[j][i]) {
              Keyboard.releaseAll();
              keys[j][i] = false;
            }
            pinMode(row, INPUT);
        }
        pinMode(col, INPUT);
    }
}
 
void loop() {
    readMatrix();
    if(Serial.read() == '!') {
      printMatrix();
    }
    delay(30);
}

void printMatrix() {
    for (int i=0; i < rows; i++) {
        if (i < 10) {
            Serial.print(F("0"));
        }
        Serial.print(i); Serial.print(F(": "));
        
        for (int j=0; j < cols; j++) {  
            Serial.print(keys[j][i]);
            if (j < cols) {
                Serial.print(F(", ")); 
            }
        }   
        Serial.println("");
    }
    Serial.println("");
}
