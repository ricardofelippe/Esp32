#include <ESP8266WiFi.h>
#include <HTTPSRedirect.h>
#include <DHTesp.h>

//#include <WiFi.h>
//#include <DHT11.h>
//dht11 DHT;


// DHT11 Temperature and Humidity Sensor -----------------------
#include <DHT.h>
#include <DHT_U.h>

 
// Este dado retira-se do SCRIPT na planilha GOOGLE.
const char* GScriptId   = "AKfycbyF6Bq7r180NjNaCKJWZIh05YI5v9mlzbBgvVm8NX6FTDq7GXOc0b3ceJZlyWXt-c90";
//-----------------------------------------------------------------
 
String payload_base =  "{\"command\": \"append_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";
 
const char* host        = "script.google.com";
const int   httpsPort   = 443;
String      url         = String("/macros/s/") + GScriptId + "/exec?cal";
 
 

//#define DHTPIN 2     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

#define DHTPIN    14  //PINO D5 NO ESP32

DHT_Unified dht(DHTPIN, DHTTYPE);
//-----------------------------------------

//PIR Motion Sensor-------------------------------
//Control Variables------------------------
//byte SensorMovimentor=
#define SensorPin_movimento    12  //PINO D6 NO ESP32
byte indicator=13;
int movimento=0;
//-----------------------------------------


 

 
//Objects----------------------------------
HTTPSRedirect* client = nullptr;
//-----------------------------------------
 
 
//Control Variables------------------------
int ID = 0;
int SensorHumidadeTerra = 0;
int Temperatura = 0;
int LeituraTemp=0;
int SensorHumidade=0;
int PercentualHumidadeTerra; // moisture value in percentage
int SensorHumidadeAr;

//-----------------------------------------
 

// BUZZER/LED
// #define buzzer 2 
const char *cellAddress1 = "L2"; //To set moisture value
String url1 = String("/macros/s/") + GScriptId + "/exec?read=" + cellAddress1;
String payload1 = "";

#define PORT_BUZZER  2  // pino D4 no ESP32
# define PIN_RELE_D2 4  // D2
# define PIN2_RELE_D0 16  // D0: este pino ficará HIGH

// RELE+LED

//# define pinReleD2 4  // Pino D2 no ESP32-Este é o sinal(S) que fica ao lado da pinagem + e -

//# define pin2ReleD0 16  // D0: este pino ficará HIGH. Esta é a conexão do meio do bonner com o pino D0




 
void setup()
{
//pinMode(buzzer, OUTPUT);  
//pinMode(pinReleD2, OUTPUT);  
//pinMode(pin2ReleD0,OUTPUT);
//digitalWrite(pin2ReleD0, HIGH);  


     pinMode(PORT_BUZZER, OUTPUT); 
     pinMode(PIN_RELE_D2, OUTPUT);
     pinMode(PIN2_RELE_D0,OUTPUT);
    digitalWrite(PIN_RELE_D2, HIGH);  
  
 


Serial.begin(115200);
//prepareSingleLed();  

pinMode(SensorPin_movimento, INPUT);
pinMode(indicator, OUTPUT);

  
//DHT------------------------------
  dht.begin();
  //Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
//  Serial.println(F("------------------------------------"));
//  Serial.println(F("Temperature Sensor"));
//  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
//  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
//  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
//  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
//  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
//  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
//  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
//  Serial.println(F("Humidity Sensor"));
//  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
//  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
//  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
//  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
//  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
//  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
//  Serial.println(F("------------------------------------"));
  
 
  //WiFi Setup
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){ Serial.print("."); delay(500); }
  Serial.println("[OK]");
 
  //HTTPS Redirect Setup
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  Serial.print("Conectando ao Google...");
 
  bool flag = false;
  for (int i=0; i<5; i++)
  { 
    int retval = client->connect(host, httpsPort);
    if (retval == 1)
    {
       flag = true;
       Serial.println("[OK]");
       break;
    }
    else
      Serial.println("[Error]");
  }
  if (!flag)
  {
    Serial.print("[Error]");
    Serial.println(host);
    return;
  }
  delete client;
  client = nullptr;
}
 
void loop()
{
  ID ++;
  SensorHumidadeTerra = analogRead(0);
  PercentualHumidadeTerra = map(SensorHumidadeTerra,0,1023,0,100); // To display the soil moisture value in percentage LeituraTemp =analogRead(DHTPIN);    // READ DATA
  //LeituraTemp=analogRead(DHTPIN);
  
    
  static bool flag = false;
  if (!flag)
  {
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr) { if (!client->connected()){ client->connect(host, httpsPort); } }
  else { Serial.println("[Error]"); }
   
  //payload = payload_base + "\"" + ID + "," + LeituraSensor + "," + value2 + "," + Humidade + "\"}";
   //payload = payload_base + "\"" + ID + "," + LeituraSensor + "," + value2 + "," + Humidade + "," + movimento+ "\"}";
  payload = payload_base + "\"" + ID + "," + SensorHumidadeTerra + "," + Temperatura + "," + SensorHumidadeAr + "," + movimento + "," + PercentualHumidadeTerra +"\"}";
  
   
  //delay(3600000);
// Delay between measurements.
  // Get temperature event and print its value.
  
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    //Serial.println(F("Error reading temperature!"));
  }
  else {
    //Serial.print(F("Temperature: "));
    Temperatura=event.temperature;
    //Serial.print(event.temperature);
    //Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    //Serial.print(F("Humidity: "));
    SensorHumidade=event.relative_humidity;
    
    //Serial.print(event.relative_humidity);
    //Serial.println(F("%"));
  }

//Serial.println("Enviando...");
  //Serial.print(",\t");
  if(client->POST(url, host, payload)){ 
    //Serial.println(" [OK]"); 
    }
  else { Serial.println("[Error]"); }

// PIR Movimento
  byte state = digitalRead(SensorPin_movimento);
  //digitalWrite(indicator,state);
  if(state == 1)
    {//Serial.println("Somebody is in this area!");
    movimento=1;}
  else if(state == 0){
    //Serial.println("No one!");
    movimento=0;
  }


// RELE

  


  //getData();

  //-----------------
  


// Subroutine for Getting data to Google Sheets
//for (int thisPin = 2; thisPin < 1000; thisPin++) {
  static int connect_count = 0;

  //Serial.print("GET Data from cell 1 soil moisture: ");
  //Serial.println(cellAddress1);
  if (client->GET(url1, host)){
    //get the value of the cell
    payload1 = client->getResponseBody();
    payload1.trim(); //soil moisture set value
    //Serial.println(payload1);
    ++connect_count;
  }
  
// Add some delay in between checks
  //delay(1000);
  delay(2000);
  int moisture = payload1.toInt();
  

 //monitoringSingleLed(moisture); // Acende/apaga Led azul e rele com led vermelho

 if(moisture % 2==0)
{
  digitalWrite(PIN_RELE_D2,LOW);
  digitalWrite(PORT_BUZZER,LOW);
  digitalWrite(PIN2_RELE_D0,LOW);
  
  
  }
else{

  digitalWrite(PORT_BUZZER,HIGH);
  digitalWrite(PIN_RELE_D2,HIGH);
  digitalWrite(PIN2_RELE_D0,HIGH);
  
  
  }





/*if(moisture%2==0)
{
  digitalWrite(pinReleD2,LOW);
  digitalWrite(buzzer,LOW);
  
  }
else{

  digitalWrite(buzzer,HIGH);
  digitalWrite(pinReleD2,HIGH);
  
  }*/



//delay(1);  
  
  
//} // end for
  //--------------------

  delay(10000);  
  
  //delay(10000);  
  //delay(3600000);
  //delay(1800000);

  


}


// Subroutine for Getting data to Google Sheets


void getData() {




//for (int thisPin = 2; thisPin < 1000; thisPin++) {
  static int connect_count = 0;

  //Serial.print("GET Data from cell 1 soil moisture: ");
  //Serial.println(cellAddress1);
  if (client->GET(url1, host)){
    //get the value of the cell
    payload1 = client->getResponseBody();
    payload1.trim(); //soil moisture set value
    //Serial.println(payload1);
    ++connect_count;
  }
  
// Add some delay in between checks
  //delay(1000);
  delay(2000);
  int moisture = payload1.toInt();
  

 //monitoringSingleLed(moisture); // Acende/apaga Led azul e rele com led vermelho

 if(moisture % 2==0)
{
  digitalWrite(PIN_RELE_D2,LOW);
  digitalWrite(PORT_BUZZER,LOW);
  digitalWrite(PIN2_RELE_D0,LOW);
  
  
  }
else{

  digitalWrite(PORT_BUZZER,HIGH);
  digitalWrite(PIN_RELE_D2,HIGH);
  digitalWrite(PIN2_RELE_D0,HIGH);
  
  
  }





/*if(moisture%2==0)
{
  digitalWrite(pinReleD2,LOW);
  digitalWrite(buzzer,LOW);
  
  }
else{

  digitalWrite(buzzer,HIGH);
  digitalWrite(pinReleD2,HIGH);
  
  }*/



//delay(1);  
  
  
//} // end for
} //end fucntion