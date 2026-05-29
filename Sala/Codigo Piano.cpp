#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin de salida de audio hacia el TIP41C
const int speakerPin = 9;

// Configuración de botones y notas
const int buttonPins[] = {2, 3, 4, 5, 6, 7, 8};
const int notes[] = {262, 294, 330, 349, 392, 440, 494};
const String noteNames[] = {"DO", "RE", "MI", "FA", "SOL", "LA", "SI"};
const int numButtons = 7;

// Inicializa la pantalla LCD en la dirección 0x27 (o 0x3F en algunos modelos) con 16 columnas y 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

int lastNotePlayed = -1; // Almacena la última nota para evitar parpadeos en la pantalla

void setup() {
  pinMode(speakerPin, OUTPUT);
  
  // Configura botones
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Inicializa la pantalla LCD
  lcd.init();
  lcd.backlight();
  mostrarPantallaInicio();
}

void loop() {
  bool buttonPressed = false;
  int currentNoteIndex = -1;

  // Revisa si hay algún botón presionado
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      currentNoteIndex = i;
      buttonPressed = true;
      break; 
    }
  }

  if (buttonPressed) {
    tone(speakerPin, notes[currentNoteIndex]);
    
    // Solo actualiza la pantalla si la nota cambió
    if (currentNoteIndex != lastNotePlayed) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nota: " + noteNames[currentNoteIndex]);
      lcd.setCursor(0, 1);
      lcd.print("Freq: " + String(notes[currentNoteIndex]) + " Hz");
      lastNotePlayed = currentNoteIndex;
    }
  } else {
    noTone(speakerPin);
    // Si se soltó el botón, vuelve a la pantalla de inicio una sola vez
    if (lastNotePlayed != -1) {
      mostrarPantallaInicio();
      lastNotePlayed = -1;
    }
  }
  
  delay(10); 
}

// Función auxiliar para el estado de espera
void mostrarPantallaInicio() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  PIANO ARDUINO ");
  lcd.setCursor(0, 1);
  lcd.print("Presiona una tec");
}
