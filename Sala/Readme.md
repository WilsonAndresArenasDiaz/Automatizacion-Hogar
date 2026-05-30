# 🎹 Piano Arduino con LCD I2C y TIP41C

# 📌 Descripción del proyecto

Este proyecto implementa un piano electrónico usando:

* Arduino UNO
* 7 botones musicales
* Pantalla LCD I2C 16x2
* Transistor TIP41C
* Speaker/Buzzer
* Notas musicales reales

Cada botón reproduce una nota musical distinta y la pantalla muestra:

✅ Nombre de la nota
✅ Frecuencia en Hz
✅ Estado del piano

# 🧰 Librerías utilizadas
```cpp id="zy0m5v"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
```

# 📚 Explicación

## `#include <Wire.h>`

Activa la comunicación I2C del Arduino.

La pantalla LCD I2C usa únicamente:

* SDA
* SCL

para comunicarse.


## `#include <LiquidCrystal_I2C.h>`

Carga la librería para controlar la pantalla LCD mediante I2C.


# 🔈 Pin de salida de audio
```cpp id="2l7v81"
const int speakerPin = 9;
```

## Explicación

Pin que envía la señal de audio.

Este pin va conectado al:

* TIP41C
* Speaker


# 🎹 Configuración de botones
```cpp id="uzcm7q"
const int buttonPins[] = {2, 3, 4, 5, 6, 7, 8};
```

## Explicación

Arreglo que almacena los pines de cada tecla del piano.

| Nota | Pin |
| ---- | --- |
| DO   | 2   |
| RE   | 3   |
| MI   | 4   |
| FA   | 5   |
| SOL  | 6   |
| LA   | 7   |
| SI   | 8   |

# 🎼 Frecuencias musicales
```cpp id="5s4h4r"
const int notes[] = {262, 294, 330, 349, 392, 440, 494};
```

## Explicación

Frecuencia de cada nota musical en Hertz.

| Nota | Frecuencia |
| ---- | ---------- |
| DO   | 262 Hz     |
| RE   | 294 Hz     |
| MI   | 330 Hz     |
| FA   | 349 Hz     |
| SOL  | 392 Hz     |
| LA   | 440 Hz     |
| SI   | 494 Hz     |


# 🖥️ Nombres musicales
```cpp id="22n7tx"
const String noteNames[] = {"DO", "RE", "MI", "FA", "SOL", "LA", "SI"};
```

## Explicación

Se usan para mostrar el nombre de la nota en la LCD.


# 🔢 Número de botones
```cpp id="u7p5m1"
const int numButtons = 7;
```
Cantidad total de teclas del piano.

# 🖥️ Configuración LCD
```cpp id="b7wp0n"
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

## Parámetros

| Parámetro | Significado        |
| --------- | ------------------ |
| 0x27      | Dirección I2C      |
| 16        | Número de columnas |
| 2         | Número de filas    |

# 💾 Última nota reproducida
```cpp id="9cf15r"
int lastNotePlayed = -1;
```

## Explicación

Guarda la última nota tocada.

Sirve para:

✅ Evitar parpadeos
✅ Evitar limpiar la pantalla innecesariamente

# ⚙️ Función setup()
```cpp id="eqaymp"
void setup()
```
Se ejecuta una sola vez al iniciar Arduino.

# 🔈 Configurar speaker

```cpp id="rzepvz"
pinMode(speakerPin, OUTPUT);
```
Define el pin del speaker como salida.

# 🔘 Configuración de botones
```cpp id="6byk8u"
for (int i = 0; i < numButtons; i++)
```
Recorre todos los botones.

```cpp id="kcvz11"
pinMode(buttonPins[i], INPUT_PULLUP);
```
Configura cada botón como entrada pull-up.

# 📚 ¿Qué es INPUT_PULLUP?

Arduino activa una resistencia interna.

## Funcionamiento

| Estado        | Valor |
| ------------- | ----- |
| Sin presionar | HIGH  |
| Presionado    | LOW   |


# 🖥️ Inicializar LCD

```cpp id="fjlwm1"
lcd.init();
lcd.backlight();
```
Inicializa y enciende la pantalla LCD.

# 🏁 Pantalla inicial
```cpp id="sl2v3s"
mostrarPantallaInicio();
```
Muestra mensaje de bienvenida.

# 🔄 Función loop()
```cpp id="jpvslj"
void loop()
```
Se ejecuta continuamente.

# 🔍 Variables de control
```cpp id="5vyqqk"
bool buttonPressed = false;
```
Indica si algún botón fue presionado.

```cpp id="f0qkqk"
int currentNoteIndex = -1;
```
Guarda la nota actual.

# 🔘 Revisar botones
```cpp id="3blwvh"
for (int i = 0; i < numButtons; i++)
```
Recorre todos los botones.

# Detectar pulsación
```cpp id="evg1ng"
if (digitalRead(buttonPins[i]) == LOW)
```
Detecta cuando un botón es presionado.

# Guardar nota actual
```cpp id="3dyukn"
currentNoteIndex = i;
```
Guarda cuál nota corresponde.

# Activar bandera
```cpp id="b8sgkl"
buttonPressed = true;
```
Indica que sí hubo pulsación.

# Salir del bucle
```cpp id="9l8u7i"
break;
```
Detiene la búsqueda porque ya encontró una tecla presionada.

# 🎵 Reproducir sonido
```cpp id="3n2ul4"
tone(speakerPin, notes[currentNoteIndex]);
```
Genera la frecuencia musical.

# 📺 Actualizar LCD solo si cambia la nota
```cpp id="70dskw"
if (currentNoteIndex != lastNotePlayed)
```
Evita refrescar innecesariamente la pantalla.

Esto elimina:

❌ Parpadeos
❌ Reescritura constante

# Limpiar pantalla
```cpp id="zvmg3x"
lcd.clear();
```
Borra el contenido anterior.

# Mostrar nota
```cpp id="f4j3ol"
lcd.print("Nota: " + noteNames[currentNoteIndex]);
```
Muestra el nombre musical.

Ejemplo:
```text id="icnd9k"
Nota: DO
```

# Mostrar frecuencia
```cpp id="yg2xvq"
lcd.print("Freq: " + String(notes[currentNoteIndex]) + " Hz");
```
Muestra frecuencia.

Ejemplo:
```text id="kdbec2"
Freq: 262 Hz
```

# Guardar última nota
```cpp id="x5l68k"
lastNotePlayed = currentNoteIndex;
```
Actualiza la última nota reproducida.

# 🔇 Si no hay botones presionados

```cpp id="yp0lma"
else
```
Cuando nadie toca el piano.

# Detener sonido
```cpp id="zk4lo9"
noTone(speakerPin);
```
Apaga el speaker.

# Restaurar pantalla inicial
```cpp id="3pqkdf"
if (lastNotePlayed != -1)
```
Solo actualiza una vez.

```cpp id="r3s0cc"
mostrarPantallaInicio();
```
Vuelve al menú principal.

# Reiniciar nota
```cpp id="yc4j0z"
lastNotePlayed = -1;
```
Indica que ya no hay nota activa.

# ⏱️ Delay pequeño
```cpp id="x8zv6o"
delay(10);
```
Reduce rebotes eléctricos.

# 🖥️ Función mostrarPantallaInicio()
```cpp id="8t2ncm"
void mostrarPantallaInicio()
```
Pantalla de espera.

# Limpiar LCD
```cpp id="v8l4xv"
lcd.clear();
```
Borra contenido anterior.

# Texto principal
```cpp id="f02r1f"
lcd.print("  PIANO ARDUINO ");
```
Título del proyecto.

# Texto secundario
```cpp id="vgq0ow"
lcd.print("Presiona una tec");
```
Indica al usuario tocar una tecla.

# 🔌 Conexiones LCD I2C

| LCD | Arduino UNO |
| --- | ----------- |
| VCC | 5V          |
| GND | GND         |
| SDA | A4          |
| SCL | A5          |

# 🔘 Conexión de botones

| Nota | Pin Arduino |
| ---- | ----------- |
| DO   | 2           |
| RE   | 3           |
| MI   | 4           |
| FA   | 5           |
| SOL  | 6           |
| LA   | 7           |
| SI   | 8           |

Cada botón:

* Un lado al pin
* Otro lado a GND

---

# 🔈 Conexión del TIP41C

## 📌 Configuración típica

| TIP41C   | Conexión                   |
| -------- | -------------------------- |
| Base     | Pin 9 mediante resistencia |
| Colector | Speaker                    |
| Emisor   | GND                        |

# 🔊 Speaker

| Speaker  | Conexión        |
| -------- | --------------- |
| Positivo | 5V              |
| Negativo | Colector TIP41C |


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

# 😀 Animación de Caritas en LCD I2C 16x2 con Arduino para simulacion de tv

# 📌 Descripción del proyecto

Este proyecto utiliza una pantalla LCD I2C 16x2 para mostrar una animación de cuatro caritas creadas mediante caracteres personalizados.

Las caritas:

😀 Feliz
😮 Sorprendida
☹️ Triste
😉 Guiñando

se desplazan juntas desde el extremo derecho de la pantalla hacia el extremo izquierdo.

# 🛠 Materiales utilizados

* Arduino UNO
* Pantalla LCD I2C 16x2
* Módulo I2C (dirección 0x27)
* Cables Dupont
* Protoboard (opcional)

# 📚 Librerías utilizadas
```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
```
## Explicación

### Wire.h

Permite la comunicación I2C entre Arduino y la pantalla LCD.

Utiliza únicamente:

* SDA
* SCL

### LiquidCrystal_I2C.h

Permite controlar fácilmente una pantalla LCD equipada con módulo I2C.

# 🖥 Configuración de la pantalla

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

## Parámetros

| Parámetro | Significado   |
| --------- | ------------- |
| 0x27      | Dirección I2C |
| 16        | Columnas      |
| 2         | Filas         |

# 😀 Carácter 1: Carita Feliz
```cpp
byte cara1[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
```

## Representación gráfica
```text
     
 O O
 O O

O   O
 OOO
```

# 😮 Carácter 2: Carita Sorprendida
```cpp
byte cara2[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B01110,
  B10001,
  B01110,
  B00000
};
```
Representa una expresión de sorpresa.

# ☹️ Carácter 3: Carita Triste

```cpp
byte cara3[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B01110,
  B10001,
  B00000,
  B11111
};
```
Representa una expresión triste.

# 😉 Carácter 4: Carita Guiñando

```cpp
byte cara4[8] = {
  B00000,
  B00010,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
```
Representa una carita guiñando un ojo.

# ⚙️ Función setup()
```cpp
void setup()
```
Se ejecuta una sola vez cuando Arduino inicia.

## Inicializar LCD
```cpp
lcd.init();
```
Inicializa la comunicación con la pantalla.

## Encender retroiluminación
```cpp
lcd.backlight();
```
Activa la luz de fondo.

## Registrar caracteres personalizados
```cpp
lcd.createChar(0, cara1);
lcd.createChar(1, cara2);
lcd.createChar(2, cara3);
lcd.createChar(3, cara4);
```
La memoria CGRAM de la LCD almacena hasta 8 caracteres personalizados.

| Número | Carácter    |
| ------ | ----------- |
| 0      | Feliz       |
| 1      | Sorprendida |
| 2      | Triste      |
| 3      | Guiñando    |

# Mostrar título
```cpp
lcd.setCursor(4, 0);
lcd.print("TV IoT");
```

## Resultado
```text
    TV IoT
```

Se imprime en la primera fila.

# 🔄 Función loop()
```cpp
void loop()
```

Se ejecuta continuamente.

# Movimiento de las caritas
```cpp
for (int pos = 15; pos >= -4; pos--)
```

## Explicación
La variable:
```cpp
pos
```
controla la posición horizontal.

Comienza en:
```cpp
15
```

que corresponde a la última columna de la LCD.

Y termina en:
```cpp
-4
```
para permitir que todas las caritas salgan completamente de la pantalla.

# Limpiar segunda fila

```cpp
lcd.setCursor(0, 1);
lcd.print("                ");
```

Se escriben 16 espacios.

Resultado:
```text
[                ]
```
La fila queda vacía antes de dibujar nuevamente.

# Dibujar carita feliz
```cpp
if (pos >= 0 && pos < 16)
```
Verifica si la posición está dentro de la pantalla.

```cpp
lcd.setCursor(pos, 1);
lcd.write(byte(0));
```
Dibuja la carita feliz.

# Dibujar carita sorprendida
```cpp
if (pos + 1 >= 0 && pos + 1 < 16)
```
Ubica la segunda carita una columna después.

```cpp
lcd.write(byte(1));
```
Muestra la carita sorprendida.

# Dibujar carita triste
```cpp
if (pos + 2 >= 0 && pos + 2 < 16)
```
Desplaza dos columnas.

```cpp
lcd.write(byte(2));
```
Muestra la carita triste.

# Dibujar carita guiñando
```cpp
if (pos + 3 >= 0 && pos + 3 < 16)
```
Desplaza tres columnas.

```cpp
lcd.write(byte(3));
```
Muestra la carita guiñando.

# Ejemplo visual del movimiento

### Inicio
```text
TV IoT

            😀😮☹😉
```

### Movimiento intermedio
```text
TV IoT

      😀😮☹😉
```

### Final
```text
TV IoT

😀😮☹😉
```

### Saliendo de pantalla
```text
TV IoT

😮☹😉
```

Luego:
```text
TV IoT

☹😉
```

Luego:
```text
TV IoT

😉
```
Finalmente desaparecen.

# Control de velocidad
```cpp
delay(200);
```
La animación espera:

200 ms

entre cada desplazamiento.

