#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_MIC A0
#define LED_VERDE 5
#define LED_AMARELO 6
#define LED_VERMELHO 7

// Limites de alerta em decibéis estimados
#define LIMITE_PLUGS 85
#define LIMITE_ABAFADOR 95
#define LIMITE_CRITICO 100

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Monitor de Ruido");
  delay(2000);
  lcd.clear();
}

void loop() {
  int somaLeituras = 0;
  int numLeituras = 30;
  int maiorValor = 0;
  int menorValor = 1023;

  for (int i = 0; i < numLeituras; i++) {
    int leitura = analogRead(SENSOR_MIC);
    somaLeituras += leitura;
    if (leitura > maiorValor) maiorValor = leitura;
    if (leitura < menorValor) menorValor = leitura;
    delay(5);
  }

  int media = somaLeituras / numLeituras;
  int pico = maiorValor - menorValor;

  // Conversão aproximada para decibéis (ajustável)
  float decibeis = map(pico, 0, 1023, 30, 120);

  // Exibe no monitor serial
  Serial.print("Media: ");
  Serial.print(media);
  Serial.print(" | Pico: ");
  Serial.print(pico);
  Serial.print(" | Estimado: ");
  Serial.print(decibeis);
  Serial.println(" dB");

  // Atualiza LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nivel: ");
  lcd.print(decibeis, 1);
  lcd.print(" dB");

  lcd.setCursor(0, 1);

  // Lógica dos alertas com LEDs
  if (decibeis >= LIMITE_CRITICO) {
    lcd.print("ALERTA CRITICO!");
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
  }
  else if (decibeis >= LIMITE_ABAFADOR) {
    lcd.print("Use ABAFADOR");
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
  }
  else if (decibeis >= LIMITE_PLUGS) {
    lcd.print("Use PLUG AURAL");
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
  }
  else {
    lcd.print("Ambiente OK");
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
  }

  delay(1000);
}
