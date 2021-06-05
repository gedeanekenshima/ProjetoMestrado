#include <ESP8266WiFi.h>
#define PIN1 D8
#define PIN2 D7
int led = LOW;
unsigned long int tempo_atual;
const char* ssid = "angrybirds";
const char* password = "gedeane9374";

WiFiServer server(80);
int valor1, valor2, valor3, val1;
void setup() {

  Serial.begin(115200);
    delay(10);
  pinMode(PIN1,OUTPUT);
  pinMode(PIN2,OUTPUT);
  //Inicia o WiFi para modo AP (Access Point)
  WiFi.mode(WIFI_AP);
  //Configura o AP
  WiFi.softAP(ssid,password,2,0);

  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

    // Wait until the client sends some data
   unsigned long timeout = millis() + 3000;
  while (!client.available() && millis() < timeout) {
    delay(1);
  }
  
  if (millis() > timeout) {
    Serial.println("timeout");
    client.flush();
    client.stop();
    return;
  }

  // Le string até chegar no enter "\r"
  
  String req = client.readStringUntil('\r'); 
  Serial.println(req);
  client.flush();

  // Match the request
  int val = req.toInt();
  Serial.println(valor1);

  if(valor1 <999 && valor1 >300){
   val1 = 1; 
  }
  else if (val1 <=300){
    val1 = 2;
  }
  else{
    val1 = 0;
  }
  switch(val1){
    case 0: 
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, LOW);
    break;
    
    case 1: 
    digitalWrite(PIN1, HIGH);
    digitalWrite(PIN2, LOW);
    delay(1000-valor1);
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, LOW);
    delay(1000-valor1);
    break;

    case 2:
    digitalWrite(PIN1, HIGH);
    digitalWrite(PIN2, HIGH);
    delay(1000-valor1);
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, LOW);
    delay(1000-valor1);
    break;  

    default:
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, LOW);
  }

}
