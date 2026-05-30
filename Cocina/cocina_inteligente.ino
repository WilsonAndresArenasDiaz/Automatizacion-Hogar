#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ==========================
// 📶 WIFI (CAMBIA AQUÍ TUS DATOS SI ES OTRA RED)
// ==========================
const char* ssid     = "BigDj";
const char* password = "BIGDJ111";

// ==========================
// 🌐 HOSTINGER — BD (VERIFICAR CON TU COMPAÑERO)
// ==========================
const char* SERVER_URL = "https://grupoeos.com.co/api/recibir_datos.php";
const char* API_KEY    = "clave_secreta_proyecto_2025";

WebServer server(80);

// ==========================
// 🔌 PINES
// ==========================
const int pinGas      = 34;
const int pinFlama    = 32;
const int pinHumedad  = 35;

const int buzzer      = 25;
const int ledEstufa   = 26;
const int ledNevera   = 27;
const int pinExtractor= 33;   // TIP122 (HIGH activa, LOW apaga)
const int pinBomba    = 14;   // TIP127 (LOW activa, HIGH apaga)

// ==========================
// 📊 UMBRALES (AJUSTA SEGÚN TUS SENSORES)
// ==========================
int umbralGas          = 3000;
int umbralHumedadSeco  = 3200;

// ==========================
// ⏱️ TIMERS
// ==========================
bool riegoEnCurso         = false;
unsigned long tiempoInicioRiego   = 0;
const unsigned long duracionRiego = 5000;
unsigned long ultimoChequeoHumedad= 0;
const unsigned long intervaloChequeo = 10000;

unsigned long ultimoEnvioHostinger = 0;
const unsigned long INTERVALO_ENVIO = 10000;

bool sonidoGasActivado    = false;
bool sonidoFuegoActivado  = false;
bool sonidoHumedadActivado= false;

// ==========================
// 🔔 BUZZER
// ==========================
void sonarBuzzer(int duracionMs = 1000, int frecuencia = 1500) {
  tone(buzzer, frecuencia);
  delay(duracionMs);
  noTone(buzzer);
}

// ==========================
// 📈 LECTURA ANALÓGICA PROMEDIADA
// ==========================
int leerAnalogico(int pin) {
  long suma = 0;
  for (int i = 0; i < 5; i++) {
    suma += analogRead(pin);
    delay(5);
  }
  return suma / 5;
}

// ==========================
// 📤 ENVIAR DATOS A HOSTINGER (DASHBOARD DEL COMPAÑERO)
// ==========================
void enviarAHostinger() {
  if (WiFi.status() != WL_CONNECTED) return;

  int valorGas    = leerAnalogico(pinGas);
  bool hayFuego   = (digitalRead(pinFlama) == LOW);
  int valorHum    = leerAnalogico(pinHumedad);

  float humedadPct = map(valorHum, 0, 4095, 100, 0);
  float gasPpm = (float)valorGas / 4095.0 * 5.0;

  String estadoEstufa = (digitalRead(ledEstufa) == HIGH) ? "encendida" : "apagada";
  String estadoNevera = (digitalRead(ledNevera) == HIGH) ? "encendida" : "apagada";

  StaticJsonDocument<300> doc;
  doc["api_key"]     = API_KEY;
  doc["modulo"]      = "cocina";
  doc["gas"]         = gasPpm;
  doc["fuego"]       = hayFuego ? 1 : 0;
  doc["humedad"]     = humedadPct;
  doc["temperatura"] = 0;
  doc["estufa"]      = estadoEstufa;
  doc["nevera"]      = estadoNevera;

  String json;
  serializeJson(doc, json);

  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", API_KEY);
  http.setTimeout(8000);

  int codigo = http.POST(json);
  if (codigo == 200) {
    Serial.println("[Hostinger] OK → cocina enviada");
  } else {
    Serial.printf("[Hostinger] Error %d\n", codigo);
  }
  http.end();
}

// ==========================
// 🌐 ENDPOINTS DEL SERVIDOR WEB
// ==========================
void handleDatos() {
  int gas    = leerAnalogico(pinGas);
  int fuego  = (digitalRead(pinFlama) == LOW) ? 1 : 0;
  int humedad= leerAnalogico(pinHumedad);
  String json = "{\"gas\":" + String(gas) + ",\"fuego\":" + String(fuego) + ",\"humedad\":" + String(humedad) + "}";
  server.send(200, "application/json", json);
}

void handleEstufa() {
  String estado = server.arg("estado");
  if (estado == "on")  digitalWrite(ledEstufa, HIGH);
  else if (estado == "off") digitalWrite(ledEstufa, LOW);
  server.send(200, "text/plain", "OK");
}

void handleNevera() {
  String estado = server.arg("estado");
  if (estado == "on")  digitalWrite(ledNevera, HIGH);
  else if (estado == "off") digitalWrite(ledNevera, LOW);
  server.send(200, "text/plain", "OK");
}

void handleExtractor() {
  digitalWrite(pinExtractor, HIGH);
  delay(5000);
  digitalWrite(pinExtractor, LOW);
  server.send(200, "text/plain", "OK");
}

void handleBomba() {
  digitalWrite(pinBomba, LOW);
  delay(duracionRiego);
  digitalWrite(pinBomba, HIGH);
  server.send(200, "text/plain", "OK");
}

void handleBuzzer() {
  sonarBuzzer(500, 2000);
  server.send(200, "text/plain", "OK");
}

// ==========================
// 🚀 SETUP
// ==========================
void setup() {
  Serial.begin(115200);

  pinMode(buzzer,       OUTPUT);
  pinMode(ledEstufa,    OUTPUT);
  pinMode(ledNevera,    OUTPUT);
  pinMode(pinExtractor, OUTPUT);
  pinMode(pinBomba,     OUTPUT);
  pinMode(pinFlama,     INPUT);

  digitalWrite(ledEstufa,    LOW);
  digitalWrite(ledNevera,    LOW);
  digitalWrite(pinExtractor, LOW);
  digitalWrite(pinBomba,     HIGH);   // bomba apagada (TIP127)

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi OK");
  Serial.print("📡 IP ESP32 Cocina: ");
  Serial.println(WiFi.localIP());

  server.on("/datos",     handleDatos);
  server.on("/estufa",    handleEstufa);
  server.on("/nevera",    handleNevera);
  server.on("/extractor", handleExtractor);
  server.on("/bomba",     handleBomba);
  server.on("/buzzer",    handleBuzzer);

  server.begin();
  Serial.println("✅ Servidor web OK");
}

// ==========================
// 🔁 LOOP PRINCIPAL
// ==========================
void loop() {
  server.handleClient();

  int valorGas      = leerAnalogico(pinGas);
  bool fuegoDetectado = (digitalRead(pinFlama) == LOW);
  int valorHumedad  = leerAnalogico(pinHumedad);

  // ── ALARMA GAS ───────────────────────────────────
  if (valorGas > umbralGas) {
    if (!sonidoGasActivado) {
      sonidoGasActivado = true;
      Serial.println("⚠️ GAS DETECTADO");
      sonarBuzzer(1000, 2000);
      digitalWrite(pinExtractor, HIGH);
      delay(5000);
      digitalWrite(pinExtractor, LOW);
    }
  } else {
    sonidoGasActivado = false;
  }

  // ── ALARMA FUEGO ─────────────────────────────────
  if (fuegoDetectado) {
    if (!sonidoFuegoActivado) {
      sonidoFuegoActivado = true;
      Serial.println("🔥 FUEGO DETECTADO");
      sonarBuzzer(1000, 2500);
      digitalWrite(pinExtractor, HIGH);
      delay(5000);
      digitalWrite(pinExtractor, LOW);
    }
  } else {
    sonidoFuegoActivado = false;
  }

  // ── RIEGO AUTOMÁTICO (se activa si humedad > umbral) ──
  if (!riegoEnCurso && (millis() - ultimoChequeoHumedad >= intervaloChequeo)) {
    ultimoChequeoHumedad = millis();
    if (valorHumedad > umbralHumedadSeco) {
      Serial.println("💧 Suelo seco, activando riego automático");
      riegoEnCurso = true;
      digitalWrite(pinBomba, LOW);    // activa bomba (TIP127)
      tiempoInicioRiego = millis();
    }
  }

  if (riegoEnCurso && (millis() - tiempoInicioRiego >= duracionRiego)) {
    digitalWrite(pinBomba, HIGH);     // apaga bomba
    riegoEnCurso = false;
    Serial.println("✅ Riego completado");
  }

  // Alerta sonora si el suelo sigue seco (sin riego en curso)
  if (valorHumedad > umbralHumedadSeco) {
    if (!sonidoHumedadActivado && !riegoEnCurso) {
      sonidoHumedadActivado = true;
      Serial.println("💧 ALERTA: Suelo seco");
      sonarBuzzer(500, 1000);
    }
  } else {
    sonidoHumedadActivado = false;
  }

  // ── ENVÍO A HOSTINGER cada 10 segundos ──────────────
  if (millis() - ultimoEnvioHostinger >= INTERVALO_ENVIO) {
    ultimoEnvioHostinger = millis();
    enviarAHostinger();
  }

  delay(50);
}
