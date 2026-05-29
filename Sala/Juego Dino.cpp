#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Diseños de los personajes
byte dino[8] = {
  B00111, B00101, B00111, B10110, B11111, B01010, B01010, B00000
};
byte cactus[8] = {
  B00100, B00101, B10101, B10111, B11100, B00100, B00100, B00000
};

int dinoPos = 1;             // 1 = abajo, 0 = arriba
int score = 0;
bool lastButtonState = HIGH; 

// --- CONFIGURACIÓN DE MÚLTIPLES CACTUS ---
const int MAX_CACTUS = 3;       // Máximo de cactus simultáneos en pantalla
int cactusPos[MAX_CACTUS];      // Posiciones horizontales de cada cactus
int cactusRow[MAX_CACTUS];      // Fila de cada cactus (0 o 1)
bool cactusActivo[MAX_CACTUS];   // Saber si el cactus está jugando o esperando

// --- DIFICULTAD DINÁMICA ---
int velocidadInicial = 240;  
int velocidad = velocidadInicial; 
int velocidadMinima = 80;    
int factorAceleracion = 6;   // Se acelera un poco más suave por la cantidad de obstáculos

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);
  
  pinMode(12, INPUT_PULLUP); 
  randomSeed(analogRead(A0)); 
  
  // ¡CORREGIDO! Se eliminó el espacio accidental
  reiniciarVariablesJuego();
  mostrarPantallaInicio();
}

void loop() {
  // --- LECTURA DEL BOTÓN ---
  bool buttonState = digitalRead(12);
  if (buttonState == LOW && lastButtonState == HIGH) {
    dinoPos = (dinoPos == 1) ? 0 : 1; 
    delay(40); // Antirebote rápido
  }
  lastButtonState = buttonState;

  // --- LOGICA DE GENERACIÓN DE CACTUS INESPERADOS ---
  if (random(0, 100) < 30) { // 30% de probabilidad en cada ciclo
    generarNuevoCactus();
  }

  // --- RENDERIZADO EN PANTALLA ---
  lcd.clear(); 

  // Dibuja el Score
  lcd.setCursor(11, 0);
  lcd.print("S:");
  lcd.print(score);

  // Dibuja el Dinosaurio
  lcd.setCursor(0, dinoPos);
  lcd.write(byte(0));

  // --- ACTUALIZACIÓN Y DIBUJO DE CADA CACTUS ---
  for (int i = 0; i < MAX_CACTUS; i++) {
    if (cactusActivo[i]) {
      // Dibuja el cactus si está dentro de los límites visuales
      if (cactusPos[i] >= 0 && cactusPos[i] < 16) {
        lcd.setCursor(cactusPos[i], cactusRow[i]);
        lcd.write(byte(1));
      }

      // Detección estricta de colisión
      if (cactusPos[i] == 0 && dinoPos == cactusRow[i]) {
        ejecutarGameOver();
        return; 
      }

      // Mueve el cactus hacia la izquierda
      cactusPos[i]--;

      // Si el cactus superó la pantalla, se desactiva y suma puntos
      if (cactusPos[i] < 0) {
        cactusActivo[i] = false;
        score++;
        
        // Escalado de velocidad dinámico
        velocidad = velocidadInicial - (score * factorAceleracion);
        if (velocidad < velocidadMinima) velocidad = velocidadMinima;
      }
    }
  }

  delay(velocidad); 
}

// Función para activar un cactus en la reserva si las condiciones se cumplen
void generarNuevoCactus() {
  // Verifica si el último cactus lanzado ya avanzó lo suficiente
  for (int i = 0; i < MAX_CACTUS; i++) {
    if (cactusActivo[i] && cactusPos[i] >= 12) {
      return; 
    }
  }

  // Busca un espacio libre en el arreglo para activar el nuevo cactus
  for (int i = 0; i < MAX_CACTUS; i++) {
    if (!cactusActivo[i]) {
      cactusPos[i] = 15;          
      cactusRow[i] = random(0, 2); 
      cactusActivo[i] = true;     
      break;                      
    }
  }
}

void reiniciarVariablesJuego() {
  dinoPos = 1;
  score = 0;
  velocidad = velocidadInicial;
  for (int i = 0; i < MAX_CACTUS; i++) {
    cactusActivo[i] = false;
    cactusPos[i] = -1;
  }
}

void mostrarPantallaInicio() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("DINO MULTI");
  lcd.setCursor(1, 1);
  lcd.print("Presiona Pin 12");
  while(digitalRead(12) == HIGH) {
    delay(10); 
  }
  lcd.clear();
}

void ejecutarGameOver() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GAME OVER");
  lcd.setCursor(4, 1);
  lcd.print("Score: ");
  lcd.print(score);
  delay(3000); 
  
  reiniciarVariablesJuego();
  mostrarPantallaInicio();
}

