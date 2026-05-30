#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "BigDj";
const char* password = "BIGDJ111";
const char* SERVER_URL = "https://grupoeos.com.co/api/recibir_datos.php";
const char* API_KEY = "clave_secreta_proyecto_2025";

WebServer server(80);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

#define SENSOR_ENTRADA 34
#define SENSOR_SALIDA  35
#define SERVO_PIN      13
#define LED_ROJO       14
#define LED_VERDE      27

int espacios = 8;
const int MAX_ESPACIOS = 8;
unsigned long ultimoEnvio = 0;
const unsigned long INTERVALO_ENVIO = 5000;
bool entradaActiva = false;
bool salidaActiva = false;

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_ENTRADA, INPUT_PULLUP);
  pinMode(SENSOR_SALIDA, INPUT_PULLUP);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
  servo.attach(SERVO_PIN);
  servo.write(0);
  lcd.init(); lcd.backlight();
  actualizarLCD();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("IP: " + WiFi.localIP().toString());

  server.on("/estado", []() {
    String json = "{\"espacios\":" + String(espacios) + ",\"disponibles\":" + String(espacios) + "}";
    server.send(200, "application/json", json);
  });
  server.begin();
}

void loop() {
  server.handleClient();

  bool entrada = !digitalRead(SENSOR_ENTRADA);
  bool salida = !digitalRead(SENSOR_SALIDA);

  if (entrada && !entradaActiva && espacios > 0) {
    entradaActiva = true;
    espacios--;
    abrirTalanquera();
    actualizarLCD();
    enviarDatos();
  } else if (!entrada) entradaActiva = false;

  if (salida && !salidaActiva && espacios < MAX_ESPACIOS) {
    salidaActiva = true;
    espacios++;
    abrirTalanquera();
    actualizarLCD();
    enviarDatos();
  } else if (!salida) salidaActiva = false;

  // LEDs indicadores
  digitalWrite(LED_ROJO, espacios == 0 ? HIGH : LOW);
  digitalWrite(LED_VERDE, espacios > 0 ? HIGH : LOW);

  if (millis() - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = millis();
    enviarDatos();
  }
  delay(50);
}

void abrirTalanquera() {
  servo.write(90);
  delay(1500);
  servo.write(0);
}

void actualizarLCD() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Garaje");
  lcd.setCursor(0,1);
  lcd.print("Libres: ");
  lcd.print(espacios);
  lcd.print("/");
  lcd.print(MAX_ESPACIOS);
}

void enviarDatos() {
  if (WiFi.status() != WL_CONNECTED) return;
  StaticJsonDocument<200> doc;
  doc["api_key"] = API_KEY;
  doc["modulo"] = "garaje";
  doc["espacios_libres"] = espacios;
  doc["total"] = MAX_ESPACIOS;
  String json; serializeJson(doc, json);
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", API_KEY);
  int code = http.POST(json);
  if (code == 200) Serial.println("Datos garaje enviados");
  else Serial.printf("Error %d\n", code);
  http.end();
}
