/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
//Pin digital 12 para Trigger do sensor
const int Trigger = D3;
//Pin digital 11 para Echo do sensor
const int Echo = D4; 

#ifndef STASSID
#define STASSID "nomedaRede"
#define STAPSK  "senhadaRede"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.4.1";
const uint16_t port = 80;

void setup() {
  Serial.begin(115200);
  pinMode(Trigger, OUTPUT); //pin como saída
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos o pin com 0

  // Início da conexão WiFi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  // Conecta com servidor
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  // Enquanto conexão não é realizada, aparecem pontos (...) no Monitor Serial 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Mostra dados do servidor
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("conectado a ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Usa classe WiFiClient para criar conexões TCP
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("falha na conexão");
    delay(5000);
    return;
  }

  // This will send a string to the server
  Serial.println("sending data to server");

 
  if (client.connected()) {
    long t; //tempo que demora para chegar em echo
    long d; //distância em centímetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos um pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtemos eco do pulso
  d = (t/59)*10;             //escalamos o tempo a uma distância em cm
  Serial.println(d);
  client.println(d);

  }

  // Aguarda por dados
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(1000);
      return;
    }
  }

  // Lê todas as linhas para responder ao servidor e escreve na Serial
  Serial.println("receiving from remote server");
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Fecha a conexão
  Serial.println();
  Serial.println("conexão fechada");
  client.stop();
} // Fim do código
