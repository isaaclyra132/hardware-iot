#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

/* Atividade 6
  A prática consistiu em utilizar o protocolo de comunicação sem fio
  MQTT para envio de sinais de controle, recepção e exibiçãode dados 
  de sensores através de um broker(adafruit).
*/

#define IO_USERNAME  "jrs132"
#define IO_KEY       "aio_VzSC93je2LD6IkBEF218RDilY9W5"
/*Put your SSID & Password*/
const char* ssid = "Isaac_fastrn"; // Nome da rede
const char* password = "@88653111"; // Senha da rede
const char* mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char* mqttUser = IO_USERNAME;
const char* mqttPassword = IO_KEY;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Entradas digitais
#define LED_PIN1 22
#define LED_PIN2 23

// Entradas analogicas
#define TEMP 36
#define LDR 39
#define POT 34

// Variáveis
float pot = 0;
float ldr = 0;
float temp = 0;
float temperatura = 0;

void setup_wifi() {
  delay(10);
  analogSetAttenuation(ADC_0db);  // Atenuação do ADC 1,1V - 4095
  analogSetPinAttenuation(POT, ADC_11db); // Atenuação do ADC 3,3V - 4095

  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Messagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //messageTemp += (char)payload[i]; <----------Usar quando tiver uma mensagem na resposta do bloco
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if(!strcmp(topic, "jrs132/feeds/led1")){
    if ((char)payload[0] == '1') {
      digitalWrite(LED_PIN1, HIGH);   // Turn the LED on (Note that LOW is the voltage level
      Serial.println("LED Ligado");    
    } else {
      digitalWrite(LED_PIN1, LOW);  // Turn the LED off by making the voltage HIGH
      Serial.println("LED Desligado");    
    }
  } else if(!strcmp(topic, "jrs132/feeds/led2")) {
    if ((char)payload[0] == '1') {
      digitalWrite(LED_PIN2, HIGH);   // Turn the LED on (Note that LOW is the voltage level
      Serial.println("LED Ligado");    
    } else {
      digitalWrite(LED_PIN2, LOW);  // Turn the LED off by making the voltage HIGH
      Serial.println("LED Desligado");    
    }
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    // Create a random client ID
    String clientId = "ESP32 - Sensores";
    clientId += String(random(0xffff), HEX);
    // Se conectado
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("conectado");
      // Depois de conectado, publique um anúncio ...
      client.publish("jrs132/feeds/temp", "Iniciando Comunicação");
      client.publish("jrs132/feeds/pot", "Iniciando Comunicação");
      client.publish("jrs132/feeds/ldr", "Iniciando Comunicação");
      //... e subscribe.
      client.subscribe("jrs132/feeds/led1"); // <<<<----- mudar aqui
      client.subscribe("jrs132/feeds/led2");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s");
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  delay(100);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttserver, 1883); // Publicar
  client.setCallback(callback); // Receber mensagem
}

void loop() {
  // put your main code here, to run repeatedly:
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(10000); //Intervalo de 10s entre leituras
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    temp = analogRead(TEMP);
    temperatura = temp*0.02686203;; //calculo da temperatura            
    char s_temp[8];
    dtostrf(temperatura,1,2,s_temp);
    Serial.print("Temperatura: ");
    Serial.println(s_temp);
    Serial.println(temperatura);
    client.publish("jrs132/feeds/temp", s_temp);

    
    pot = analogRead(POT)/100;
    char s_pot[8];
    dtostrf(pot,1,2,s_pot);
    Serial.print("Potênciometro: ");
    Serial.println(s_pot);
    Serial.println(pot);
    client.publish("jrs132/feeds/pot", s_pot);


    ldr = analogRead(LDR)/100;
    char s_ldr[5];
    dtostrf(ldr,1,2,s_ldr);
    Serial.print("LDR: ");
    Serial.println(s_ldr);
    Serial.println(ldr);
    client.publish("jrs132/feeds/ldr", s_ldr);
  }
}