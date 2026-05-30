#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
const int speakerPin = 9;
const int buttonPins[] = {2,3,4,5,6,7,8};
const int notes[] = {262,294,330,349,392,440,494};
const String noteNames[] = {"DO","RE","MI","FA","SOL","LA","SI"};
const int numButtons = 7;
int lastNotePlayed = -1;

void setup() {
  pinMode(speakerPin, OUTPUT);
  for(int i=0;i<numButtons;i++) pinMode(buttonPins[i], INPUT_PULLUP);
  lcd.init(); lcd.backlight();
  lcd.print("  PIANO ARDUINO ");
  lcd.setCursor(0,1); lcd.print("Presiona una tec");
}
void loop() {
  bool pressed = false;
  int currentNote = -1;
  for(int i=0;i<numButtons;i++) {
    if(digitalRead(buttonPins[i]) == LOW) {
      pressed = true;
      currentNote = i;
      break;
    }
  }
  if(pressed) {
    tone(speakerPin, notes[currentNote]);
    if(currentNote != lastNotePlayed) {
      lcd.clear();
      lcd.print("Nota: " + noteNames[currentNote]);
      lcd.setCursor(0,1);
      lcd.print("Freq: " + String(notes[currentNote]) + " Hz");
      lastNotePlayed = currentNote;
    }
  } else {
    noTone(speakerPin);
    if(lastNotePlayed != -1) {
      lcd.clear();
      lcd.print("  PIANO ARDUINO ");
      lcd.setCursor(0,1); lcd.print("Presiona una tec");
      lastNotePlayed = -1;
    }
  }
  delay(10);
}
