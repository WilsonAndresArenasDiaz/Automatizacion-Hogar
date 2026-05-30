```
// ==========================================================
// 1. LIBRERÍAS DEL PROYECTO
// ==========================================================
//
// Wire.h
// Permite la comunicación I2C entre Arduino y dispositivos
// externos como la pantalla LCD.
//
// LiquidCrystal_I2C.h
// Permite controlar la pantalla LCD mediante el protocolo I2C.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==========================================================
// 2. CONFIGURACIÓN DE LA PANTALLA LCD
// ==========================================================
//
// Se crea un objeto LCD indicando:
//
// Dirección I2C : 0x27
// Columnas      : 16
// Filas         : 2
//
// Esto permitirá mostrar menús, puntajes y mensajes.

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==========================================================
// 3. CONFIGURACIÓN DE PINES Y NOTAS MUSICALES
// ==========================================================
//
// Pin conectado al parlante o buzzer mediante TIP41C.

const int speakerPin = 9;

// Arreglo que almacena los pines de los botones musicales.
//
// Pin 2 -> DO
// Pin 3 -> RE
// Pin 4 -> MI
// Pin 5 -> FA
// Pin 6 -> SOL
// Pin 7 -> LA
// Pin 8 -> SI

const int buttonPins[] = {2, 3, 4, 5, 6, 7, 8};

// Frecuencias de cada nota musical en Hertz.

const int notes[] = {262, 294, 330, 349, 392, 440, 494};

// Nombre de cada nota para mostrarla en la LCD.

const String noteNames[] = {
  "DO", "RE", "MI", "FA", "SOL", "LA", "SI"
};

// Cantidad total de botones musicales.

const int numButtons = 7;

// Botón especial conectado al Pin 12.
//
// Funciones:
//
// - Saltar en Dino
// - Salir del Piano
// - Cancelar Simón Dice
// - Regresar al Menú Principal

const int escapeButton = 12;

// ==========================================================
// 4. CARACTERES PERSONALIZADOS LCD
// ==========================================================
//
// Se define el gráfico del dinosaurio utilizando una matriz
// de 8 filas con formato binario.
//
// Cada número binario representa una fila de píxeles
// del carácter personalizado almacenado en la LCD.

byte dinoCustom[8] = {
  B00111,
  B00101,
  B00111,
  B10110,
  B11111,
  B01010,
  B01010,
  B00000
};

// Gráfico personalizado del cactus utilizado como obstáculo.

byte cactusCustom[8] = {
  B00100,
  B00101,
  B10101,
  B10111,
  B11100,
  B00100,
  B00100,
  B00000
};

// ==========================================================
// 5. VARIABLES DE CONTROL DEL SISTEMA
// ==========================================================
//
// estadoActual controla qué pantalla o juego se encuentra
// ejecutándose actualmente.
//
// 0 = Menú Principal
// 1 = Menú Consola
// 2 = Piano
// 3 = Dinosaurio
// 4 = Simón Dice

int estadoActual = 0;

// Guarda el estado anterior del botón Escape.
//
// Se utiliza para detectar cambios de estado y evitar
// múltiples lecturas de una sola pulsación.

bool lastEscapeState = HIGH;

// ==========================================================
// 6. VARIABLES DEL JUEGO DINO
// ==========================================================
//
// Número máximo de cactus simultáneos que pueden existir
// en pantalla.

const int MAX_CACTUS = 3;

// Posición horizontal de cada cactus.

int cactusPos[MAX_CACTUS];

// Fila donde aparece cada cactus.
//
// 0 = Superior
// 1 = Inferior

int cactusRow[MAX_CACTUS];

// Indica si el cactus está activo o no.

bool cactusActivo[MAX_CACTUS];

// Posición actual del dinosaurio.
//
// 0 = Arriba
// 1 = Abajo

int dinoPos = 1;

// Puntaje acumulado del jugador.

int scoreDino = 0;

// Velocidad actual del juego.

int velDino = 240;

// ==========================================================
// 7. VARIABLES DEL JUEGO SIMÓN DICE
// ==========================================================
//
// Longitud máxima de la secuencia generada.

const int MAX_PASOS = 50;

// Arreglo donde se almacenan las notas aleatorias
// que debe memorizar el jugador.

int secuencia[MAX_PASOS];

// Nivel actual alcanzado.

int nivelSimon = 0;
```
