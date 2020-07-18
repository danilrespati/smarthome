#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHTesp.h"
#include <Servo.h>

#define DHTTYPE DHT22
#define DHT_PIN D2    //V0=h, V1=t
#define PIR_PIN D3    //V2=pirState, V3=pirAlert
#define RELAY1_PIN D4 //V4=relayLamp
#define RELAY2_PIN D5 //V5=relayLock
#define RELAY_ON LOW
#define SERVO_PIN D6  //V6=servoPos

BLYNK_WRITE(V4)
{
  int state = param.asInt();
  digitalWrite(RELAY1_PIN, !state);
}

BLYNK_WRITE(V5)
{
  int state = param.asInt();
  digitalWrite(RELAY2_PIN, !state);
}

DHTesp dht;
Servo servo;

char auth[] = "udGkXpgDnwhJ6a81s0JKs_YfEkXPwHh7";
char ssid[] = "PW";
char pass[] = "Password";
unsigned long previousMillis = 0;
long interval = 2000; //min. 2000ms
float h;
float t;
int pirState = LOW;
int pirValue = 0;
int servoPos = 0;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.setup(DHT_PIN, DHTesp::DHTTYPE);
  pinMode(PIR_PIN, INPUT);
  digitalWrite(RELAY1_PIN, !RELAY_ON);
  digitalWrite(RELAY2_PIN, !RELAY_ON);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
}

void loop() {
  Blynk.run();
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
     previousMillis = currentMillis; 
     getDhtValue();
  }
  getPirValue();
}

void getDhtValue() {
  h = dht.getHumidity();
  t = dht.getTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0,h);
  Blynk.virtualWrite(V1,t);
  Serial.print("{\"humidity\": ");
  Serial.print(h);
  Serial.print(", \"temp\": ");
  Serial.print(t);
  Serial.print("}\n");
}

void getPirValue() {
  pirValue = digitalRead(PIR_PIN);  
  if (pirValue == HIGH) {           
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      switchRelay(RELAY1_PIN);
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}

void switchRelay(char relayPin) {
  int state = digitalRead(relayPin);
  digitalWrite(relayPin, !state);
  Serial.println(state);
  Blynk.virtualWrite(V4,state);
}
