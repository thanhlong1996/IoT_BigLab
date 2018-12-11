#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE);
String apiKey = "OB1A7ARSVFCK5510"; 
const char *ssid =  "Jackillian";
const char *pass =  "Password";
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup(void)
{ 
  dht.begin();
  Serial.begin(115200);
  Serial.println("Connecting to ");
  Serial.println(ssid);
 
 
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
  Serial.println("");
  Serial.println("WiFi connected");

  delay(700);

}

String Get(String url, String fp){
    HTTPClient http;
    http.begin(url,fp);
    int httpCode = http.GET();
    String payload = http.getString();
    http.end();
    return(payload);
}

void loop() {
    pinMode(D5,OUTPUT);
    pinMode(D6,OUTPUT);
    pinMode(D8,OUTPUT);
    digitalWrite(D8,HIGH);
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    float l = analogRead(A0);
    String out;
    String l1 = "https://api.thingspeak.com/channels/630223/fields/3/last.txt";
    String l2 = "https://api.thingspeak.com/channels/630223/fields/4/last.txt";
    String f = "https://api.thingspeak.com/channels/630223/fields/5/last.txt";
    String fp = "F9:C2:65:6C:F9:EF:7F:66:8B:F7:35:FE:15:EA:82:9F:5F:55:54:3E";
    if (client.connect(server,80)) 
      {              
         String postStr = apiKey;
         postStr +="&field1=";
         postStr += String(t);
         postStr +="&field2=";
         postStr += String(l);
         postStr += "\r\n\r\n";
 
         client.print("POST /update HTTP/1.1\n");
         client.print("Host: api.thingspeak.com\n");
         client.print("Connection: close\n");
         client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
         client.print("Content-Type: application/x-www-form-urlencoded\n");
         client.print("Content-Length: ");
         client.print(postStr.length());
         client.print("\n\n");
         client.print(postStr);
      }
    
    if (Get(l1,fp) == "1")
    {
      digitalWrite(D5,HIGH);
      h = dht.readHumidity();
     t = dht.readTemperature();
    }
    else
    {
      digitalWrite(D5,LOW);
      h = dht.readHumidity();
      t = dht.readTemperature();
    }
    if (Get(l2,fp) == "1")
    {
      digitalWrite(D6,HIGH);
      h = dht.readHumidity();
      t = dht.readTemperature();
    }
    else
    {
      digitalWrite(D6,LOW);
      h = dht.readHumidity();
      t = dht.readTemperature();
    }
    if (Get(f,fp) == "2"){
      analogWrite(D7,1023);
      h = dht.readHumidity();
      t = dht.readTemperature();
    }
    else{
      if (Get(f,fp) == "1"){
         analogWrite(D7,50);
         h = dht.readHumidity();
         t = dht.readTemperature();
      }
      else{
        analogWrite(D7,0);
        h = dht.readHumidity();
        t = dht.readTemperature();
      }
    }
    
    client.stop();
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.print("C  ");
    Serial.print("light density = ");
    Serial.print(l);
    Serial.println("lux");
    Serial.println("Waiting...");
  delay(2000);
}
