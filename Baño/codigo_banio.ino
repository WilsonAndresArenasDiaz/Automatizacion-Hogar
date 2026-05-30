#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==========================
// Configuración WiFi
// ==========================
const char* ssid = "BigDj";
const char* password = "BIGDJ111";

// ==========================
// OLED
// ==========================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==========================
// Pines LED (RGB)
// ==========================
#define LED_ROJO   25
#define LED_VERDE  26
#define LED_AMARILLO 27   // amarillo para modo día

WebServer server(80);

// ==========================
// Funciones OLED
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
// Control hardware
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
    mostrarOLED("SPA");
  }
  else if (modo == "manana") {
    digitalWrite(LED_AMARILLO, HIGH);
    mostrarOLED("DIA");
  }
  else if (modo == "noche") {
    digitalWrite(LED_VERDE, HIGH);
    mostrarOLED("NOCHE");
  }
}

// ==========================
// Procesar comando
// ==========================
String interpretarModo(String entrada) {
  entrada.toLowerCase();
  if (entrada.indexOf("spa") >= 0) return "spa";
  if (entrada.indexOf("mañana") >= 0 || entrada.indexOf("manana") >= 0) return "manana";
  if (entrada.indexOf("noche") >= 0) return "noche";
  return "none";
}

// ==========================
// Endpoint HTTP
// ==========================
void handleModo() {
  String comando = server.arg("modo");
  String modo = interpretarModo(comando);
  if (modo != "none") {
    activarModo(modo);
    server.send(200, "application/json", "{\"status\":\"ok\",\"modo\":\"" + modo + "\"}");
  } else {
    mostrarOLED("ERROR");
    server.send(400, "application/json", "{\"status\":\"error\"}");
  }
}

// ==========================
// Setup
// ==========================
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED falló");
  } else {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,20);
    display.println("INICIO");
    display.display();
  }

  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  apagarTodo();

  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nIP: " + WiFi.localIP().toString());
  mostrarOLED("LISTO");

  server.on("/modo", handleModo);
  server.begin();
}

void loop() {
  server.handleClient();
}
