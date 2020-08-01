#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define sensor 0 

float  leitura_sensor;


char ssid[] = "***********";  // nome da sua rede wifi
char password[] = "***********";  // senha da sua rede wifi

String key = "*********************************"; // sua key do webhook
String event_name = "Alerta_Gas"; //nome do seu evento

//configurando wifi e sensor
void setup() {

  Serial.begin(115200);
  
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
}

int flag = 0;

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  //leitura do sensor de gas
  leitura_sensor = digitalRead(sensor);
  Serial.println(leitura_sensor);
   
  if(leitura_sensor == HIGH){
   Serial.print("GAS AUSENTE!");
    flag = 0;
   }
  else if(leitura_sensor == LOW && flag == 0){
   flag = 1;
   Serial.print("GAS DETECTADO");
   
   //triggerEvent takes an Event Name and then you can optional pass in up to 3 extra Strings
   HTTPClient http;
   http.begin("http://maker.ifttt.com/trigger/"+event_name+"/with/key/"+key);
   http.GET();
   http.end();
   Serial.print("done");
  }
}
