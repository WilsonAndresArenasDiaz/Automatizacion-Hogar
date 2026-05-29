# 🦖 Juego Dino Multi Obstáculos - Arduino + LCD I2C

## 📌 Descripción del proyecto

Este proyecto crea un juego estilo Chrome Dino utilizando:

* Arduino
* Pantalla LCD I2C 16x2
* Botón en el pin 12
* Obstáculos múltiples (cactus)
* Sistema de velocidad dinámica

El dinosaurio puede moverse entre la parte superior e inferior de la pantalla para esquivar cactus.


# 📚 Librerías utilizadas

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
```

## Explicación

### `#include <Wire.h>`

Activa la comunicación I2C del Arduino.

La pantalla LCD I2C usa este protocolo para comunicarse usando solo 2 cables:

* SDA
* SCL

### `#include <LiquidCrystal_I2C.h>`

Carga la librería especial para controlar pantallas LCD con módulo I2C.


# 🖥️ Configuración de la pantalla LCD

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

## Explicación

Se crea el objeto `lcd`.

### Parámetros:

| Parámetro | Significado                  |
| --------- | ---------------------------- |
| `0x27`    | Dirección I2C de la pantalla |
| `16`      | Número de columnas           |
| `2`       | Número de filas              |

# 🦖 Diseño del dinosaurio
```cpp
byte dino[8] = {
  B00111,
  B00101,
  B00111,
  B10110,
  B11111,
  B01010,
  B01010,
  B00000
};
```

## Explicación

Se crea un carácter personalizado de 8 filas.

Cada `1` representa un píxel encendido.

Ejemplo visual:
```text
 ███
 █ █
 ███
█ ██
█████
 █ █
 █ █
```


# 🌵 Diseño del cactus
```cpp
byte cactus[8] = {
  B00100,
  B00101,
  B10101,
  B10111,
  B11100,
  B00100,
  B00100,
  B00000
};
```

## Explicación

Se diseña el obstáculo del juego.


# 🎮 Variables principales
```cpp
int dinoPos = 1;
```
## Explicación

Controla la posición vertical del dinosaurio.

| Valor | Posición |
| ----- | -------- |
| `0`   | Arriba   |
| `1`   | Abajo    |

```cpp
int score = 0;
```
Guarda el puntaje del jugador.

```cpp
bool lastButtonState = HIGH;
```
Guarda el estado anterior del botón.

Sirve para detectar solo una pulsación y evitar múltiples cambios rápidos.

# 🌵 Configuración de múltiples cactus
```cpp
const int MAX_CACTUS = 3;
```
Máximo número de cactus simultáneos.


```cpp
int cactusPos[MAX_CACTUS];
```

Guarda la posición horizontal de cada cactus.


```cpp
int cactusRow[MAX_CACTUS];
```

Guarda en qué fila está cada cactus.

| Valor | Fila   |
| ----- | ------ |
| `0`   | Arriba |
| `1`   | Abajo  |


```cpp
bool cactusActivo[MAX_CACTUS];
```

Indica si el cactus está activo en pantalla.


# ⚡ Sistema de dificultad

```cpp
int velocidadInicial = 240;
```

Velocidad inicial del juego en milisegundos.


```cpp
int velocidad = velocidadInicial;
```

Velocidad actual del juego.


```cpp
int velocidadMinima = 80;
```

Límite mínimo de velocidad.


```cpp
int factorAceleracion = 6;
```

Cada punto aumenta la velocidad reduciendo el delay.


# ⚙️ Función setup()

```cpp
void setup() {
```

Función que se ejecuta una sola vez al encender Arduino.


## Inicialización LCD

```cpp
lcd.init();
```

Inicializa la pantalla LCD.


```cpp
lcd.backlight();
```

Enciende la luz de fondo.


## Crear caracteres personalizados

```cpp
lcd.createChar(0, dino);
lcd.createChar(1, cactus);
```

Guarda los caracteres en memoria.

| Número | Personaje |
| ------ | --------- |
| `0`    | Dino      |
| `1`    | Cactus    |


## Configurar botón

```cpp
pinMode(12, INPUT_PULLUP);
```

El pin 12 funciona como entrada con resistencia pull-up interna.

### Funcionamiento

| Estado        | Valor |
| ------------- | ----- |
| Sin presionar | HIGH  |
| Presionado    | LOW   |


## Aleatoriedad

```cpp
randomSeed(analogRead(A0));
```

Genera números aleatorios usando ruido eléctrico del pin A0.


## Reinicio del juego

```cpp
reiniciarVariablesJuego();
```

Restablece todas las variables.


## Pantalla de inicio

```cpp
mostrarPantallaInicio();
```

Muestra mensaje antes de iniciar.


# 🔄 Función loop()

```cpp
void loop() {
```

Se ejecuta infinitamente.


# 🔘 Lectura del botón

```cpp
bool buttonState = digitalRead(12);
```

Lee el estado del botón.


## Detectar pulsación

```cpp
if (buttonState == LOW && lastButtonState == HIGH)
```

Detecta el momento exacto de presionar.


## Cambiar posición del dinosaurio

```cpp
dinoPos = (dinoPos == 1) ? 0 : 1;
```

Si está abajo sube.

Si está arriba baja.


## Antirebote

```cpp
delay(40);
```

Evita falsas pulsaciones.


## Guardar estado anterior

```cpp
lastButtonState = buttonState;
```

Actualiza el estado del botón.


# 🌵 Generación aleatoria de cactus

```cpp
if (random(0, 100) < 30)
```

30% de probabilidad de generar un cactus nuevo.


```cpp
generarNuevoCactus();
```

Activa un cactus nuevo.


# 🖥️ Dibujar pantalla

```cpp
lcd.clear();
```

Limpia toda la pantalla.


# 🏆 Mostrar score

```cpp
lcd.setCursor(11, 0);
lcd.print("S:");
lcd.print(score);
```

Muestra el puntaje.


# 🦖 Dibujar dinosaurio

```cpp
lcd.setCursor(0, dinoPos);
lcd.write(byte(0));
```

Dibuja el dino en columna 0.


# 🌵 Bucle de cactus

```cpp
for (int i = 0; i < MAX_CACTUS; i++)
```

Recorre todos los cactus.


# Dibujar cactus

```cpp
lcd.setCursor(cactusPos[i], cactusRow[i]);
lcd.write(byte(1));
```

Dibuja el cactus.


# 💥 Detección de colisión

```cpp
if (cactusPos[i] == 0 && dinoPos == cactusRow[i])
```

Si el cactus llega al dino y están en la misma fila:

➡️ GAME OVER


# Movimiento del cactus

```cpp
cactusPos[i]--;
```

El cactus se mueve a la izquierda.


# Punto ganado

```cpp
if (cactusPos[i] < 0)
```

Cuando sale de pantalla:

* Se desactiva
* Suma punto


# ⚡ Aumento de dificultad
```cpp
velocidad = velocidadInicial - (score * factorAceleracion);
```
El juego se acelera con cada punto.


# Limitar velocidad mínima
```cpp
if (velocidad < velocidadMinima)
```
Evita que el juego sea imposible.


# ⏱️ Delay principal
```cpp
delay(velocidad);
```
Controla la velocidad general.


# 🌵 Función generarNuevoCactus()
```cpp
void generarNuevoCactus()
```
Genera cactus nuevos.

## Evitar cactus muy pegados
```cpp
if (cactusActivo[i] && cactusPos[i] >= 12)
```
No deja crear cactus demasiado juntos.


## Activar cactus
```cpp
cactusPos[i] = 15;
```
Aparece al extremo derecho.

```cpp
cactusRow[i] = random(0, 2);
```
Fila aleatoria.

```cpp
cactusActivo[i] = true;
```
Activa el cactus.


# 🔄 Función reiniciarVariablesJuego()

```cpp
void reiniciarVariablesJuego()
```
Reinicia todas las variables del juego.


# 🖥️ Pantalla de inicio

```cpp
void mostrarPantallaInicio()
```
Muestra mensaje inicial.

```cpp
while(digitalRead(12) == HIGH)
```
Espera hasta presionar el botón.


# 💀 Función Game Over

```cpp
void ejecutarGameOver()
```
Muestra pantalla de derrota.


```cpp
lcd.print("GAME OVER");
```
Mensaje principal.

```cpp
lcd.print(score);
```
Muestra puntaje final.
```cpp
delay(3000);
```
Espera 3 segundos.

# 🔁 Reinicio automático
```cpp
reiniciarVariablesJuego();
mostrarPantallaInicio();
```
Reinicia el juego.

# 🎵 Simon Dice - Arduino + LCD I2C + Buzzer

# 📌 Descripción del proyecto

Este proyecto recrea el clásico juego **Simon Dice** usando:

* Arduino UNO
* Pantalla LCD I2C 16x2
* 4 botones
* Buzzer/Speaker
* Notas musicales
* Dificultad progresiva

El jugador debe memorizar y repetir una secuencia de sonidos y botones.

Cada nivel:

✅ Añade una nueva nota
✅ Aumenta la dificultad
✅ Reduce el tiempo de reacción

# 🧰 Librerías utilizadas

```cpp id="2s7u8k"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
```

## 📚 Explicación

### `#include <Wire.h>`

Activa la comunicación I2C del Arduino.

La pantalla LCD usa este protocolo para comunicarse usando solo:

* SDA
* SCL

### `#include <LiquidCrystal_I2C.h>`

Carga la librería necesaria para controlar la pantalla LCD I2C.

# 🔌 Pines del hardware
```cpp id="elgh8h"
const int speakerPin = 9;
```

## Explicación

Pin donde está conectado el buzzer o speaker.

```cpp id="k5n6lg"
const int buttonPins[] = {2, 3, 4, 5};
```

## Explicación

Arreglo con los pines de los 4 botones.

| Botón | Pin |
| ----- | --- |
| 1     | 2   |
| 2     | 3   |
| 3     | 4   |
| 4     | 5   |


# 🎼 Frecuencias musicales
```cpp id="8p4b5g"
const int notes[] = {262, 294, 330, 349};
```

## Explicación

Frecuencias de las notas musicales.
| Nota | Frecuencia |
| ---- | ---------- |
| DO   | 262 Hz     |
| RE   | 294 Hz     |
| MI   | 330 Hz     |
| FA   | 349 Hz     |

# 🖥️ Nombres mostrados en pantalla
```cpp id="vx11mt"
const String noteNames[] = {"DO", "RE", "MI", "FA"};
```
Se usan para mostrar el nombre de la nota en la LCD.

# 🔢 Número de botones
```cpp id="vzjwye"
const int numButtons = 4;
```
Cantidad total de botones.

# 🎮 Configuración del juego
```cpp id="v36r5w"
const int MAX_PASOS = 50;
```
Máximo tamaño de la secuencia.

```cpp id="1cx8jh"
int secuencia[MAX_PASOS];
```
Arreglo donde se guarda toda la secuencia del juego.

Ejemplo:
```text id="w4hnyv"
[2,0,1,3,2]
```

```cpp id="0kmb6q"
int nivel = 0;
```
Guarda el nivel actual del jugador.

# ⚡ Dificultad progresiva
```cpp id="k8jl7l"
int tiempoMuestraInicial = 500;
```
Tiempo inicial que dura cada nota.


```cpp id="94p0th"
int tiempoPausaInicial = 250;
```
Tiempo inicial entre notas.


```cpp id="t6d6g8"
int factorAceleracion = 25;
```
Cantidad de velocidad que aumenta por nivel.


```cpp id="w6pnsp"
int tiempoMuestraMinimo = 150;
```
Límite mínimo de duración de nota.


```cpp id="5dxn3l"
int tiempoPausaMinimo = 80;
```
Límite mínimo de pausa.
---
# 🖥️ Configuración de pantalla LCD

```cpp id="2d9pjr"
LiquidCrystal_I2C lcd(0x27, 16, 2);
```
## Parámetros

| Parámetro | Significado   |
| --------- | ------------- |
| 0x27      | Dirección I2C |
| 16        | Columnas      |
| 2         | Filas         |

# ⚙️ Función setup()
```cpp id="wffgfo"
void setup()
```
Se ejecuta una sola vez al iniciar Arduino.

# 🔈 Configurar buzzer
```cpp id="2mgpzr"
pinMode(speakerPin, OUTPUT);
```
Define el speaker como salida.


```cpp id="w5e7xy"
digitalWrite(speakerPin, LOW);
```
Apaga el buzzer inicialmente.

# 🔘 Configuración de botones
```cpp id="w27m4n"
for (int i = 0; i < numButtons; i++)
```
Recorre todos los botones.

```cpp id="5u6v9n"
pinMode(buttonPins[i], INPUT_PULLUP);
```
Configura los botones como entrada pull-up.

## Funcionamiento

| Estado        | Valor |
| ------------- | ----- |
| Sin presionar | HIGH  |
| Presionado    | LOW   |

# 🖥️ Inicializar LCD

```cpp id="f63iha"
lcd.init();
lcd.backlight();
```
Inicializa y enciende la pantalla.

# 🎲 Aleatoriedad

```cpp id="3p8w0s"
randomSeed(analogRead(A0));

Genera números aleatorios usando ruido eléctrico.

# 🏁 Pantalla inicial

```cpp id="7yr3n0"
mostrarPantallaInicio();

Muestra la pantalla de bienvenida.

# 🔄 Función loop()

```cpp id="nuy70f"
void loop()
```
Se ejecuta infinitamente.

# 🎲 Añadir nueva nota aleatoria
```cpp id="x6l0tf"
secuencia[nivel] = random(0, numButtons);
```
Genera un número entre 0 y 3.

Cada número representa un botón.
```cpp id="pjcvz5"
nivel++;
```
Aumenta el nivel.

# ⚡ Velocidad dinámica

```cpp id="0owbql"
int tiempoMuestraActual = tiempoMuestraInicial - (nivel * factorAceleracion);
```
Reduce el tiempo de las notas conforme aumenta el nivel.

```cpp id="vw7mhl"
int tiempoPausaActual = tiempoPausaInicial - (nivel * (factorAceleracion / 2));
```
Reduce las pausas.

# 🚫 Limitar velocidad mínima
```cpp id="l6zhl8"
if (tiempoMuestraActual < tiempoMuestraMinimo)
```
Evita que el juego sea imposible.

# 🖥️ Mostrar nivel

```cpp id="m6k1xk"
lcd.print("SIMON DICE:");
```
Título del juego.

```cpp id="5s6fc8"
lcd.print("Nivel: ");
lcd.print(nivel);
```
Muestra el nivel actual.

# 🎵 Mostrar secuencia
```cpp id="7muhf8"
for (int i = 0; i < nivel; i++)
```
Recorre toda la secuencia.

# Nota actual
```cpp id="m0uc2w"
int botonActual = secuencia[i];
```
Obtiene la nota actual.

# Mostrar nota
```cpp id="4lk02n"
lcd.print(noteNames[botonActual]);
```
Muestra:

* DO
* RE
* MI
* FA

# 🔊 Reproducir sonido
```cpp id="cb2ih4"
tone(speakerPin, notes[botonActual]);
```
Genera la frecuencia musical.

# Esperar duración
```cpp id="4rl58t"
delay(tiempoMuestraActual);
```
Duración dinámica de la nota.

# Apagar sonido

```cpp id="k0gn5z"
noTone(speakerPin);
```
Detiene el buzzer.

# ⏳ Pausa entre notas

```cpp id="7ysw4e"
delay(tiempoPausaActual);
```
Pausa dinámica.

# 👤 Turno del jugador

```cpp id="6qx45g"
lcd.print("TU TURNO...");
```

Indica que debe repetir.

# Esperar botón

```cpp id="f6x3tt"
while (botonPresionado == -1)
```
Espera hasta que el jugador presione.


# Detectar botón

```cpp id="cg1e4r"
if (digitalRead(buttonPins[b]) == LOW)
```

Detecta pulsación.

# Guardar botón

```cpp id="4r7ndn"
botonPresionado = b;
```
Guarda cuál botón fue presionado.

# Sonido del jugador

```cpp id="gmq1t5"
tone(speakerPin, notes[b]);
```
Reproduce la nota.

# Mostrar botón presionado
```cpp id="c44i44"
lcd.print(noteNames[b]);
```
Muestra la nota tocada.

# Esperar soltar botón
```cpp id="8c7f0f"
while (digitalRead(buttonPins[b]) == LOW)
```
Evita múltiples lecturas.


# ❌ Verificar error

```cpp id="y5x2f2"
if (botonPresionado != botonCorrecto)
```
Si la secuencia no coincide:

➡️ GAME OVER

# ✅ Victoria parcial

```cpp id="lqk65s"
lcd.print("¡CORRECTO!");
```
Mensaje de éxito.

# 🎶 Sonido de victoria

```cpp id="nx85a1"
tone(speakerPin, 523);
tone(speakerPin, 659);
tone(speakerPin, 784);
```
Melodía ascendente de victoria.

# 🏁 Función mostrarPantallaInicio()

```cpp id="v11gk6"
void mostrarPantallaInicio()
```

Pantalla inicial del juego.

# Esperar inicio
```cpp id="xjll4l"
while (!empezar)
```
Espera hasta que el usuario presione un botón.

# Sonido de inicio
```cpp id="xg5x5o"
tone(speakerPin, 440);
tone(speakerPin, 554);
tone(speakerPin, 659);
```
Melodía de arranque.

# Reiniciar nivel
```cpp id="jru4q1"
nivel = 0;
```
Empieza desde nivel 0.

# 💀 Función Game Over

```cpp id="qybv4m"
void ejecutarGameOver()
```

Pantalla de derrota.

---

# Mostrar error

```cpp id="t6l52w"
lcd.print("¡ERROR!");
```
Mensaje principal.

# Puntaje final
```cpp id="8l0b3u"
lcd.print(nivel - 1);
```
Muestra el nivel alcanzado.



# 🎵 Sonido triste

```cpp id="j5ucg6"
tone(speakerPin, 300);
tone(speakerPin, 200);
tone(speakerPin, 130);
```
Melodía descendente de derrota.


# 🔁 Reinicio automático
```cpp id="nl08fh"
mostrarPantallaInicio();
```
Vuelve al menú inicial.




