#include <Arduino.h>

#define BUZZ_PIN 23
#define LDR_PIN 4

int adc;

/* Atividade 3
  Desenvolva uma aplicação, onde haja o controle de intensidade de luz (LED) e som (buzzer), de acordo com a intensidade de luz no ambiente (LDR).
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(BUZZ_PIN, OUTPUT); // Definimos o pino 18 (LED) como saída
  ledcAttachPin(BUZZ_PIN, 0); // Atribuimos o pino 18 ao canal 0.
  ledcSetup(0, 5000, 12); // Atribuimos ao canal 0 a frequenciad e 5KHz com resolucao de 10bits.

  analogReadResolution(12); // resolução do ADC 12 bits
  analogSetPinAttenuation(LDR_PIN, ADC_11db);
}

void loop() {
  // put your main code here, to run repeatedly:
  adc = analogRead(LDR_PIN);
  ledcWrite(0, adc);
  Serial.println("Intensidade da luz no ambiente: "+String(adc));
  delay(1000);
}