#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
const int speakerPin = 9;
const int buttonPins[] = {2,3,4,5};
const int notes[] = {262,294,330,349};
const String noteNames[] = {"DO","RE","MI","FA"};
const int numButtons = 4;
const int MAX_PASOS = 50;
int secuencia[MAX_PASOS];
int nivel = 0;
int tiempoMuestraInicial = 500;
int tiempoPausaInicial = 250;
int factorAceleracion = 25;
int tiempoMuestraMinimo = 150;
int tiempoPausaMinimo = 80;

void setup() {
  pinMode(speakerPin, OUTPUT);
  for(int i=0;i<numButtons;i++) pinMode(buttonPins[i], INPUT_PULLUP);
  lcd.init(); lcd.backlight();
  randomSeed(analogRead(A0));
  mostrarPantallaInicio();
}
void mostrarPantallaInicio() {
  lcd.clear();
  lcd.print("SIMON DICE");
  lcd.setCursor(0,1); lcd.print("Presiona boton");
  bool empezar = false;
  while(!empezar) {
    for(int i=0;i<numButtons;i++) if(digitalRead(buttonPins[i])==LOW) { empezar=true; break; }
    delay(50);
  }
  // Sonido inicio
  tone(speakerPin,440); delay(200); tone(speakerPin,554); delay(200); tone(speakerPin,659); delay(200); noTone(speakerPin);
  nivel=0;
  loop();
}
void loop() {
  secuencia[nivel] = random(0,numButtons);
  nivel++;
  int tiempoMuestra = tiempoMuestraInicial - (nivel*factorAceleracion);
  if(tiempoMuestra<tiempoMuestraMinimo) tiempoMuestra=tiempoMuestraMinimo;
  int tiempoPausa = tiempoPausaInicial - (nivel*(factorAceleracion/2));
  if(tiempoPausa<tiempoPausaMinimo) tiempoPausa=tiempoPausaMinimo;

  lcd.clear(); lcd.print("Nivel: "); lcd.print(nivel);
  delay(1000);
  for(int i=0;i<nivel;i++) {
    int boton = secuencia[i];
    lcd.setCursor(0,1); lcd.print(noteNames[boton]);
    tone(speakerPin, notes[boton]); delay(tiempoMuestra); noTone(speakerPin);
    delay(tiempoPausa);
  }
  lcd.clear(); lcd.print("TU TURNO...");
  for(int i=0;i<nivel;i++) {
    int botonCorrecto = secuencia[i];
    int botonPresionado = -1;
    while(botonPresionado == -1) {
      for(int b=0;b<numButtons;b++) if(digitalRead(buttonPins[b])==LOW) { botonPresionado = b; break; }
      delay(50);
    }
    tone(speakerPin, notes[botonPresionado]); delay(200); noTone(speakerPin);
    lcd.setCursor(0,1); lcd.print(noteNames[botonPresionado]);
    if(botonPresionado != botonCorrecto) {
      ejecutarGameOver();
      return;
    }
    delay(500);
  }
  lcd.clear(); lcd.print("¡CORRECTO!");
  for(int i=0;i<3;i++) { tone(speakerPin,523); delay(150); tone(speakerPin,659); delay(150); tone(speakerPin,784); delay(150); } noTone(speakerPin);
  delay(1000);
  loop();
}
void ejecutarGameOver() {
  lcd.clear(); lcd.print("¡ERROR!"); lcd.setCursor(0,1); lcd.print("Score: "); lcd.print(nivel-1);
  for(int i=0;i<3;i++) { tone(speakerPin,300); delay(200); tone(speakerPin,200); delay(200); tone(speakerPin,130); delay(200); } noTone(speakerPin);
  delay(3000);
  mostrarPantallaInicio();
}
