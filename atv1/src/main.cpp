#include <Arduino.h>

#define BUTTON_PIN 16
#define LED_PIN 2
#define TOUCH_PIN T0

int buttonState = 0;
int touchValue = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.println("Atividade 1");
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(BUTTON_PIN);
  touchValue = touchRead(TOUCH_PIN);

  Serial.println("Valor do touch sensor: "+String(touchValue));

  if ( buttonState && touchValue<20) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);
  
  delay(500);
}