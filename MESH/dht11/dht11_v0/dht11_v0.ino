



/*
imre@raspberrypiZ2w3:~/ballonwheel/MESH/dht11/dht11_v0 $ ../../../../bow/arduino-cli lib install "DHT sensor library"

arduino-cli lib list

../../../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano dht11_v0.ino


*/




#include "DHT.h"

#define DHTPIN 2          // Pin where the DHT11 is connected
#define DHTTYPE DHT11     // Define sensor type

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("DHT11 Sensor Reading...");
}

void loop() {
  delay(2000); // Wait a few seconds between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
}

