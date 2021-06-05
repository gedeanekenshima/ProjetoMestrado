/*
    Sketch utilizado para plataforma ESP-WROOM-02 cliente 2, contendo três sensores VL53L0X (TOF)
    Escrito por Gedeane Kenshima (2020)
    Uso com permissão desde que citada a fonte (licença BSD)
    
    Conexões:
    XSHUT sensor 1: D7
    XSHUT sensor 2: D6
    XSHUT sensor 3: D5
    SDA (todos): D2
    SCL (todos): D1
    VCC (todos): 5V
    GND (todos): G
    
*/

// Declaração de bibliotecas
#include <ESP8266WiFi.h>
#include <Wire.h>
// Necessária instalação desta biblioteca
#include <VL53L0X.h>

// Declaração sensores
VL53L0X sensor;
VL53L0X sensor2;
VL53L0X sensor3;

// Variáveis inteiras
int a;
int b;
int c;

// Pinos para sensores
const int xshut1 = D7, xshut2 = D6, xshut3 = D5;

// Dados da rede que o cliente vai se conectar
#ifndef STASSID
#define STASSID "nomedaRede"
#define STAPSK  "senhadaRede"
#endif

// Atribuição de dados da rede
const char* ssid     = STASSID;
const char* password = STAPSK;

// Endereço que o cliente vai se conectar
const char* host = "192.168.4.1";
// Número da porta
const uint16_t port = 80;

void setup() {
  Wire.begin();
  // Velocidade de comunicação em 115200 bps
  Serial.begin(115200);
  
  // Configuração dos pinos xshut dos sensores como saídas
  pinMode(xshut1, OUTPUT);
  pinMode(xshut2, OUTPUT);
  pinMode(xshut3, OUTPUT);
  
  // Desliga todos os pinos xshut
  digitalWrite(xshut1, LOW);
  digitalWrite(xshut2, LOW);
  digitalWrite(xshut3, LOW);
  
  // Liga primeiro sensor
  digitalWrite(xshut1, HIGH);
  // Aguarda 150 ms
  delay(150);
  // Escreve na serial
  Serial.println("00");
  // Inicializa sensor
  sensor.init(true);
  // Escreve na serial
  Serial.println("01");
  delay(100);
  sensor.setAddress((uint8_t)01);
  // Escreve na serial
  Serial.println("02");
  // Liga segundo sensor
  digitalWrite(xshut2, HIGH);
  delay(150);
  
   sensor2.init(true);
  // Escreve na serial
  Serial.println("03");
  delay(100);
  sensor2.setAddress((uint8_t)02);
  // Escreve na serial
  Serial.println("04");
  // Liga terceiro sensor
  digitalWrite(xshut3, HIGH);
  delay(150);
  sensor3.init(true);

  sensor.startContinuous();
  sensor2.startContinuous();
  sensor3.startContinuous();
  
  // Realiza conexão com servidor
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // Enquanto conexão não é realizada, aparecem pontos (...) no Monitor Serial
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

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
    // Se conexão falhar, aparece mensagem de erro
    Serial.println("Falha na conexão");
    delay(5000);
    return;
  }

  // Envio de dados dos sensores
  Serial.println("Envio de dados ao servidor");

  // Se houver conexão, acontece a leitura de distância de cada sensor (em mm)
  if (client.connected()) {
    int leitura1 = sensor.readRangeContinuousMillimeters();
    int leitura2 = sensor2.readRangeContinuousMillimeters();
    int leitura3 = sensor3.readRangeContinuousMillimeters();
    int d1 = (leitura1);
    int d2 = (leitura2);
    int d3 = (leitura3);
  
    // Mostra na serial os valores das leituras
    Serial.println(d1);
    Serial.println(d2);
    Serial.println(d3);
    // Se a primeira leitura for menor que 1000 mm
    if (d1 <1000){
       // Envio da distancia d1
       client.println(d1);
     }
     // Se a segunda leitura for menor que 1000 mm
     if (d2 <1000){
       // Envio da distancia d2
       client.println(d2);
     }
     // Se a terceira leitura for menor que 1000 mm
     if (d3 <1000){
       // Envio da distancia d3
       client.println(d3);
     }
  
  }

  // Aguarda por dados de conexão disponíveis
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(1000);
      return;
    }
  }

  // Lê todas as linhas para responder ao servidor e escrever tudo na Serial
  Serial.println("receiving from remote server");
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Fecha a conexão
  Serial.println();
  Serial.println("Conexão fechada");
  client.stop();
} // Fim do código
