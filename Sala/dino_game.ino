#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
byte dino[8] = { B00111, B00101, B00111, B10110, B11111, B01010, B01010, B00000 };
byte cactus[8] = { B00100, B00101, B10101, B10111, B11100, B00100, B00100, B00000 };
int dinoPos = 1;
int score = 0;
bool lastButtonState = HIGH;
const int MAX_CACTUS = 3;
int cactusPos[MAX_CACTUS];
int cactusRow[MAX_CACTUS];
bool cactusActivo[MAX_CACTUS];
int velocidadInicial = 240;
int velocidad = velocidadInicial;
int velocidadMinima = 80;
int factorAceleracion = 6;

void setup() {
  lcd.init(); lcd.backlight();
  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);
  pinMode(12, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  reiniciarVariablesJuego();
  mostrarPantallaInicio();
}
void reiniciarVariablesJuego() {
  score = 0;
  velocidad = velocidadInicial;
  for(int i=0;i<MAX_CACTUS;i++) cactusActivo[i]=false;
  dinoPos=1;
}
void mostrarPantallaInicio() {
  lcd.clear(); lcd.print("DINO GAME"); lcd.setCursor(0,1); lcd.print("Presiona boton");
  while(digitalRead(12)==HIGH) delay(50);
  while(digitalRead(12)==LOW) delay(50);
  loop();
}
void loop() {
  bool buttonState = digitalRead(12);
  if(buttonState==LOW && lastButtonState==HIGH) dinoPos = (dinoPos==1)?0:1;
  lastButtonState = buttonState;
  delay(40);

  if(random(0,100)<30) generarNuevoCactus();
  lcd.clear();
  lcd.setCursor(11,0); lcd.print("S:"); lcd.print(score);
  lcd.setCursor(0, dinoPos); lcd.write(byte(0));
  for(int i=0;i<MAX_CACTUS;i++) {
    if(cactusActivo[i]) {
      lcd.setCursor(cactusPos[i], cactusRow[i]); lcd.write(byte(1));
      if(cactusPos[i]==0 && dinoPos==cactusRow[i]) { ejecutarGameOver(); return; }
      cactusPos[i]--;
      if(cactusPos[i]<0) { cactusActivo[i]=false; score++; }
    }
  }
  velocidad = velocidadInicial - (score * factorAceleracion);
  if(velocidad<velocidadMinima) velocidad=velocidadMinima;
  delay(velocidad);
}
void generarNuevoCactus() {
  for(int i=0;i<MAX_CACTUS;i++) {
    if(!cactusActivo[i]) {
      cactusPos[i]=15;
      cactusRow[i]=random(0,2);
      cactusActivo[i]=true;
      break;
    }
  }
}
void ejecutarGameOver() {
  lcd.clear(); lcd.print("GAME OVER"); lcd.setCursor(0,1); lcd.print("Score: "); lcd.print(score);
  delay(3000);
  reiniciarVariablesJuego();
  mostrarPantallaInicio();
}
