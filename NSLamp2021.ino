#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#define D1SSID   "WifiSSID"
#define PASSWORD "WifiWachtwoord"
#define GROEN    D1
#define ROOD     D7

void setup() {
 Serial.begin(115200);
 pinMode(ROOD, OUTPUT);
 pinMode(GROEN, OUTPUT);
 digitalWrite(ROOD,LOW);
 digitalWrite(GROEN,LOW);

 WiFi.begin(D1SSID, PASSWORD);
 while (WiFi.status() != WL_CONNECTED) {
  digitalWrite(ROOD,HIGH);
  digitalWrite(GROEN,LOW);
  delay(250);
  digitalWrite(ROOD,LOW);
  digitalWrite(GROEN,HIGH);
  delay(250);
  Serial.println("Verbinden met Wifi...");
 }
 Serial.println("Verbonden met Wifi!");
 Serial.print("IP: ");
 Serial.println(WiFi.localIP());

 digitalWrite(ROOD,LOW);
 digitalWrite(GROEN,LOW);
}

void loop() {
 static long t=millis()-4*60*1000;
 WiFiClientSecure client;
 client.setInsecure();
 if(abs(millis()-t)>4*60*1000) {
  Serial.println("Poll!");
  if(client.connect("marcelv.net", 443)) {
   Serial.println("Connectie");
   client.print(String("GET /IoT/nslamp.php HTTP/1.1\r\n")+
   String("Host: marcelv.net\r\n")+
   String("\r\n"));
   unsigned long timeout = millis();
   while(client.available() == 0) {
    if(millis() - timeout>25000) {
     Serial.println("Error 1");
     Error();
     Serial.println("Time out");
     client.stop();
     return;
    }
   }
   while(client.available()){
    String s = client.readStringUntil('\n');
    Serial.println(s);
    ToonStatus(s);
   }
  }
  t=millis();
 }
}

void ToonStatus(String &s) {
 Serial.println("Status="+s);
 if(s.indexOf("GROEN")<0
  && s.indexOf("ROOD")<0
  && s.indexOf("UIT")<0)
 return;

 digitalWrite(GROEN,LOW);
 digitalWrite(ROOD,LOW);
 if(s.indexOf("GROEN")>=0) digitalWrite(GROEN,HIGH);
 if(s.indexOf("ROOD")>=0) digitalWrite(ROOD,HIGH);
 if(s.indexOf("UIT")>=0) Uit();
}

void Uit() {
 long t=millis();
 while(abs(millis()-t)>1000 * 60 * 10) {
  digitalWrite(GROEN,HIGH);
  digitalWrite(ROOD,LOW);
  delay(200);
  digitalWrite(GROEN,LOW);
  digitalWrite(ROOD,HIGH);
  delay(200);
  digitalWrite(GROEN,LOW);
  digitalWrite(ROOD,LOW);
  delay(3000);
 }
}

void Error() {
 for(int i=0; i<40; i++) {
  digitalWrite(ROOD,HIGH);
  delay(80);
  digitalWrite(ROOD,LOW);
  delay(80);
 }
 digitalWrite(ROOD,HIGH);
}
