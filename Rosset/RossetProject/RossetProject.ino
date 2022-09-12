#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

//DHT11
#define DHTPIN D4  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
float humidity;
float temperature;
DHT dht(DHTPIN , DHTTYPE);

//solnoide valve
#define WATER D2

//soil moisture sensor
const int soilpin=A0;
int soilvalue=0;
String message;

//Ultrasonic sensor
const int limit=5;
const int trig=D2;
const int echo=D3;
const int led=D6;
float duration;
float distance;

//const int buzzer=4;

ESP8266WebServer server(80);
const char* ssid = "Nour";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);

  //DHT11
  pinMode(DHTPIN , INPUT);
  dht.begin();

  //solnoide valve
  pinMode(WATER , OUTPUT);
  digitalWrite(WATER , LOW);


  //
  pinMode(led,OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  //pinMode(buzzer,OUTPUT);
  
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid , password);
  
  Serial.println("");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected Successfully to: ");
  Serial.println(ssid);
  Serial.println("");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/" , handle_connected);
  server.onNotFound(handle_notfound);

  server.begin();
  dht.begin();
}

void loop() {
  server.handleClient();
}

void handle_connected() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  soilvalue=analogRead(soilpin);

   digitalWrite(trig,LOW);
   delayMicroseconds(5);
   digitalWrite(trig,HIGH);
   delayMicroseconds(5);
   digitalWrite(trig,LOW);
   duration=pulseIn(echo,HIGH);
   distance=duration/29/2;
   Serial.print("Distance:");
   Serial.println(distance);
   if (distance <limit){
    digitalWrite(led,HIGH);
    //digitalWrite(buzzer,HIGH);
   }
   else{
    digitalWrite(led,LOW);
    //digitalWrite(buzzer,LOW);
   }
   //DHT11 sensor
  server.send(200 , "text/html" , sendHTML(humidity , temperature , soilvalue));
  Serial.println("");
  Serial.print("Humidity = ");
  Serial.println(humidity);
  Serial.println("");
  Serial.print("Temperature = ");
  Serial.println(temperature);
  Serial.println("");
  Serial.print("Humidity Of The Plant = ");
  Serial.println(soilvalue);
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
   }
  delay(1000);
}
void handle_notfound() {
  server.send(404 , "text/plain" , "Not Found");
}
String sendHTML(float hum , float temp , float humplant) {
  String ptr;
  ptr = "<!DOCTYPE html>";
  ptr+= "<html lang=\"en\">";
  ptr+= "<head>";
  ptr+= "<meta charset=\"UTF-8\">";
  ptr +="<meta http-equiv=\"refresh\" content=\"3\">";
  ptr+= "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
  ptr+= "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  ptr+= "<title>Rosette</title>";
  ptr+= "<style>";
  ptr+= "body {background-image: url(\"/photo/background.png\");background-attachment: fixed;background-size: contain;padding: 20px;height: 100%;}";
  ptr+= ".container {display: flex;justify-content: space-between;position: relative;top: 30%;width: 100%;}";
  ptr+= ".hu img {width: 206px;height: 140px;}";
  ptr+= ".temp img {width: 220px;height: 130px;}";
  ptr+= ".huP img {width: 300px;}";
  ptr+= ".container div {position: relative;}";
  ptr+= ".container div p {position: absolute;top: 0px;font-size: 30px;font-weight: bold;font-family: cursive;color: #e67e67;width: 130px;background-color: white;padding: 5px;border-radius: 30px;text-align: center;z-index: -1;}";
  ptr+= ".num_hu {left: 40%;}";
  ptr+= ".num_temp {left: 45%;}";
  ptr+= ".num_huP {right: 30%;}";
  ptr+= ".status {position: relative;display: flex;justify-content: flex-start;align-items: center;background-color: #e67e677a;width: 800px;padding: 10px;border-radius: 30px;border: 8px solid #e67e67;margin-left: 20%;border-top-left-radius: 122px 126px;border-bottom-left-radius: 131px 152px;margin-top: 10px;}";
  ptr+= ".status img {width: 300px;}";
  ptr+= "span {content: \"\00B0\";}";
  ptr+= ".status p {font-family: cursive;font-size: 30px;color: #44554a;font-weight: bold;position: absolute;top: 50%;left: 50%;transform: translate(-10%, -100%);}";
  ptr+= ".switch {position: relative;display: inline-block;width: 110px;height: 60px;}";
  ptr+= ".slider {position: absolute;cursor: pointer;top: 0px;left: 0px;right: 0px;bottom: 0px;background-color: #cacaca;transition: 0.5s;border-radius: 34px;}";
  ptr+= ".switch input{display: none;}";
  ptr+= ".slider::before {content: \"\";position: absolute;height: 50px;width: 50px;border-radius: 50%;left: 5px;bottom: 5px;background-color: white;transition: 0.5s;}";
  ptr+= "input:checked + .slider {background-color: #e67e67;}";

//  ptr+= "input:checked + .slider {background-color: #e67e67;";
//  ptr+= digitalWrite(WATER , HIGH);
//  ptr+= "}";
//  ptr+= "input:not(:checked) {background-color: #cacaca;";
//  ptr+= digitalWrite(WATER , LOW);
//  ptr+= "}";
  
  ptr+= "input:checked + .slider::before {transform: translateX(50px);}";
  ptr+= ".swt {position: absolute;top: 15px;left: 50%;}";
  ptr+= "</style>";
  ptr+= "</head>";
  ptr+= "<body>";    
  ptr+= "<div class=\"container\">";
  ptr+= "<div class=\"hu\">";
  ptr+= "<img src=\"/photo/humidity.png\">";
  ptr+= "<p class=\"num_hu\">";
  ptr+= (String)hum;
  ptr+="%</p>";
  ptr+= "</div>";
  ptr+= "<div class=\"temp\">";
  ptr+= "<img src=\"/photo/temprature.png\">";
  ptr+= "<p class=\"num_temp\">";
  ptr+= (String)temp;
  ptr+= "<span>&#176;</span>c</p>";
  ptr+= "</div>";
  ptr+= "<div class=\"huP\">";
  ptr+= "<img src=\"/photo/humidityPlant.png\">";
  ptr+= "<p class=\"num_huP\">";
  ptr+= (String)humplant;
  ptr+= "%</p>";
  ptr+= "</div>";
  ptr+= "</div>";
  ptr+= "<div class=\"status\">";
  ptr+= "<div class=\"swt\">";
  ptr+= "<label class=\"switch\">";
  ptr+= "<input type=\"checkbox\">";
  ptr+= "<span class=\"slider\"></span>";
  ptr+= "</label>";
  ptr+= "</div>";
  ptr+= "<img src=\"E:/Robotec/Rosset/RossetProject/photo/gg.png\">";
  ptr+= "<p>";

    if(humplant<370)
    {
      message="Do Not Need Water";
    }
    else {
      message="Water Your Planet";
    }
    
  ptr+= message;
  ptr+= "</p>";
  ptr+= "</div>";
  ptr+= "</body>";
  ptr+= "</html>";

      return ptr;
}
