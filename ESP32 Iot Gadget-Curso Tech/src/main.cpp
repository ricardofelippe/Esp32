// In this project, I simply read values from the sensor and printed them in the console.
// The full sketch is in this file.

#include <Arduino.h>
#include "settings.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>



//#define SensorPin_movimento    26  //PINO D25 NO ESP32
int Temperatura = 0;
int LeituraTemp=0;
int SensorHumidade=0;
//int movimento=0;

//DHT Configuration------------------------------

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

#define DHTPIN    25  //PINO D5 NO ESP32

DHT_Unified dht(DHTPIN, DHTTYPE);




void setup() {
  Serial.begin(115200);
  //pinMode(LED_BUILTIN,OUTPUT);
  pinMode(SensorPin_movimento, INPUT);
  
//DHT------------------------------
 dht.begin();
 Serial.println(F("DHTxx Unified Sensor Example"));
 //Print temperature sensor details.
 sensor_t sensor;
 dht.temperature().getSensor(&sensor);
 Serial.println(F("------------------------------------"));
 Serial.println(F("Temperature Sensor"));
 Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
 Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
 Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
 Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
 Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
 Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
 Serial.println(F("------------------------------------"));
 //Print humidity sensor details.
 dht.humidity().getSensor(&sensor);
 Serial.println(F("Humidity Sensor"));
 Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
 Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
 Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
 Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
 Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
 Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
 Serial.println(F("------------------------------------"));
  
  
}

void loop() {
  // PIR Movimento
  sensor_PIR();

  // DHT11
// Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Temperatura=event.temperature;
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
// Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    SensorHumidade=event.relative_humidity;
    
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }

  delay(2000); // Delay between readings.

}

