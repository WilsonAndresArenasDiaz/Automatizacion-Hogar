<img width="914" height="521" alt="image" src="https://github.com/user-attachments/assets/d0ebf1c7-6fa5-40e6-bf97-dcffdceb2c1f" />


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
 
// ✅ SOLO AGREGADO OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
// ==========================
// ✅ CONFIG OLED
// ==========================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
// ==========================
// 📶 CONFIGURACIÓN WIFI
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
// ✅ FUNCIÓN OLED (mínima y segura)
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

# 🔥 Asistente de Voz

```
from flask import Flask, request, send_file, jsonify
import requests

ESP_BANO = ["192.168.43.63"]
ESP_COCINA = ["192.168.43.104"]

# ✅ NUEVO ESP HABITACIÓN
ESP_HABITACION = ["192.168.43.123"]

# ✅ SERVIDOR REMOTO (el de tu ESP32 habitación)
COMANDO_URL = "https://grupoeos.com.co/api/enviar_comando.php"
API_KEY = "dRcor0tZY54MGdR7jJhVriwpilioskj0"

app = Flask(__name__)

# ==========================
# ✅ RUTA PRINCIPAL (HTML)
# ==========================
@app.route("/")
def index():
    return send_file("index.html")

# ==========================
# 🧠 NORMALIZAR TEXTO
# ==========================
def normalizar(texto):
    return texto.lower().strip()

# ==========================
# 🔥 DETECTAR MODO BAÑO
# ==========================
def detectar_modo(texto):

    if any(x in texto for x in ["spa","relajar","calma","descanso","tranquilo"]):
        return "spa"

    if any(x in texto for x in ["dia","día","diurno","mañana","manana","luz","claro"]):
        return "manana"

    if any(x in texto for x in ["noche","oscuro","dormir"]):
        return "noche"

    return "none"

# ==========================
# 🍳 DETECTAR COCINA
# ==========================
def detectar_cocina(texto):

    if any(x in texto for x in ["estufa","cocina","gas","fogón","fogon"]):
        return "estufa"

    if any(x in texto for x in ["nevera","refrigerador","heladera"]):
        return "nevera"

    if any(x in texto for x in ["extractor","ventilador","humo","aire"]):
        return "extractor"

    if any(x in texto for x in ["bomba","agua","riego","regar"]):
        return "bomba"

    if any(x in texto for x in ["buzzer","alarma","sonido"]):
        return "buzzer"

    return "none"

# ==========================
# 🛏️ DETECTAR HABITACIÓN (NUEVO)
# ==========================
def detectar_habitacion(texto):

    if any(x in texto for x in ["luz habitacion","luz cuarto","luz habitacion"]):
        return "luz"

    if any(x in texto for x in ["persiana","cortina","ventana"]):
        return "persiana"

    if "modo noche" in texto:
        return "modo_noche"

    if "automatico" in texto:
        return "automatico"

    return "none"

# ==========================
# ✅ DETECTAR ORDEN
# ==========================
def es_orden(texto):
    return any(x in texto for x in [
        "modo","cambia","pon","activa","enciende",
        "apaga","prende","haz","quiero",
        "activar","apagar","iniciar","detener"
    ])

# ==========================
# 📊 SENSORES
# ==========================
def leer_sensores():
    try:
        r = requests.get(
            f"http://{ESP_COCINA[0]}/datos",
            timeout=2,
            headers={"Connection": "close"}
        )
        return r.json()
    except:
        return None

# ==========================
# 🤖 CHAT PRINCIPAL
# ==========================
@app.route("/chat", methods=["POST"])
def chat():

    data = request.get_json(silent=True) or {}
    texto = normalizar(data.get("mensaje", ""))

    print("\n👤 Usuario:", texto)

    modo = detectar_modo(texto)
    cocina = detectar_cocina(texto)
    habitacion = detectar_habitacion(texto)

    ejecutar = es_orden(texto)

    # ✅ forzar ejecución
    if cocina in ["extractor","bomba","buzzer"]:
        ejecutar = True

    if habitacion != "none":
        ejecutar = True

    estado = "on"
    if "apaga" in texto:
        estado = "off"

    accion_realizada = False

    # ==========================
    # ✅ BAÑO
    # ==========================
    if modo != "none" and ejecutar:
        for ip in ESP_BANO:
            try:
                requests.get(f"http://{ip}/modo?modo={modo}", timeout=2)
                accion_realizada = True
            except:
                pass

    # ==========================
    # ✅ COCINA
    # ==========================
    if cocina != "none" and ejecutar:
        for ip in ESP_COCINA:
            try:

                if cocina == "extractor":
                    requests.get(f"http://{ip}/extractor", timeout=10, headers={"Connection": "close"})

                elif cocina == "bomba":
                    requests.get(f"http://{ip}/bomba", timeout=10, headers={"Connection": "close"})

                elif cocina == "buzzer":
                    requests.get(f"http://{ip}/buzzer", timeout=3)

                else:
                    requests.get(f"http://{ip}/{cocina}?estado={estado}", timeout=3)

                accion_realizada = True

            except Exception as e:
                print("❌ error cocina:", e)

    # ==========================
    # ✅ HABITACIÓN (NUEVO CONTROL VOZ)
    # ==========================
    if habitacion != "none" and ejecutar:
        try:
            print("📡 HABITACIÓN:", habitacion)

            comando_texto = ""

            if habitacion == "luz":
                if estado == "on":
                    comando_texto = "enciende la luz"
                else:
                    comando_texto = "apaga la luz"

            elif habitacion == "persiana":
                if "sube" in texto:
                    comando_texto = "sube la persiana"
                elif "baja" in texto:
                    comando_texto = "baja la persiana"

            elif habitacion == "modo_noche":
                comando_texto = "modo noche"

            elif habitacion == "automatico":
                comando_texto = "modo automatico"

            # ✅ enviar comando al servidor ESP32
            if comando_texto != "":
                requests.post(COMANDO_URL, json={
                    "api_key": API_KEY,
                    "comando": comando_texto
                }, timeout=5)

                print("✅ Comando enviado habitación:", comando_texto)
                accion_realizada = True

        except Exception as e:
            print("❌ error habitación:", e)

    # ==========================
    # ✅ SENSORES
    # ==========================
    if any(x in texto for x in ["gas","humedad","fuego"]):
        datos = leer_sensores()
        if datos:
            return jsonify({
                "respuesta": f"Gas:{datos['gas']} | Humedad:{datos['humedad']} | Fuego:{datos['fuego']}"
            })

    # ==========================
    # ✅ RESPUESTA
    # ==========================
    if accion_realizada:
        return jsonify({"respuesta": "✅ Acción ejecutada"})

    return jsonify({
        "respuesta": "🤖 No entendí el comando"
    })

# ==========================
# 🚀 RUN
# ==========================
if __name__ == "__main__":
    print("🚿🤖 SERVIDOR FINAL FUNCIONANDO")
    app.run(host="0.0.0.0", port=5000)
```


# 🔥 HTML

<img width="984" height="823" alt="image" src="https://github.com/user-attachments/assets/47cdd1fe-6db8-4fdb-80e0-4aded127c0e2" />


```
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<title>Smart Bathroom AI</title>

<style>

body {
  margin:0;
  font-family: 'Segoe UI', sans-serif;
  background: linear-gradient(135deg, #0f2027, #203a43);
  color:white;
  display:flex;
  justify-content:center;
  align-items:center;
  height:100vh;
}

.container {
  width:380px;
  background: rgba(255,255,255,0.05);
  border-radius:20px;
  padding:20px;
  box-shadow: 0 0 40px rgba(0,0,0,0.6);
}

h2 {
  text-align:center;
}

/* OLED SIMULADO */
.oled {
  background:black;
  color:#00ffcc;
  font-family: monospace;
  text-align:center;
  padding:15px;
  border-radius:10px;
  margin-bottom:15px;
  box-shadow: 0 0 20px #00ffcc;
}

.oled-title {
  font-size:12px;
  color:#777;
}

.oled-status {
  font-size:22px;
  font-weight:bold;
}

/* CHAT */
.chat {
  height:250px;
  overflow-y:auto;
  background:#111f2c;
  padding:10px;
  border-radius:10px;
}

.msg {
  margin:5px;
  padding:8px;
  border-radius:8px;
  max-width:75%;
}

.user {
  background:#007bff;
  margin-left:auto;
}

.bot {
  background:#00c6ff;
  color:black;
}

/* INPUT */
.inputBox {
  display:flex;
  margin-top:10px;
}

input {
  flex:1;
  padding:10px;
  border-radius:10px;
  border:none;
  outline:none;
}

button {
  margin-left:5px;
  padding:10px;
  border:none;
  border-radius:10px;
  cursor:pointer;
}

.send {
  background:#007bff;
  color:white;
}

.mic {
  background:#00c851;
  color:white;
}

/* INDICADOR COLOR */
.barra {
  margin-top:10px;
  height:10px;
  border-radius:10px;
}

</style>
</head>

<body>

<div class="container">

<h2>🤖 Smart Bathroom</h2>

<!-- OLED SIMULADO -->
<div class="oled">
  <div class="oled-title">MODO ACTUAL</div>
  <div id="oled" class="oled-status">---</div>
</div>

<div id="chat" class="chat"></div>

<div id="barra" class="barra"></div>

<div class="inputBox">
  <input id="input" placeholder="Escribe o habla..." onkeydown="if(event.key==='Enter'){enviar()}">
  <button class="send" onclick="enviar()">Enviar</button>
  <button class="mic" onclick="voz()">🎤</button>
</div>

</div>

<script>

// =====================
// CHAT
// =====================
function addMsg(text, tipo){
  let chat = document.getElementById("chat");

  let div = document.createElement("div");
  div.className = "msg " + tipo;
  div.innerText = text;

  chat.appendChild(div);
  chat.scrollTop = chat.scrollHeight;
}

// =====================
// ENVIAR TEXTO
// =====================
async function enviar(){

  let input = document.getElementById("input");
  let texto = input.value.trim();

  if(texto === "") return;

  addMsg("Tú: " + texto, "user");
  input.value = "";

  try {

    let res = await fetch("http://localhost:5000/chat", {
      method:"POST",
      headers:{"Content-Type":"application/json"},
      body: JSON.stringify({mensaje:texto})
    });

    let data = await res.json();

    addMsg("Bot: " + data.respuesta, "bot");

    actualizarUI(data.modo);

  } catch(e){
    addMsg("Bot: Error conexión 🤖", "bot");
  }
}

// =====================
// VOZ
// =====================
function voz(){

  const SpeechRecognition = window.webkitSpeechRecognition;

  if(!SpeechRecognition){
    alert("No soporta voz");
    return;
  }

  let recognition = new SpeechRecognition();
  recognition.lang = "es-ES";

  recognition.onresult = function(event){
    let texto = event.results[0][0].transcript;
    document.getElementById("input").value = texto;
    enviar();
  };

  recognition.start();
}

// =====================
// ACTUALIZAR OLED + BARRA
// =====================
function actualizarUI(modo){

  let oled = document.getElementById("oled");
  let barra = document.getElementById("barra");

  if(modo === "spa"){
    oled.innerText = "SPA";
    barra.style.background = "red";
  }
  else if(modo === "manana"){
    oled.innerText = "MAÑANA";
    barra.style.background = "yellow";
  }
  else if(modo === "noche"){
    oled.innerText = "NOCHE";
    barra.style.background = "green";
  }
  else {
    oled.innerText = "---";
    barra.style.background = "transparent";
  }
}

</script>

</body>
</html>
```
