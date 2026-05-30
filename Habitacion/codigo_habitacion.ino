#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <BH1750.h>
#include <TM1637Display.h>
#include <NTPClient.h>
#include <WiFiUDP.h>

const char* WIFI_SSID = "BigDj";
const char* WIFI_PASSWORD = "BIGDJ111";
const char* SERVER_URL = "https://grupoeos.com.co/api/recibir_datos.php";
const char* COMANDO_URL = "https://grupoeos.com.co/api/obtener_comando.php";
const char* API_KEY = "dRcor0tZY54MGdR7jJhVriwpilioskj0";

#define PIN_RELE      33
#define PIN_MOTOR_IN1 27
#define PIN_MOTOR_IN2 14
#define PIN_POTENCIOM 34
#define PIN_TM_CLK    18
#define PIN_TM_DIO    19

BH1750 sensorLuz;
TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);
WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP, "pool.ntp.org", -18000, 60000);

bool luzEncendida = false;
bool modoAutomatico = false;
float umbralEncender = 100.0;
float umbralApagar = 400.0;
int posicionAnterior = -1;

unsigned long tUltimoLuz = 0, tUltimoPersi = 0, tUltimoReloj = 0, tUltimoDisplay = 0, tUltimoVoz = 0;
const unsigned long INTERVALO_LUZ = 10000, INTERVALO_PERSI = 2000, INTERVALO_RELOJ = 3600000, INTERVALO_DISPLAY = 1000, INTERVALO_VOZ = 3000;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RELE, OUTPUT);
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);
  digitalWrite(PIN_RELE, LOW);
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  display.setBrightness(7);
  display.showNumberDec(0, true);
  Wire.begin(21,22);
  sensorLuz.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  conectarWiFi();
  ntp.begin();
  ntp.update();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) conectarWiFi();
  ntp.update();
  gestionarIluminacion();
  gestionarPersiana();
  gestionarDisplay();
  consultarComandoVoz();
  if (millis() - tUltimoReloj >= INTERVALO_RELOJ || tUltimoReloj == 0) {
    enviarReloj();
    tUltimoReloj = millis();
  }
}

void gestionarIluminacion() {
  if (millis() - tUltimoLuz < INTERVALO_LUZ) return;
  tUltimoLuz = millis();
  float lux = sensorLuz.readLightLevel();
  if (lux < 0) return;
  if (modoAutomatico) {
    if (lux < umbralEncender && !luzEncendida) encenderLuz();
    else if (lux > umbralApagar && luzEncendida) apagarLuz();
  }
  String estado = luzEncendida ? "encendido" : "apagado";
  Serial.printf("Luz: %.1f lux - %s\n", lux, estado.c_str());
  StaticJsonDocument<200> doc;
  doc["api_key"] = API_KEY;
  doc["modulo"] = "iluminacion";
  doc["lux"] = lux;
  doc["estado"] = estado;
  enviarJSON(doc);
}

void encenderLuz() { digitalWrite(PIN_RELE, HIGH); luzEncendida = true; Serial.println("Luz encendida"); }
void apagarLuz() { digitalWrite(PIN_RELE, LOW); luzEncendida = false; Serial.println("Luz apagada"); }

void gestionarPersiana() {
  if (millis() - tUltimoPersi < INTERVALO_PERSI) return;
  tUltimoPersi = millis();
  int raw = analogRead(PIN_POTENCIOM);
  int posicion = map(raw, 0, 4095, 0, 100);
  if (abs(posicion - posicionAnterior) < 3) return;
  String comando = "manual";
  if (posicionAnterior != -1) {
    if (posicion > posicionAnterior) { comando = "subir"; subirPersiana(); }
    else { comando = "bajar"; bajarPersiana(); }
  }
  posicionAnterior = posicion;
  delay(500);
  detenerPersiana();
  Serial.printf("Persiana: %d%% - %s\n", posicion, comando.c_str());
  StaticJsonDocument<200> doc;
  doc["api_key"] = API_KEY;
  doc["modulo"] = "persiana";
  doc["posicion"] = posicion;
  doc["comando"] = comando;
  enviarJSON(doc);
}
void subirPersiana() { digitalWrite(PIN_MOTOR_IN1, HIGH); digitalWrite(PIN_MOTOR_IN2, LOW); }
void bajarPersiana() { digitalWrite(PIN_MOTOR_IN1, LOW); digitalWrite(PIN_MOTOR_IN2, HIGH); }
void detenerPersiana() { digitalWrite(PIN_MOTOR_IN1, LOW); digitalWrite(PIN_MOTOR_IN2, LOW); }

void gestionarDisplay() {
  if (millis() - tUltimoDisplay < INTERVALO_DISPLAY) return;
  tUltimoDisplay = millis();
  int hora = ntp.getHours();
  int minutos = ntp.getMinutes();
  int tiempo = hora * 100 + minutos;
  bool puntos = (millis() / 1000) % 2;
  display.showNumberDecEx(tiempo, puntos ? 0b01000000 : 0, true);
}
void enviarReloj() {
  String hora = ntp.getFormattedTime();
  StaticJsonDocument<200> doc;
  doc["api_key"] = API_KEY;
  doc["modulo"] = "reloj";
  doc["hora_dispositivo"] = hora;
  enviarJSON(doc);
}

void consultarComandoVoz() {
  if (millis() - tUltimoVoz < INTERVALO_VOZ) return;
  tUltimoVoz = millis();
  HTTPClient http;
  String url = String(COMANDO_URL) + "?api_key=" + API_KEY;
  http.begin(url);
  http.setTimeout(5000);
  int code = http.GET();
  if (code == 200) {
    String resp = http.getString();
    StaticJsonDocument<256> doc;
    deserializeJson(doc, resp);
    if (doc["ok"] == true) {
      String comando = doc["comando"].as<String>();
      if (comando.length() > 0) procesarComandoVoz(comando);
    }
  }
  http.end();
}
void procesarComandoVoz(String comando) {
  comando.toLowerCase();
  if (comando.indexOf("enciende la luz") >= 0) { modoAutomatico = false; encenderLuz(); }
  else if (comando.indexOf("apaga la luz") >= 0) { modoAutomatico = false; apagarLuz(); }
  else if (comando.indexOf("sube la persiana") >= 0) { subirPersiana(); delay(2000); detenerPersiana(); }
  else if (comando.indexOf("baja la persiana") >= 0) { bajarPersiana(); delay(2000); detenerPersiana(); }
  else if (comando.indexOf("modo noche") >= 0) { modoAutomatico = false; apagarLuz(); bajarPersiana(); delay(3000); detenerPersiana(); }
  else if (comando.indexOf("modo automatico") >= 0) modoAutomatico = true;
}

void conectarWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi conectado, IP: " + WiFi.localIP().toString());
}
void enviarJSON(StaticJsonDocument<200>& doc) {
  if (WiFi.status() != WL_CONNECTED) return;
  String json; serializeJson(doc, json);
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", API_KEY);
  int code = http.POST(json);
  if (code != 200) Serial.printf("Error envio: %d\n", code);
  http.end();
}
