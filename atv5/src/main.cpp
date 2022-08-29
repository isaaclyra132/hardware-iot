#include <Arduino.h>
#include <WiFi.h>
//#include <WebServer.h>

/* Atividade 5
  Nesta prática foi criado um servidor HTTP que foi hospedado no próprio ESP32,
  ao se conectar no IP oferecido pelo ESP, é possível ligar e desligar dois leds,
  como também visualizar o valor lido pelo sensor de temperatura.
*/

/*Put your SSID & Password*/
const char* ssid = "Isaac_fastrn"; // Nome da rede
const char* password = "@88653111"; // Senha da rede

WiFiServer server(80);

// Entradas digitais
#define LED_PIN 22

// Entradas analogicas
#define TEMP 36
#define LDR 39
#define POT 34

// Variáveis
int pot = 0;
int ldr = 0;
int temp = 0;
float temperatura = 0;

void connectWifi();
void WiFiLocalWebPageCtrl();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);

  //Configuração dos pinos
  pinMode(LED_PIN, OUTPUT);
  analogReadResolution(12); // resolução do ADC 12 bits
  analogSetAttenuation(ADC_0db); // Atenuação do ADC 1,1V - 4095
  analogSetPinAttenuation(POT, ADC_11db); // Atenuação do ADC 3,3V - 4095
  delay(10);

  connectWifi();
}

void connectWifi() {
  Serial.println();
  Serial.println();
  Serial.println("\n\nConectando em: ");
  Serial.println(ssid);
  
  // Função de conexão na WiFi
  WiFi.begin(ssid, password);

  // Verificação do status da conexão
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectada...!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  pot = analogRead(POT);
  ldr = analogRead(LDR);
  temp = analogRead(TEMP);
  temperatura = temp*0.02686203; // 110 graus valor máximo dividido pela amostragem

  WiFiLocalWebPageCtrl();
}

void WiFiLocalWebPageCtrl() {
  WiFiClient client = server.available();    // Procura os Clientes

  if (client) {
    Serial.println("Nome cliente.");
    String currentLine = "";                  // Faz uma string para conter os dados de entrada do cliente
    while (client.connected()) {              // Enquanto cliente conectado
      if (client.available()) {               // Ver se há dados
        char c = client.read();               // Ler os dados
        Serial.write(c);                      // Escreve no monitor serial
        if (c == '\n') {                      // Se houver uma nova linha (\n nova linah em ascII)
          // Se a linha atual estiver em branco, você terá dois caracteres de nova
          // esse é o fim da solicitação HTTP do cliente, então envie u ma resposta:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("Valor do potenciometro:    "); //24
            client.print(pot);
            client.print("   <br>");
            client.print("<br>");

            client.print("Valor do LDR:    "); 
            client.print(ldr);
            client.print("   <br>");
            client.print("<br>");

            client.print("Valor do Temperatura:    "); 
            client.print(temperatura);
            client.print(" °C   <br>");
            client.print("<br>");

            client.print("Click <a href=\"/H\">Aqui<a/> para ligar o LED.<br>");
            client.print("Click <a href=\"/L\">Aqui<a/> para desligar o LED.<br>");

            client.println(); // A resposta HTTP termina com outra linha em branco

            break; // sair do loop while
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { // se você obteve qualquer coisa além de um caractere
            currentLine += c;
        }
        // Verifica se a requisição é /H ou /L
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_PIN, HIGH);    // LED ON
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_PIN, LOW);     // LED OFF
        }
      }
    }
    // fechando conexão
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}