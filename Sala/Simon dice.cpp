#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pines de hardware
const int speakerPin = 9;
const int buttonPins[] = {2, 3, 4, 5}; // Pines del 2 al 5
const int notes[] = {262, 294, 330, 349}; // DO (262Hz), RE (294Hz), MI (330Hz), FA (349Hz)
const String noteNames[] = {"DO", "RE", "MI", "FA"}; // Nombres musicales en pantalla
const int numButtons = 4;

// Configuración del juego
const int MAX_PASOS = 50;       
int secuencia[MAX_PASOS];       
int nivel = 0;                  

// Variables para la velocidad dinámica (Dificultad progresiva)
int tiempoMuestraInicial = 500;  // Tiempo inicial que dura la nota encendida (milisegundos)
int tiempoPausaInicial = 250;    // Pausa inicial entre notas
int factorAceleracion = 25;      // Cuánto se acelera (resta en ms) por cada nivel avanzado
int tiempoMuestraMinimo = 150;   // Límite de velocidad máxima para la muestra
int tiempoPausaMinimo = 80;      // Límite de velocidad máxima para la pausa

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(speakerPin, OUTPUT);
  digitalWrite(speakerPin, LOW);

  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(A0)); 
  
  mostrarPantallaInicio();
}

void loop() {
  // 1. Añadir un nuevo paso aleatorio a la secuencia
  secuencia[nivel] = random(0, numButtons);
  nivel++;

  // Calcular tiempos dinámicos para este nivel específico
  int tiempoMuestraActual = tiempoMuestraInicial - (nivel * factorAceleracion);
  int tiempoPausaActual = tiempoPausaInicial - (nivel * (factorAceleracion / 2));
  
  // Asegurar que el juego no se vuelva imposible superando los límites físicos
  if (tiempoMuestraActual < tiempoMuestraMinimo) tiempoMuestraActual = tiempoMuestraMinimo;
  if (tiempoPausaActual < tiempoPausaMinimo) tiempoPausaActual = tiempoPausaMinimo;

  // 2. Mostrar la secuencia al jugador
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("SIMON DICE:");
  lcd.setCursor(4, 1);
  lcd.print("Nivel: ");
  lcd.print(nivel);
  delay(1000);

  for (int i = 0; i < nivel; i++) {
    int botonActual = secuencia[i];
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Escucha: ");
    lcd.print(noteNames[botonActual]);
    
    tone(speakerPin, notes[botonActual]);
    delay(tiempoMuestraActual); // Tiempo dinámico que se reduce cada nivel
    noTone(speakerPin);
    digitalWrite(speakerPin, LOW);
    delay(tiempoPausaActual);   // Pausa dinámica que se reduce cada nivel
  }

  // 3. Esperar a que el jugador repita la secuencia
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("TU TURNO...");
  lcd.setCursor(2, 1);
  lcd.print("¡Repitelo!");

  for (int i = 0; i < nivel; i++) {
    int botonCorrecto = secuencia[i];
    int botonPresionado = -1;

    // Bucle de espera hasta que presione un botón
    while (botonPresionado == -1) {
      for (int b = 0; b < numButtons; b++) {
        if (digitalRead(buttonPins[b]) == LOW) {
          botonPresionado = b;
          
          tone(speakerPin, notes[b]);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Tocaste: ");
          lcd.print(noteNames[b]);
          
          while (digitalRead(buttonPins[b]) == LOW) { delay(10); }
          noTone(speakerPin);
          digitalWrite(speakerPin, LOW);
          break;
        }
      }
      delay(10);
    }

    // Verificar error
    if (botonPresionado != botonCorrecto) {
      ejecutarGameOver();
      return; 
    }
  }

  // Si completó la secuencia actual con éxito: Sonido de victoria corta
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("¡CORRECTO!");
  lcd.setCursor(3, 1);
  lcd.print("Aumentando...");
  
  tone(speakerPin, 523); delay(100);
  tone(speakerPin, 659); delay(100);
  tone(speakerPin, 784); delay(150);
  noTone(speakerPin);
  digitalWrite(speakerPin, LOW);
  
  delay(1000);
}

void mostrarPantallaInicio() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("SIMON DICE");
  lcd.setCursor(1, 1);
  lcd.print("Presiona Boton");
  
  // Sonido de bienvenida en bucle suave mientras espera
  bool empezar = false;
  int notaIntro = 0;
  while (!empezar) {
    for (int i = 0; i < numButtons; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        empezar = true;
        // Efecto de sonido de arranque rápido
        tone(speakerPin, 440); delay(100);
        tone(speakerPin, 554); delay(100);
        tone(speakerPin, 659); delay(150);
        noTone(speakerPin);
        digitalWrite(speakerPin, LOW);
        delay(200);
        break;
      }
    }
    delay(10);
  }
  nivel = 0; 
}

void ejecutarGameOver() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("¡ERROR!");
  lcd.setCursor(2, 1);
  lcd.print("Puntaje: ");
  lcd.print(nivel - 1); 

  // Sonido clásico de Game Over descendente y triste
  tone(speakerPin, 300); delay(400);
  tone(speakerPin, 200); delay(400);
  tone(speakerPin, 130); delay(800);
  noTone(speakerPin);
  digitalWrite(speakerPin, LOW);
  
  delay(2000);
  mostrarPantallaInicio();
}
