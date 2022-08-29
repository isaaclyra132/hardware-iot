#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

/* Atividade 4
  Nesta prática foi criado um servidor HTTP que foi hospedado no próprio ESP32,
  ao se conectar no IP oferecido pelo ESP, é possível ligar e desligar dois leds,
  como também visualizar o valor lido pelo sensor de temperatura.
*/

/*Put your SSID & Password*/
const char* ssid = "Isaac_fastrn"; // Nome da rede
const char* password = "@88653111"; // Senha da rede

WebServer server(80);

uint8_t led1Pin = 22;
bool led1Status = LOW;

uint8_t led2Pin = 23;
bool led2Status = LOW;

int LM35 = 36;
u32_t adc;
float temperatura;

void handle_OnConnect();
void handle_led1on();
void handle_led1off();
void handle_led2on();
void handle_led2off();
void handle_NotFound();
String SendHTML(uint8_t led1stat, uint8_t led2stat, float temperatura);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);

  //Configuração dos pinos
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  analogReadResolution(12); // resolução do ADC 12 bits
  analogSetAttenuation(ADC_0db); // Atenuação do ADC 1,1V - 4095

  Serial.println("Conectando em: ");
  Serial.println(ssid);
  
  // Função de conexão na WiFi
  WiFi.begin(ssid, password);

  // Verificação do status da conexão
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectada...!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  //Configuração das URL
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  //server.on("/temp", handle_temp);
  server.onNotFound(handle_NotFound); // para uma url não existente

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient(); // Tratando os objetos do servidor
  adc = analogRead(LM35);
  temperatura = adc*0.02686203; // 110 graus valor máximo dividido pela amostragem
  //Serial.println("Temperatura é: "+String(temperatura)+" °C");

  if(led1Status) { // analisa o status da pagina e liga ou desliga o led 1
    digitalWrite(led1Pin, HIGH);
  }
  else{
    digitalWrite(led1Pin, LOW);
  }

  if(led2Status) { // analisa o status da pagina e liga ou desliga o led 2
    digitalWrite(led2Pin, HIGH);
  }
  else{
    digitalWrite(led2Pin, LOW);
  }
}

void handle_OnConnect() {
  led1Status = LOW;
  led2Status = LOW;
  //Serial.println("Temperatura é: "+String(temperatura)+" °C");
  Serial.println("GPIO22 Status: OFF | GPIO23 Status: OFF");
  server.send(200, "text/html", SendHTML(led1Status, led2Status, temperatura));
}

void handle_led1on() {
  led1Status = HIGH;
  Serial.println("GPIO22 Status: ON");
  server.send(200, "text/html", SendHTML(true, led2Status, temperatura));
}

void handle_led1off() {
  led1Status = LOW;
  Serial.println("GPIO22 Status: OFF");
  server.send(200, "text/html", SendHTML(false, led2Status, temperatura));
}

void handle_led2on() {
  led2Status = HIGH;
  Serial.println("GPIO23 Status: ON");
  server.send(200, "text/html", SendHTML(led1Status, true, temperatura));
}

void handle_led2off() {
  led2Status = LOW;
  Serial.println("GPIO23 Status: OFF");
  server.send(200, "text/html", SendHTML(led1Status, false, temperatura));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not Found");
}

// pagina HTML
String SendHTML(uint8_t led1stat, uint8_t led2stat, float temperatura){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; color:#001414;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #93c47d;}\n";
  ptr +=".button-on:active {background-color: #425839;}\n";
  ptr +=".button-off {background-color: #e06666;}\n";
  ptr +=".button-off:active {background-color: #703737;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Acionando LED's WiFi e Vendo Temperatura</h1>\n";
  ptr +="<h3>Programando em HTML</h3>\n";

  //Temperatura
  ptr += "<p>Temperatura: ";
  ptr += temperatura;
  ptr += "&deg;C</p>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: Ligado</p><a class=\"button button-off\" href=\"/led1off\">Desligar</a>\n";}
  else
  {ptr +="<p>LED1 Status: Desligado</p><a class=\"button button-on\" href=\"/led1on\">Ligar</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: Ligado</p><a class=\"button button-off\" href=\"/led2off\">Desligar</a>\n";}
  else
  {ptr +="<p>LED2 Status: Desligado</p><a class=\"button button-on\" href=\"/led2on\">Ligar</a>\n";}

  ptr +="<p><a href=\"https://www.w3schools.com\">Visit W3Schools.com!</a></p>\n";
  ptr +="<p><img src=\"https://logodownload.org/wp-content/uploads/2018/04/ufrn-logo.png\" alt=\"Logo UFRN\" width=\"500\"</p>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;

}