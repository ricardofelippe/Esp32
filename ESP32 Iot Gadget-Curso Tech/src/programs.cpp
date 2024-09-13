#include <Arduino.h>
#include "settings.h"

void sensor_PIR(){
byte state = digitalRead(SensorPin_movimento);
  //digitalWrite(indicator,state);
  if(state == 1)
    {
    Serial.println("Somebody is in this area!");
    //movimento=1;
    }
  else if(state == 0){
    Serial.println("No one!");
    //movimento=0;
  }
  }