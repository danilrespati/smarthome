#include "DHTesp.h"

#define DHT_PIN D2
#define DHTTYPE DHT22

DHTesp dht;
 
void setup() {
    Serial.begin(9600);
    dht.setup(DHT_PIN, DHTesp::DHTTYPE);
}
 
void loop() {
    float h = dht.getHumidity();
    float t = dht.getTemperature();
 
    Serial.print("{\"humidity\": ");
    Serial.print(h);
    Serial.print(", \"temp\": ");
    Serial.print(t);
    Serial.print("}\n");
 
    delay(2000);
}
