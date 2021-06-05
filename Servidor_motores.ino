/*
    Sketch utilizado para plataforma ESP-WROOM-02 serrvidor, contendo dois motores de vibração
    Escrito por Gedeane Kenshima (2020)
    Uso com permissão desde que citada a fonte (licença BSD)
    
    Conexões:
    motor de vibração 1: D8 - fio vermelho
    motor de vibração 2: D7 - fio vermelho
    motor de vibração 1 e 2: G - fios pretos   
*/

// Declaração de bibliotecas
#include <ESP8266WiFi.h>

// Pinos positivos dos motores
#define PIN1 D8
#define PIN2 D7

// Variável para contagem de tempo
unsigned long int tempo_atual;

// Dados da rede a ser criada
const char* ssid = "nomedaRede";
const char* password = "senhadaRede";

// Porta da rede criada
WiFiServer server(80);

// Variáveis inteiras
int valor1, valor2, valor3, val1;
void setup() {
  // Velocidade da serial em 115200 bps
  Serial.begin(115200);
  // Aguarda 10 ms
  delay(10);
  // Define pinos dos motores como saída
  pinMode(PIN1,OUTPUT);
  pinMode(PIN2,OUTPUT);
  //Inicia o WiFi para modo AP (Access Point)
  WiFi.mode(WIFI_AP);
  //Configura o AP
  WiFi.softAP(ssid,password,2,0);

  Serial.println("");
  Serial.println("WiFi conectado");

  // Inicia o servidor
  server.begin();
  Serial.println("Servidor iniciado");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Conexão com cliente
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

   // Aguarda até cliente enviar algum dado
   unsigned long timeout = millis() + 3000;
  while (!client.available() && millis() < timeout) {
    delay(1);
  }
  
  // Se o tempo esgotar
  if (millis() > timeout) {
    Serial.println("timeout");
    // Limpa conexão
    client.flush();
    // Interrompe conexão
    client.stop();
    return;
  }

  // Lê string até chegar no enter "\r"
  String req = client.readStringUntil('\r'); 
  Serial.println(req);
  client.flush();

  // Match the request
  int val = req.toInt();
  Serial.println(valor1);
  
  // Se valor recebido da string for maior que 300 e menor que 999
  if(valor1 <999 && valor1 >300){
   val1 = 1; 
  }
  // Se valor recebido da string for menor ou igual que 300
  else if (val1 <=300){
    val1 = 2;
  }
  // Se valor recebido da string não atender as condições acima
  else{
    val1 = 0;
  }
  // Verifica val1
  switch(val1){
    case 0:
      // Motores desligados
      digitalWrite(PIN1, LOW);
      digitalWrite(PIN2, LOW);
      break;
    
    case 1: 
      // Liga motor 1
      // Distância menor que 1m e maior que 300 cm
      digitalWrite(PIN1, HIGH);
      digitalWrite(PIN2, LOW);
      // Aguarda 1000 ms menos o valor1
      delay(1000-valor1);
      digitalWrite(PIN1, LOW);
      digitalWrite(PIN2, LOW);
      delay(1000-valor1);
      break;

    case 2:
      // Liga os dois motores
      // Distância menor ou igual 300 cm
      digitalWrite(PIN1, HIGH);
      digitalWrite(PIN2, HIGH);
      delay(1000-valor1);
      digitalWrite(PIN1, LOW);
      digitalWrite(PIN2, LOW);
      delay(1000-valor1);
      break;  
      
    // Motores desligados
    default:
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, LOW);
  }

} // Fim do código
