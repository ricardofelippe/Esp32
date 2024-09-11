void sensor_dht11(){
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
  }