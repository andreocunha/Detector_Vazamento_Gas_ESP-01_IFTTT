#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define sensor 2 

float  leitura_sensor;

DHT dht(0, DHT11);


char ssid[] = "***********";  // nome da sua rede wifi
char password[] = "***********";  // senha da sua rede wifi

String key = "*********************************"; // sua key do webhook
String event_name1 = "Alerta_Gas"; //nome do evento para o gás
String event_name2 = "Alerta_Fogo"; //nome do seu para a temperatura

//configurando wifi e sensor
void setup() {

  Serial.begin(115200);
  dht.begin();
  pinMode(sensor, INPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(250);

  // tentativa de se conectar ao wifi:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  delay(5000);
}

int flagGas = 0;
int flagFogo = 0;
float t = 0;

void sendAlarmGas(){
  flagGas = 1;
   Serial.print("GAS detectado");

   HTTPClient http;
   http.begin("http://maker.ifttt.com/trigger/"+event_name1+"/with/key/"+key);
   http.GET();
   http.end();
   Serial.print("done");
}

void sendAlarmFogo(){
  flagFogo = 1;
   Serial.print("FOGO detectado");

   HTTPClient http;
   http.begin("http://maker.ifttt.com/trigger/"+event_name2+"/with/key/"+key);
   http.GET();
   http.end();
   Serial.print("done");
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2500);

  //leitura do sensor de gas
  leitura_sensor = digitalRead(sensor);
//  Serial.println(leitura_sensor);
   
  if(leitura_sensor == HIGH){
  //GAS AUSENTE
    flagGas = 0;
  }
  else if(leitura_sensor == LOW && flagGas == 0){
    sendAlarmGas();
  }

  //leitura do sensor de temperatura
  t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if ( isnan(t) ) {
    Serial.println(F("Falha ao ler a temperatura do sensor DHT!"));
    return;
  }

  if(t >= 40 && flagFogo == 0){
    sendAlarmFogo();
  }else if(t < 40){
    flagFogo = 0;
  }
}
