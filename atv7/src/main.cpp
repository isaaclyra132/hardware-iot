#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

/* Atividade 7
  A atividade consistiu em utilizar o protocolo BLE(Bluetooth Low Energy)
  para envio e recepção de sinais de sensores e leds. A interface gráfica
  foi produzida no app inventor.
*/

String valor;
String temp_s;
String button_s;
int temp_i;
int button_i;
int led1 = 22;
int led2 = 23; 
int temp = 39;
int button = 36;


#define SERVICE_UUID        "2b63d1ab-48b5-4709-bff5-263ae5df6f7c"
#define CHARACTERISTIC_UUID "1a77e754-6df0-4efa-89f6-680cdbff28f2"

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
     
    if (value.length() > 0) {
        valor = "";
        for (int i = 0; i < value.length(); i++){
          // Serial.print(value[i]); 
          valor = valor + value[i];
        }

        Serial.println("*********");
        Serial.print("valor = ");
        Serial.println(valor); 
        if(valor == "temp") {            
          pCharacteristic->setValue(temp_s.c_str()); 
          }
        if(valor == "button") {            
          pCharacteristic->setValue(button_s.c_str()); 
          }
        if(valor == "L1") {
            digitalWrite(led1, HIGH);
          pCharacteristic->setValue("Ligado"); 
          }          
        if(valor == "D1") {
            digitalWrite(led1, LOW);
          pCharacteristic->setValue("Desligado"); 
          }  
        if(valor == "L2") {
            digitalWrite(led2, HIGH);
          pCharacteristic->setValue("Ligado"); 
          }          
        if(valor == "D2") {
            digitalWrite(led2, LOW);
          pCharacteristic->setValue("Desligado"); 
          }     
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  analogReadResolution(12); // resolução do ADC 12 bits
  analogSetAttenuation(ADC_0db);  // Atenuação do ADC 1,1V - 4095
  analogSetPinAttenuation(button, ADC_11db); // Atenuação do ADC 3,3V - 4095
  BLEDevice::init("Pratica 07");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Iniciado.");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  delay(1000);
  temp_i=analogRead(temp);
  temp_s = (String) temp_i;
  button_i=analogRead(button);
  button_s = (String) button_i;
 }
