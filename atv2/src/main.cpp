#include <Arduino.h>

#define LED_PIN 21
#define LM35_PIN 4
#define LDR_PIN 2

//variaveis
int ldr;
int adc;
float temperatura;

/* Atividade 2
  Faça uma aplicação onde com os seguintes requisitos: - A medição de temperatura do ambiente - Indique se é o ambiente está com boa iluminação, escuro ou claro demais.- Apresente esses dados na porta serial- Caso o ambiente esteja muito escuro acenda um led.
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //inicia a porta serial na velocidade de 115200
  analogReadResolution(12); // resolução do ADC 12 bits
  pinMode(LED_PIN, OUTPUT);

  // SENSOR DE LUMINOSIDADE
  analogSetPinAttenuation(LDR_PIN, ADC_11db);

  // SENSOR DE TEMPERATURA
  analogSetPinAttenuation(LM35_PIN, ADC_0db);

  // analogSetAttenuation(ADC_0db); // Atenuação do ADC 1,1V - 4095
  //analogSetPinAttenuation(lm35, ADC_11db); //3.3V
}

void loop() {
  // put your main code here, to run repeatedly:
  ldr = analogRead(LDR_PIN);
  adc = analogRead(LM35_PIN);
  temperatura = adc*0.02686203;
  Serial.println("Temperatura é: "+String(temperatura)+" °C");
  if(ldr<=1000){
    Serial.print("Valor do LDR é: "+String(ldr)+", baixa iluminação, led aceso\n\n");
    digitalWrite(LED_PIN, HIGH);
  }
  else if(ldr>1000 && ldr <3000){
    Serial.print("Valor do LDR é: "+String(ldr)+", boa iluminação, led apagado\n\n");
    digitalWrite(LED_PIN, LOW);
  }
  else {
    Serial.print("Valor do LDR é: "+String(ldr)+", ambiente claro demais, led apagado\n\n");
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
}