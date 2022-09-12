#include<ESP8266WiFi.h>
#include<DHT.h>
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
char name1[]="Eska";
char pass[]="eska#2021";
char host[]="api.thingspeak.com";
 //solnoide valve
 int solnoidepin=4;
//soil moisture sensor
const int soilpin=A0;
const int soilvalue;
//Ultrasonic sesor
const int limit=5;
const int trig=D2;
const int eco=D3;
const int led=D6;
//const int buzzer=4;
float duration;
float distance;

void setup() {
  Serial.begin(115200);
  Serial.println("connecting to ",name1);
  WiFi.begin(name1,pass);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("Connected");
  
  //Ultrasonic
  pinMode(led,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(eco,INPUT);
 // pinMode(buzzer,OUTPUT);
  pinMode(solnoidepin,OUTPUT);
  // DHT11 sensor
  dht.begin();
 
}

void loop() {
  // soil moiture senor
  soilvalue=analogRead(soilpin);
   Serial.println("soil moiture sensor value:");
   Serial.println(soilvalue);
   if(soilvalue>370){
    digitalWrite(led,HIGH);
   }
   else{
    digitalWrite(led,LOW);
   }
   //ultrasonic
   digitalWrite(trig,LOW);
   delayMicroseconds(5);
    digitalWrite(trig,HIGH);
   delayMicroseconds(5);
    digitalWrite(trig,LOW);
   duration=pulseIn(eco,HIGH);
   distance=duration/29/2;
   Serial.println("Distance:");
   Serial.println(distance);
   if (distance <limit){
    digitalWrite(led,HIGH);
    //digitalWrite(buzzer,HIGH);
   }
   else{
    digitalWrite(led,LOW);
   // digitalWrite(buzzer,LOW);
   }
   //DHT11 sensor
   float h=dht.readHumidity();
   float t=dht.readTemperature();
   Serial.println("Humidity:");
   Serial.println(h);
   Serial.println("Temperature:");
   Serial.println(t);
   //IOT
   WiFiClient client;
  String line;

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, 80))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print(String("GET /") + "/channels/1490473/feeds.json?api_key=UUPVISSAIZPR5DYC&results=2" +
                 "HTTP/1.1\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("[Response:]");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        line = client.readString();
        Serial.println(line);
        int x = line.length();
        String y = line.substring(x - 5);
       
        if (y[0] == '1'){
          digitalWrite(solnoidepin , 0);
          Serial.println("water is ON");
        }
        else if (y[0] == '0')
        {
          digitalWrite(solnoidepin , 1);
           Serial.println("water is OFF");
        }
      }
    }

    /*Serial.println(line.charAt(line.length()-3));
      data1 = line.charAt(line.length()-3);*/

    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }
  
  
}
