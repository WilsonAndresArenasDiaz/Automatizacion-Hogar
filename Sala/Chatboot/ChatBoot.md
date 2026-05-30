# 🎤🧠 Asistente de Voz con Whisper + Ollama + Arduino

# 📌 Descripción General

Este proyecto implementa un asistente de voz inteligente capaz de:

1. Escuchar la voz del usuario mediante un micrófono.
2. Convertir el audio en texto usando Whisper.
3. Analizar la intención mediante Gemma 2B ejecutándose localmente en Ollama.
4. Clasificar el comando.
5. Enviar órdenes a Arduino mediante comunicación Serial USB.
6. Controlar diferentes aplicaciones y juegos embebidos.

# 🏗 Arquitectura del Sistema

```text
Usuario
   │
   ▼
Micrófono
   │
   ▼
SpeechRecognition
   │
   ▼
Archivo WAV
   │
   ▼
Whisper Tiny
   │
   ▼
Texto Transcrito
   │
   ▼
Gemma 2B (Ollama)
   │
   ▼
Clasificación
   │
   ▼
Puerto Serial USB
   │
   ▼
Arduino
   │
   ├── Piano
   ├── Consola
   ├── Dino
   └── Simon Dice
```

# 📚 Librerías utilizadas
```python
import whisper
import speech_recognition as sr
import requests
import os
import serial
import time
import json
```

# 🎤 whisper
```python
import whisper
```

Permite convertir voz a texto.

Modelo utilizado:
```python
model_stt = whisper.load_model("tiny")
```

Ventajas:

* Muy rápido
* Funciona sin internet
* Consume pocos recursos

# 🎙 speech_recognition

```python
import speech_recognition as sr
```
Captura audio desde el micrófono.

Funciones principales:

* Escuchar usuario
* Ajustar ruido ambiente
* Guardar audio

# 🌐 requests
```python
import requests
```
Permite enviar solicitudes HTTP a Ollama.

Se utiliza para comunicarse con:
```text
http://localhost:11434
```

# 📁 os
```python
import os
```
Gestiona archivos temporales.

Ejemplo:
```python
os.remove("temp_audio.wav")
```

# 🔌 serial
```python
import serial
```
Permite enviar comandos al Arduino mediante USB.

# ⏱ time
```python
import time
```
Controla retardos y sincronización.

# 📦 json

```python
import json
```
Procesa respuestas de Ollama.

# 🔌 Conexión con Arduino
```python
arduino = serial.Serial('COM8',9600,timeout=1)
```

## Parámetros

| Parámetro | Función          |
| --------- | ---------------- |
| COM8      | Puerto USB       |
| 9600      | Velocidad Serial |
| timeout=1 | Espera máxima    |

# Esperar Inicialización
```python
time.sleep(2)
```
Da tiempo al Arduino para reiniciarse.


# Modo Simulación
```python
except Exception as e:
```
Si Arduino no existe:

```python
arduino = None
```
El sistema sigue funcionando.

# Cargar Whisper
```python
model_stt = whisper.load_model("tiny")
```
Modelos disponibles:

| Modelo | Velocidad   |
| ------ | ----------- |
| tiny   | Muy rápida  |
| base   | Rápida      |
| small  | Media       |
| medium | Lenta       |
| large  | Muy precisa |

# Crear Reconocedor
```python
recognizer = sr.Recognizer()
```
Objeto principal para capturar voz.

# Función IA

```python
def consultar_ollama_gemma(texto_usuario):
```

Encargada de enviar texto a Gemma.
# URL de Ollama
```python
url="http://localhost:11434/api/generate"
```
API REST local.

# Prompt del Sistema
```python
prompt_sistema = (...)
```
Es el cerebro del clasificador.

Define las reglas.

# Comandos Reconocidos

## Piano

Ejemplos:
```text
quiero el piano
abre piano
uno piano
```

Respuesta:
```text
[PIANO]
```

## Consola

Ejemplos:
```text
abre consola
consola
menu consola
```

Respuesta:
```text
[CONSOLA]
```

## Dinosaurio

Ejemplos:
```text
dinosaurio
jugar dino
juego 1
```

Respuesta:
```text
[DINO]
```

## Simón Dice

Ejemplos:
```text
simon dice
juego 2
```
Respuesta:
```text
[SIMON]
```

## Menú

Ejemplos:
```text
volver
salir
cancelar
```

Respuesta:
```text
[MENU]
```

# Construcción del Prompt
```python
payload = {
    "model":"gemma:2b",
    "prompt":...
}
``
Información enviada al modelo.

# Desactivar Streaming
```python
"stream": False
```
La respuesta llega completa.

# Enviar Solicitud
```python
requests.post(url,json=payload)
```
Consulta a Gemma.

# Limpiar Respuesta
```python
if "[PIANO]" in resultado_texto
```
Verifica etiquetas válidas.

# Inicio del Sistema
```python
print("CHATBOT DE VOZ LISTO")
```
Muestra mensaje de arranque.

# Archivo Temporal
```python
ARCHIVO_AUDIO="temp_audio.wav"
```
Archivo donde se guarda la grabación.

# Bucle Principal
```python
while True:
```
Mantiene el sistema activo.

# Esperar Usuario
```python
input("Presiona ENTER...")
```
Inicia una nueva captura.

# Captura de Audio
```python
with sr.Microphone() as source:
```
Accede al micrófono.

# Ajuste de Ruido
```python
recognizer.adjust_for_ambient_noise(source,duration=0.5)
```
Filtra ruido ambiente.

# Escuchar
```python
audio = recognizer.listen(source)
```
Graba la voz.

# Guardar WAV
```python
audio.get_wav_data()
```

Convierte audio a formato WAV.

# Transcripción
```python
resultado = model_stt.transcribe(...)
```

Whisper convierte:
```text
Audio
```
en
```text
Texto
```

# Mostrar Resultado
```python
print(texto_dicho)
```

Ejemplo:
```text
Quiero jugar dinosaurio
```

# Eliminar Archivo
```python
os.remove(ARCHIVO_AUDIO)
```
Libera espacio.

# Analizar con IA
```python
accion = consultar_ollama_gemma(texto_dicho)
```
Gemma determina intención.

# Resultado IA

Ejemplo:
```text
[DINO]
```

# Envío al Arduino

## Piano
```python
arduino.write(b"PIANO\n")
```

Arduino recibe:
```text
PIANO
```

## Consola
```python
arduino.write(b"CONSOLA\n")
```

## Dinosaurio
```python
arduino.write(b"DINO\n")
```

## Simón Dice
```python
arduino.write(b"SIMON\n")
```

## Menú
```python
arduino.write(b"MENU\n")
```

# Flujo Completo
```text
Usuario habla
      │
      ▼
Micrófono
      │
      ▼
Whisper
      │
      ▼
Texto
      │
      ▼
Gemma 2B
      │
      ▼
Clasificación
      │
      ▼
Puerto Serial
      │
      ▼
Arduino
      │
      ├── Piano
      ├── Dino
      ├── Simon
      └── Menu
```

# Requisitos

## Python
```bash
pip install openai-whisper
pip install SpeechRecognition
pip install requests
pip install pyserial
pip install pyaudio
```

# Ollama

Instalar:
```bash
ollama pull gemma:2b
```
Ejecutar:
```bash
ollama run gemma:2b
```

# Arduino

Configurar:
```cpp
Serial.begin(9600);
```

y leer comandos mediante:
```cpp
Serial.readStringUntil('\n');
```

# Tecnologías Utilizadas

✅ Python

✅ Arduino

✅ Whisper

✅ Ollama

✅ Gemma 2B

✅ Speech Recognition

✅ Comunicación Serial

✅ Inteligencia Artificial Local

# 🎮🎤 PlayStation Arduino Inteligente con IA Local dashboard

# 📌 Descripción General

Este proyecto desarrolla una estación de juegos controlada por voz mediante Inteligencia Artificial Local.

El sistema permite:

✅ Escuchar comandos de voz

✅ Analizar órdenes usando Gemma 2B ejecutándose en Ollama

✅ Controlar juegos en Arduino

✅ Mostrar información en tiempo real mediante una interfaz gráfica

✅ Gestionar la comunicación Serial USB

# 🏗 Arquitectura del Sistema

```text
┌──────────────────────┐
│      Usuario         │
└──────────┬───────────┘
           │ Voz
           ▼
┌──────────────────────┐
│     Micrófono        │
└──────────┬───────────┘
           ▼
┌──────────────────────┐
│ Speech Recognition   │
└──────────┬───────────┘
           ▼
┌──────────────────────┐
│      Gemma 2B        │
│     (Ollama IA)      │
└──────────┬───────────┘
           ▼
┌──────────────────────┐
│ Clasificación IA     │
└──────────┬───────────┘
           ▼
┌──────────────────────┐
│ Comunicación Serial  │
└──────────┬───────────┘
           ▼
┌──────────────────────┐
│      Arduino         │
├──────────────────────┤
│ Piano                │
│ Consola              │
│ Dino                 │
│ Simon Dice           │
└──────────────────────┘
```

# 📚 Librerías Utilizadas
```python
import tkinter as tk
from tkinter import ttk
import speech_recognition as sr
import requests
import serial
import threading
import time
```

# 🖥 Tkinter
```python
import tkinter as tk
```
Permite crear la interfaz gráfica.

Funciones principales:

* Ventanas
* Botones
* Etiquetas
* Dashboard

# 🎨 ttk
```python
from tkinter import ttk
```
Componentes visuales modernos para Tkinter.

Se usa para:
```python
ttk.Button()
```

# 🎤 Speech Recognition
```python
import speech_recognition as sr
```

Permite:

* Capturar audio
* Detectar voz
* Convertir voz a texto

# 🌐 Requests
```python
import requests
```

Comunicación con Ollama.
Ejemplo:
```python
requests.post()
```

# 🔌 PySerial
```python
import serial
```
Comunicación USB con Arduino.

# 🧵 Threading
```python
import threading
```
Permite escuchar audio sin congelar la ventana gráfica.

# ⏱ Time
```python
import time
```
Controla retardos y sincronización.

# 🔌 Configuración Serial
```python
PUERTO_COM = "COM10"
```
Puerto donde está conectado Arduino.

# Conexión
```python
arduino = serial.Serial(
    PUERTO_COM,
    9600,
    timeout=0.1
)
```

## Parámetros

| Parámetro   | Función          |
| ----------- | ---------------- |
| COM10       | Puerto USB       |
| 9600        | Baudrate         |
| timeout=0.1 | Tiempo de espera |

# Espera Inicial
```python
time.sleep(2)
```
Da tiempo al Arduino para reiniciarse.

# Modo Simulación

Si Arduino no existe:
```python
arduino = None
```
La aplicación sigue funcionando.

# Reconocedor de Voz
```python
recognizer = sr.Recognizer()
```
Objeto principal para escuchar al usuario.

# Dashboard
```python
datos_dashboard = {
...
}
```
Almacena información mostrada en pantalla.

# Variables del Dashboard

## Modo Actual
```python
"modo_actual"
```

Ejemplos:
```text
Piano
Dino
Simón
Consola
```

## Último Comando
```python
"ultimo_comando"
```
Última frase pronunciada.

## Estado Ollama
```python
"estado_ollama"
```
Estado de la IA.

## Estado USB
```python
"tiempo_conexion"
```

Muestra:
```text
Conectado COM10
```

o
```text
Simulado
```

# 🧠 Función IA
```python
def consultar_ollama_gemma()
```
Analiza comandos de voz.

# URL Ollama
```python
url="http://localhost:11434/api/generate"
```
Servidor local de Ollama.

# Prompt Inteligente
```python
prompt_sistema
```
Define el comportamiento de Gemma.

# Clasificaciones

## Piano

Frases:
```text
abre piano
quiero el piano
piano
```

Respuesta:
```text
[PIANO]
```

## Consola
```text
consola
abre consola
```

Respuesta:
```text
[CONSOLA]
```

## Dino
```text
jugar dinosaurio
dino
```

Respuesta:
```text
[DINO]
```

## Simon
```text
simón dice
juego simón
```

Respuesta:
```text
[SIMON]
```

## Menú
```text
volver
salir
cancelar
```

Respuesta:
```text
[MENU]
```

# Payload enviado a Gemma
```python
payload = {
   "model":"gemma:2b",
   ...
}
```
Indica:

* Modelo
* Prompt
* Configuración

# Procesamiento de Voz
```python
def escuchar_voz_hilo()
```
Funciona en segundo plano.

# Estado Escuchando
```python
lbl_estado_voz.config(...)
```
Actualiza la interfaz.

# Activar Micrófono
```python
with sr.Microphone()
```
Acceso al micrófono.

# Ajustar Ruido
```python
adjust_for_ambient_noise()
```
Filtra ruido ambiental.

# Escuchar
```python
listen()
```
Captura la voz.

# Convertir a Texto
```python
recognize_google()
```
Realiza la transcripción.

# Ejemplo
Usuario:
```text
Quiero jugar dinosaurio
```

Resultado:
```text
Quiero jugar dinosaurio
```

# Consultar IA
```python
accion, modo_nombre =
consultar_ollama_gemma(...)
```
Gemma interpreta la intención.

# Resultado
```text
[DINO]
```

# Actualización Dashboard

```python
datos_dashboard["modo_actual"]
```
Actualiza los datos visuales.

# Envío al Arduino
```python
comando_limpio =
accion.replace(...)
```

Convierte:
```text
[DINO]
```

en
```text
DINO
```

# Agregar Salto de Línea
```python
mensaje_final =
comando_limpio + "\n"
```

Arduino espera:
```text
DINO\n
```

# Codificación ASCII
```python
encode('ascii')
```
Convierte texto a bytes.

# Transmisión
```python
arduino.write(...)
```
Envía el comando.

# Flush
```python
arduino.flush()
```
Fuerza envío inmediato.

# Hilo Independiente
```python
threading.Thread(
    target=escuchar_voz_hilo,
    daemon=True
)
```
Evita congelar la ventana.

# Dashboard Visual
```python
actualizar_dashboard_visual()
```

Actualiza:

* Juego activo
* Último comando
* Estado IA
* Estado USB

# Interfaz Principal
```python
root = tk.Tk()
```
Ventana principal.

# Configuración
```python
root.title(...)
root.geometry(...)
```

Define:

* Título
* Tamaño

# Navegación

Dos pantallas:

## Asistente

```text
🎙 Hablar
```

## Dashboard
```text
📊 Información
```

# Pantalla de Voz

Contiene:
```text
Título
Botón Hablar
Estado
Transcripción
```

# Dashboard

Muestra:
```text
Modo Actual
Último Comando
Estado IA
Estado USB
```

# Flujo Completo

```text
Usuario habla
       │
       ▼
Micrófono
       │
       ▼
Speech Recognition
       │
       ▼
Texto
       │
       ▼
Gemma 2B
       │
       ▼
Clasificación
       │
       ▼
Dashboard
       │
       ▼
Serial USB
       │
       ▼
Arduino
       │
       ├── Piano
       ├── Consola
       ├── Dino
       └── Simon
```

# Requisitos

Instalar dependencias:
```bash
pip install pyserial
pip install requests
pip install SpeechRecognition
pip install pyaudio
```

# Ollama

Descargar modelo:
```bash
ollama pull gemma:2b
```

Ejecutar:
```bash
ollama run gemma:2b
```

# Arduino

Debe tener:
```cpp
Serial.begin(9600);
```

y recibir:
```cpp
Serial.readStringUntil('\n');
```

# Tecnologías Empleadas

✅ Python

✅ Tkinter

✅ Speech Recognition

✅ Ollama

✅ Gemma 2B

✅ PySerial

✅ Inteligencia Artificial Local

✅ Arduino

✅ Dashboard en Tiempo Real
