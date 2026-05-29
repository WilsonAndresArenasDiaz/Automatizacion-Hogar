# Laboratorio Final: Casa con IoT



<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135303.png" alt="Captura 1" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135342.png" alt="Captura 2" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135431.png" alt="Captura 3" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135542.png" alt="Captura 4" width="900" />
</p>
# 🚽 Baño Inteligente con ESP32

Sistema de automatización para baño inteligente usando **ESP32**, **LED RGB**, **pantalla OLED** y comunicación vía **WiFi + API REST**.  
El proyecto permite cambiar los modos del baño mediante comandos de voz o peticiones HTTP.

---

# 📌 Descripción del Proyecto

Este laboratorio implementa un sistema de iluminación inteligente controlado por un **ESP32**.

El usuario puede seleccionar distintos modos ambientales:

- 🌅 **Modo Mañana** → Luz Amarilla
- 🧖 **Modo Spa** → Luz Roja
- 🌙 **Modo Noche** → Luz Verde

Además:

- Se muestra el estado actual en una pantalla **OLED**
- El ESP32 expone una API REST
- Los cambios pueden hacerse mediante voz o desde navegador/web

---

# 🧰 Componentes Utilizados

| Componente | Descripción |
|---|---|
| ESP32 | Microcontrolador principal |
| Pantalla OLED SSD1306 | Visualización de estados |
| LEDs RGB | Indicadores de modo |
| Resistencias | Protección LEDs |
| WiFi | Comunicación inalámbrica |
| API REST | Control remoto |

---

# ⚡ Conexiones

## 🔌 LEDs

| LED | GPIO ESP32 |
|---|---|
| Rojo | GPIO 25 |
| Verde | GPIO 26 |
| Amarillo | GPIO 27 |

---

## 🖥 OLED SSD1306

| OLED | ESP32 |
|---|---|
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| VCC | 3.3V |
| GND | GND |

---

# 📚 Librerías Utilizadas

Instalar desde el **Library Manager** de Arduino IDE:

```cpp
WiFi.h
WebServer.h
Wire.h
Adafruit_GFX.h
Adafruit_SSD1306.h
```

---

# 🌐 Funcionamiento

El ESP32:

1. Se conecta a la red WiFi
2. Inicia un servidor web en el puerto 80
3. Espera comandos HTTP
4. Interpreta el modo solicitado
5. Activa LEDs
6. Muestra información en la OLED

---

# 🧠 Modos del Sistema

| Comando | Acción | LED |
|---|---|---|
| spa | Ambiente relajante | 🔴 Rojo |
| mañana / manana | Ambiente día | 🟡 Amarillo |
| noche | Ambiente nocturno | 🟢 Verde |

---

# 🧠 Lógica Inteligente

El sistema interpreta palabras clave usando:

```cpp
entrada.indexOf("spa")
entrada.indexOf("mañana")
entrada.indexOf("noche")
```

Esto permite integrar asistentes de voz fácilmente.

---

# 🔥 Código Principal

```
#include <WiFi.h>
#include <WebServer.h>
 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
// ==========================
// ✅ OLED
// ==========================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
// ==========================
// 📶 WIFI
// ==========================
const char* ssid = "CASA69";
const char* password = "Casa692025";
 
// ==========================
// 🔌 PINES LED
// ==========================
#define LED_ROJO 25
#define LED_VERDE 26
#define LED_AMARILLO 27  // reemplaza azul
 
WebServer server(80);
 
// ==========================
// ✅ FUNCIÓN OLED
// ==========================
void mostrarOLED(String texto) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(texto);
  display.display();
}
 
// ==========================
// 🔌 CONTROL HARDWARE
// ==========================
void apagarTodo() {
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARILLO, LOW);
}
 
void activarModo(String modo) {
 
  apagarTodo();
 
  if (modo == "spa") {
    digitalWrite(LED_ROJO, HIGH);
    mostrarOLED("SPA");   // ✅ agregado
  }
  else if (modo == "manana") {
    digitalWrite(LED_AMARILLO, HIGH);
    mostrarOLED("DIA");   // ✅ agregado
  }
  else if (modo == "noche") {
    digitalWrite(LED_VERDE, HIGH);
    mostrarOLED("NOCHE"); // ✅ agregado
  }
}
 
// ==========================
// 🧠 PROCESAMIENTO INTELIGENTE
// ==========================
String interpretarModo(String entrada) {
 
  entrada.trim();
  entrada.toLowerCase();
 
  if (entrada.indexOf("spa") >= 0) return "spa";
 
  if (entrada.indexOf("mañana") >= 0 || entrada.indexOf("manana") >= 0)
    return "manana";
 
  if (entrada.indexOf("noche") >= 0)
    return "noche";
 
  return "none";
}
 
// ==========================
// 🌐 API WEB
// ==========================
void handleModo() {
 
  String comando = server.arg("modo");
 
  Serial.println("Comando recibido:");
  Serial.println(comando);
 
  String modo = interpretarModo(comando);
 
  if (modo != "none") {
 
    activarModo(modo);
 
    Serial.print("Modo activado: ");
    Serial.println(modo);
 
    server.send(200, "application/json",
      "{\"status\":\"ok\",\"modo\":\""+modo+"\"}");
 
  } else {
 
    mostrarOLED("ERROR");  // ✅ agregado
 
    server.send(400, "application/json",
      "{\"status\":\"error\"}");
  }
}
 
// ==========================
// 🚀 SETUP
// ==========================
void setup() {
 
  Serial.begin(115200);
 
  // ✅ EXACTAMENTE COMO TU TEST FUNCIONÓ
  Wire.begin(21, 22);
  delay(1000);
 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED FALLÓ");
  } else {
    Serial.println("OLED OK");
 
    // ✅ mensaje inicial (igual test)
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.println("INICIO");
    display.display();
  }
 
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
 
  apagarTodo();
 
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("\n✅ Conectado a WiFi");
  Serial.print("📡 IP ESP32: ");
  Serial.println(WiFi.localIP());
 
  // ✅ listo en pantalla
  mostrarOLED("LISTO");
 
  // Rutas
  server.on("/modo", handleModo);
 
  server.begin();
}
 
// ==========================
void loop() {
  server.handleClient();
}
```

---

# 📡 Monitor Serial

El ESP32 muestra:

```bash
Conectando a WiFi...
✅ Conectado a WiFi
📡 IP ESP32: 192.168.X.X
```



<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135604.png" alt="Captura 5" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135619.png" alt="Captura 6" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135637.png" alt="Captura 7" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135647.png" alt="Captura 8" width="900" />
</p>


<p align="center">
  <img src="Foto/Captura de pantalla 2026-05-29 135656.png" alt="Captura 9" width="900" />
</p>
