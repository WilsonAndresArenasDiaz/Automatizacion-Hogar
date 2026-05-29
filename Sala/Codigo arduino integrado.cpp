#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines de hardware globales
const int speakerPin = 9;
const int buttonPins[] = {2, 3, 4, 5, 6, 7, 8};
const int notes[] = {262, 294, 330, 349, 392, 440, 494};
const String noteNames[] = {"DO", "RE", "MI", "FA", "SOL", "LA", "SI"};
const int numButtons = 7;
const int escapeButton = 12; // Pin 12 sirve para saltar (en Dino), cancelar o volver

// Gráficos originales del Dinosaurio restaurados
byte dinoCustom[8] = {
  B00111, B00101, B00111, B10110, B11111, B01010, B01010, B00000
};
byte cactusCustom[8] = {
  B00100, B00101, B10101, B10111, B11100, B00100, B00100, B00000
};

// Variables de Control
// 0=Menu Principal, 1=Menu Consola, 2=Modo Piano, 3=Modo Dino, 4=Modo Simon Dice
int estadoActual = 0; 
bool lastEscapeState = HIGH;

// Lógica de múltiples cactus
const int MAX_CACTUS = 3;
int cactusPos[MAX_CACTUS];
int cactusRow[MAX_CACTUS];
bool cactusActivo[MAX_CACTUS];
int dinoPos = 1;
int scoreDino = 0;
int velDino = 240;

// Lógica de Simón Dice
const int MAX_PASOS = 50;
int secuencia[MAX_PASOS];
int nivelSimon = 0;

void setup() {
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);
  digitalWrite(speakerPin, LOW);
  pinMode(escapeButton, INPUT_PULLUP);

  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  lcd.init();
  lcd.backlight();
  
  // Registrar los caracteres especiales en la memoria de la LCD
  lcd.createChar(0, dinoCustom);
  lcd.createChar(1, cactusCustom);
  
  randomSeed(analogRead(A0));
  irMenuPrincipal();
}

void loop() {
  // --- CONTROL SERIAL POR VOZ DESDE PC ---
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    if (comando == "PIANO") { iniciarModoPiano(); }
    else if (comando == "CONSOLA") { irMenuConsola(); }
    else if (comando == "DINO") { iniciarModoDino(); }
    else if (comando == "SIMON") { iniciarModoSimon(); }
    else if (comando == "MENU") { irMenuPrincipal(); }
  }

  // --- BOTÓN GLOBAL DE CANCELAR/VOLVER (Pin 12) ---
  // Solo actúa como escape si estamos dentro de un juego/modo y no en los menús
  if (estadoActual > 1) {
    // En el modo Dino, el pin 12 se usa para saltar, por lo que la cancelación 
    // se maneja directamente dentro de la función de juego si se deja presionado.
    if (estadoActual != 3 && digitalRead(escapeButton) == LOW) {
      pitidoMenu();
      irMenuPrincipal();
      delay(300);
    }
  }

  // --- CONTROL DE ESTADOS ---
  switch (estadoActual) {
    case 0: // Menú Principal
      if (digitalRead(2) == LOW) { pitidoMenu(); iniciarModoPiano(); }
      if (digitalRead(3) == LOW) { pitidoMenu(); irMenuConsola(); }
      break;

    case 1: // Menú Consola
      if (digitalRead(2) == LOW) { pitidoMenu(); iniciarModoDino(); }
      if (digitalRead(3) == LOW) { pitidoMenu(); iniciarModoSimon(); }
      if (digitalRead(escapeButton) == LOW) { pitidoMenu(); irMenuPrincipal(); delay(300); }
      break;

    case 2: // Ejecución Piano
      ejecutarPiano();
      break;

    case 3: // Ejecución Dino
      ejecutarDino();
      break;

    case 4: // Ejecución Simón Dice
      ejecutarSimon();
      break;
  }
  delay(10);
}

// === TRANSICIONES Y MENÚS ===
void pitidoMenu() {
  tone(speakerPin, 600); delay(80); noTone(speakerPin);
}

void sonidoDerrota() {
  tone(speakerPin, 300); delay(300);
  tone(speakerPin, 200); delay(300);
  tone(speakerPin, 130); delay(600);
  noTone(speakerPin);
}

void irMenuPrincipal() {
  estadoActual = 0;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Hola bienvenido");
  lcd.setCursor(0, 1); lcd.print("DO:Pian  RE:Cons");
  delay(200);
}

void irMenuConsola() {
  estadoActual = 1;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("PlayStation Menu");
  lcd.setCursor(0, 1); lcd.print("DO:Dino  RE:Simo");
  delay(200);
}

// Retorna 'true' si elige seguir (Boton DO) o 'false' si elige volver (Boton 12)
bool mostrarMenuPostJuego(int puntajeFinal) {
  sonidoDerrota();
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("DO:Seguir   S:" + String(puntajeFinal));
  lcd.setCursor(0, 1); lcd.print("Pin12:Volver");
  delay(500); // Pequeña pausa de seguridad

  while (true) {
    if (digitalRead(2) == LOW) { // Botón DO
      pitidoMenu();
      return true; 
    }
    if (digitalRead(escapeButton) == LOW) { // Botón 12
      pitidoMenu();
      return false; 
    }
    delay(10);
  }
}

// === MODO PIANO ===
void iniciarModoPiano() {
  estadoActual = 2;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("  MODO PIANO   ");
  lcd.setCursor(0, 1); lcd.print("12:Salir  Teclas");
}

void ejecutarPiano() {
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      tone(speakerPin, notes[i]);
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Nota: " + noteNames[i]);
      lcd.setCursor(0, 1); lcd.print("Freq: " + String(notes[i]) + " Hz");
      while (digitalRead(buttonPins[i]) == LOW) {
        // Permitir salir incluso si dejas una tecla presionada
        if (digitalRead(escapeButton) == LOW) { noTone(speakerPin); return; }
        delay(5); 
      }
      noTone(speakerPin);
      iniciarModoPiano(); 
      break;
    }
  }
}

// === MODO DINO ===
void iniciarModoDino() {
  estadoActual = 3;
  scoreDino = 0;
  velDino = 240;
  dinoPos = 1;
  for (int i = 0; i < MAX_CACTUS; i++) cactusActivo[i] = false;
  lcd.clear();
  lcd.setCursor(3, 0); lcd.print("DINO READY");
  delay(800);
}

void ejecutarDino() {
  bool btn = digitalRead(escapeButton);
  
  // El pin 12 controla el salto del dino con un toque rápido
  if (btn == LOW && lastEscapeState == HIGH) {
    dinoPos = (dinoPos == 1) ? 0 : 1;
    tone(speakerPin, 440); delay(30); noTone(speakerPin); 
  }
  lastEscapeState = btn;

  // Lógica de generación de cactus
  if (random(0, 100) < 30) {
    for (int i = 0; i < MAX_CACTUS; i++) {
      if (cactusActivo[i] && cactusPos[i] >= 12) return;
    }
    for (int i = 0; i < MAX_CACTUS; i++) {
      if (!cactusActivo[i]) {
        cactusPos[i] = 15; cactusRow[i] = random(0, 2); cactusActivo[i] = true;
        break;
      }
    }
  }

  lcd.clear();
  lcd.setCursor(11, 0); lcd.print("S:" + String(scoreDino));
  
  // ¡RESTAURADO! Imprime el carácter personalizado 0 (Dinosaurio)
  lcd.setCursor(0, dinoPos); 
  lcd.write(byte(0)); 

  for (int i = 0; i < MAX_CACTUS; i++) {
    if (cactusActivo[i]) {
      if (cactusPos[i] >= 0 && cactusPos[i] < 16) {
        // ¡RESTAURADO! Imprime el carácter personalizado 1 (Cactus)
        lcd.setCursor(cactusPos[i], cactusRow[i]); 
        lcd.write(byte(1));
      }
      
      // Detección de Colisión
      if (cactusPos[i] == 0 && dinoPos == cactusRow[i]) {
        if (mostrarMenuPostJuego(scoreDino)) {
          iniciarModoDino(); // Elige Seguir
        } else {
          irMenuPrincipal(); // Elige Volver
        }
        return;
      }
      
      cactusPos[i]--;
      if (cactusPos[i] < 0) {
        cactusActivo[i] = false; scoreDino++;
        velDino = max(80, 240 - (scoreDino * 6));
      }
    }
  }
  delay(velDino);
}

// === MODO SIMON DICE ===
void iniciarModoSimon() {
  estadoActual = 4;
  nivelSimon = 0;
  lcd.clear();
  lcd.setCursor(2, 0); lcd.print("SIMON READY");
  delay(800);
}

void ejecutarSimon() {
  secuencia[nivelSimon] = random(0, 4); // Botones del 2 al 5
  nivelSimon++;

  int tMuestra = max(150, 500 - (nivelSimon * 25));
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("SIMON DICE N:" + String(nivelSimon));
  delay(800);

  // Muestra de secuencia
  for (int i = 0; i < nivelSimon; i++) {
    // Si se presiona el Pin 12 durante la muestra, sale al menú principal
    if (digitalRead(escapeButton) == LOW) { irMenuPrincipal(); return; }
    
    int b = secuencia[i];
    lcd.clear(); lcd.setCursor(0, 0); lcd.print("Escucha: " + noteNames[b]);
    tone(speakerPin, notes[b]); delay(tMuestra); noTone(speakerPin); delay(150);
  }

  lcd.clear(); lcd.print("TU TURNO...");
  for (int i = 0; i < nivelSimon; i++) {
    int correcto = secuencia[i];
    int presionado = -1;

    while (presionado == -1) {
      // Permite abortar el juego en su turno con el Pin 12
      if (digitalRead(escapeButton) == LOW) { irMenuPrincipal(); return; }
      
      for (int b = 0; b < 4; b++) {
        if (digitalRead(buttonPins[b]) == LOW) {
          presionado = b;
          tone(speakerPin, notes[b]);
          lcd.clear(); lcd.print("Tocaste: " + noteNames[b]);
          while (digitalRead(buttonPins[b]) == LOW);
          noTone(speakerPin); break;
        }
      }
      delay(10);
    }

    // Si comete un error
    if (presionado != correcto) {
      if (mostrarMenuPostJuego(nivelSimon - 1)) {
        iniciarModoSimon(); // Elige Seguir
      } else {
        irMenuPrincipal(); // Elige Volver
      }
      return;
    }
  }
  lcd.clear(); lcd.print("CORRECTO!");
  tone(speakerPin, 523); delay(100); tone(speakerPin, 659); delay(150); noTone(speakerPin);
  delay(800);
}
