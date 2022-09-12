#include<ESP8266WiFi.h>
#include<Blynk.h>
//#include <Adafruit_Sensor.h>
//#include<SPI.h>
#include<DHT.h>// for DHT11 sensor
#include<BlynkSimpleEsp8266.h>
//#include<version.h>
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
char name1[]="Eska";
char pass[]="eska#2021";
char host[]="sYwqDenX17QOYl8vOna0LbaQGZpIIq60";
 //solnoide valve
// int solnoidepin=4;
//soil moisture sensor
const int soilpin=A0;
 int soilvalue=0;
 int value=0;
//Ultrasonic sesor
const int limit=30;
const int trig=D2;
const int eco=D3;
const int led=D6;
// const int buzzer=4;
float duration;
float distance;

void setup() {
  Serial.begin(115200);
  Blynk.begin(host,name1,pass);
  Serial.println("connecting to ");
  Serial.println(name1);
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
//  pinMode(buzzer,OUTPUT);
 // pinMode(solnoidepin,OUTPUT);
  //DHT11 sensor
  dht.begin();
  
 
}

void loop() {
// Blynk.run();
  // soil moiture senor
  soilvalue=analogRead(soilpin);
   Serial.println("soil moiture sensor value:");
   Serial.println(soilvalue);
   Blynk.virtualWrite(V2,soilvalue);
  // if(soilvalue>&& value==0){
  //  Serial.println("water your planet");
  //  Blynk.notify("Water your planet");//send notification
  //  value=1;
   // delay(500);
   //}
  // else if(soilvalue==1&&value==1){
   // Serial.println("Planet has not been watered yet");
   // delay(500);
   //}
  // else{
  //  Serial.println("Planet does not need water");
  //  value=0;
  //  delay(500);
  // }
   
   //ultrasonic
   digitalWrite(trig,LOW);
   delayMicroseconds(5);
    digitalWrite(trig,HIGH);
   delayMicroseconds(5);
    digitalWrite(trig,LOW);
   duration=pulseIn(eco,HIGH);
   distance=duration/29/2;
   Serial.println("");
   Serial.print("Distance:");
   Serial.println(distance);
   if (distance <limit){
    digitalWrite(led,LOW);
   // digitalWrite(buzzer,HIGH);
   }
   else{
    digitalWrite(led,HIGH);
   // digitalWrite(buzzer,LOW);
   }
   //DHT11 sensor
   float hum=dht.readHumidity();
   float temp=dht.readTemperature();
   if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
   }// for 
   Serial.println("Humidity:");
   Serial.println(hum);
   Serial.println("Temperature:");
   Serial.println(temp);
   Blynk.virtualWrite(V5,hum);
    Blynk.virtualWrite(V6,temp);
    /* solnoidevalve
    if(Blynk.digitalRead((D0)==1)){
     digitalWrite(solnoidepin,LOW);
      Serial.println("water is ON");
   }
    else if(Blynk.digitalRead((D0)==0)){
     digitalWrite(solnoidepin,HIGH);
      Serial.println("water is OFF");
    }
    */


    
     
   
}
