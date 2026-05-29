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


