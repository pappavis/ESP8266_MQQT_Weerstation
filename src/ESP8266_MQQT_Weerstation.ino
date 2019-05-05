/*
  20190122 Wemos weerstation door Michiel Erasmus.
 Die programma verbind ana mqqt, publiceert meetdata en terug naar deepsleep.
 wanneer de Wemos in deepsleep is kan jy geen scketch upload. Ontkoppel eerst USB en daarna weer uploaden.

 Wat doet deze sketch;
  - Maakt verbinding met WiFi & MQQT
  - Melden aan MQQT dat de wemos gereboot is.
  - ontvangt enige berichten van mqqt naar wemos, zoals deepSleepinterval.
  - Ga in deepsleep voor de gespcificeerde tijd.

 randvoorwaarde: WiFi, Ubuntu linux / Raspberry Pi Zero met dietpi image. Daarop staat sqlite, mosquitto en node-red geïnstalleerd.
 Je kunt ook Ubuntu gebruiken ipv Pi Zero.

 Om deze kode te gebruiken;
 1. Voldoen aan de software randvoorwaarden.
 2. Importeer onderstaand flows naar Node red. Installeer SQLlite in palette van node-red.
 3. Download arduino bibliotheken zoals hieronder vermeld, en installeer in Arduino IDE.
 4. Download ESP8266 wemos boards uit Arduino boards manager en kies Wemos D1 R1.
 5. Upload deze sketch naar Wemos / ESP8266.
 
 * Node red flows om te deployen;
 // Flow 1. wakker worden Wemos:
 [{"id":"88277a33.f22a58","type":"mqtt in","z":"5a52a983.5025e8","name":"","topic":"deepsleep_hallo_wereld_20190123/hallo","qos":"2","broker":"a6a3041b.8ef2e8","x":247.16668701171875,"y":77.3333511352539,"wires":[["66d547a1.fb2298"]]},{"id":"66d547a1.fb2298","type":"debug","z":"5a52a983.5025e8","name":"","active":true,"console":"false","complete":"payload","x":661.1666831970215,"y":96.66667366027832,"wires":[]},{"id":"90817a94.6fc3e8","type":"mqtt in","z":"5a52a983.5025e8","name":"Wemos is wakker","topic":"deepsleep_hallo_wereld_20190123/reboot/setup","qos":"0","broker":"a6a3041b.8ef2e8","x":124,"y":407.0000009536743,"wires":[["94d05079.7dcf8"]]},{"id":"94d05079.7dcf8","type":"function","z":"5a52a983.5025e8","name":"setdeepSleepInterval","func":"var deepSleepInterval = 60*10;\n\nif(msg.topic == \"deepsleep_hallo_wereld_20190123/reboot/setup\")\n{\n    if(parseInt(msg.payload) == 1)\n    {\n        msg.payload = parseInt(deepSleepInterval);\n    }\n}\n\nreturn msg;\n","outputs":1,"noerr":0,"x":489.00001525878906,"y":358.6666965484619,"wires":[["ca04f79c.e1b9f8","9ec4a980.817dc8"]]},{"id":"ca04f79c.e1b9f8","type":"mqtt out","z":"5a52a983.5025e8","name":"","topic":"deepsleep_hallo_wereld_20190123/deepSleepIntervalSekonden/waarde","qos":"2","retain":"","broker":"a6a3041b.8ef2e8","x":989,"y":316.3333330154419,"wires":[]},{"id":"9ec4a980.817dc8","type":"debug","z":"5a52a983.5025e8","name":"","active":true,"console":"false","complete":"payload","x":820,"y":432.3333578109741,"wires":[]},{"id":"a1ffc9ea.dd7b98","type":"inject","z":"5a52a983.5025e8","name":"inject deepSleepIntervalSekonden","topic":"deepsleep_hallo_wereld_20190123/reboot/setup","payload":"30","payloadType":"str","repeat":"","crontab":"","once":false,"x":198.00001525878906,"y":214,"wires":[["ca04f79c.e1b9f8"]]},{"id":"fba66d89.3e7e3","type":"mqtt in","z":"5a52a983.5025e8","name":"halloWereld","topic":"halloWereld","qos":"2","broker":"a6a3041b.8ef2e8","x":205,"y":537.0000486373901,"wires":[["e49cf77d.a65f18"]]},{"id":"e49cf77d.a65f18","type":"debug","z":"5a52a983.5025e8","name":"","active":true,"console":"false","complete":"payload","x":523,"y":549.3333616256714,"wires":[]},{"id":"f606f3d2.82c61","type":"inject","z":"5a52a983.5025e8","name":"","topic":"weatherstation/bmp1/debug","payload":"1","payloadType":"num","repeat":"","crontab":"","once":false,"x":251.99998474121094,"y":668.0000655651093,"wires":[["6988fcf6.bc5954"]]},{"id":"6988fcf6.bc5954","type":"mqtt out","z":"5a52a983.5025e8","name":"","topic":"weatherstation/bmp1/debug","qos":"2","retain":"","broker":"a6a3041b.8ef2e8","x":694.9999961853027,"y":647.3333578109741,"wires":[]},{"id":"da74b68c.1b7828","type":"inject","z":"5a52a983.5025e8","name":"wakker worden!","topic":"deepsleep_hallo_wereld_20190123/reboot/setup","payload":"true","payloadType":"bool","repeat":"","crontab":"","once":false,"x":137.1666717529297,"y":282.333345413208,"wires":[["94d05079.7dcf8"]]},{"id":"a6a3041b.8ef2e8","type":"mqtt-broker","z":"","broker":"localhost","port":"1883","clientid":"admin@localhost","usetls":false,"compatmode":true,"keepalive":"60","cleansession":true,"willTopic":"","willQos":"0","willPayload":"","birthTopic":"","birthQos":"0","birthPayload":""}]

 // Flow 2, meetdata naar sqlite database.
 
 
*/

// Bibliotheken in gebruik:
// https://github.com/adafruit/Adafruit_BMP085_Unified
// https://github.com/greiman/SSD1306Ascii
// https://github.com/wemos/WEMOS_DHT12_Arduino_Library
// Arduino libs ESP8266WiFi
// arduino libs PubSubClient

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <WEMOS_DHT12.h>
#include <stdio.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

Adafruit_BMP085_Unified bmp1 = Adafruit_BMP085_Unified(10085); //Uses I2C

#define I2C_ADDRESS 0x3C

bool started1 = true;

long duizendste1 = 1000000;  // dit is 1 seconden in deepSleep.
unsigned long deepSleepInterval;  // elke 30 seconden de weersituatie bijwerken.
  
const char* ssid = "Volksrepubliek";
const char* password =  "C0mmodore64!";

const char*   mqttServer = "dietpi";
const int     mqttPort = 1883;
const char*   mqttUser = "admin";
const char*   mqttPassword = "admin";

MDNSResponder mdns;
WiFiClient espClient;
WiFiUDP UDP;
PubSubClient client(espClient);

unsigned long start1 = 0;
unsigned long ulEnd = 5; // wachten 5 seconden

//SSD1306  display(0x3C, 4, 5);
SSD1306AsciiWire oled;
// Define proper RST_PIN if required.
#define RST_PIN -1

// ======================================================================================
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Start wemos deepsleep mqqt"));

  Serial.println("HelloWorld2");
  Serial.println("");
  //------------------------------------------------------------- einde verificatie of SerialMonitor goed doorkomt!  
  
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(System5x7);
  oled.setScrollMode(SCROLL_MODE_AUTO);

  if (bmp1.begin()) //Begin communication over I2C
  {
    oled.println("BMP180 verbonden.");
  }

  if(1>2)
  {
    Serial.println("BME180 niet verbonden.");
    oled.println("BME180 niet verbonden.");
  }

  // ********* Wifi access point
  Serial.print("Configuring access point...");
  // You can remove the password parameter if you want the AP to be open.
  // WiFi.softAP(ssid_ap, password_ap);

  // IPAddress myIP = WiFi.softAPIP();
  // Serial.print("AP IP address: ");
  // Serial.println(myIP);
  // oled.print("AP IP address: ");
  // oled.println(myIP);
  
  Serial.println("");
  // Wait for connection
  verbindWifi();
   // ********* Eind Wifi access point
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mQQTcallback);  

  verbindMQQT();

  oled.println(String("MQQT=" + String(mqttServer) ));
  client.publish("halloWereld", "Wemos is wakker geword!" );
  client.publish("deepsleep_hallo_wereld_20190123/reboot/setup", "1" );
}


// ======================================================================================

void loop() {
  verbindMQQT();

  String strTydTot1 = "Tyd tot pub: ";
  long intTydTot1 = millis() + 1000;

  
      publishBME(0);
      publishBME(1);
      publishBME(2);
      publishBME(3);
      publishBME(4);
  

  while (millis() < intTydTot1) {
    delay(1); // enable Modem-Sleep   https://forum.arduino.cc/index.php?topic=575354.0
    client.loop();
  }

  if (deepSleepInterval == NULL) {
    deepSleepInterval = 5;
  }

  Serial.print("Ek gaan slaap vir ");
  Serial.print(deepSleepInterval);
  Serial.println(" sekonde.");
  // delay(deepSleepInterval * 1000);

  ESP.deepSleep(deepSleepInterval * duizendste1);
}


void mQQTcallback(char* topic, byte* payload, unsigned int length) {

    Serial.print("Message arrived in topic: ");
    Serial.println(String(topic));
    
    oled.println();
    oled.print("Mqqt msg in topic:");
    oled.println(topic);
  
  if ( strcmp(topic, "deepsleep_hallo_wereld_20190123/deepSleepIntervalSekonden/waarde") == 0 ){
    payload[length] = '\0'; // Make payload a string by NULL terminating it.
    deepSleepInterval = atoi((char *)payload);
    // deepSleepInterval = 12;
    Serial.print("nuwe deepSleepInterval=");
    Serial.println(deepSleepInterval);
  }

  if (strcmp(topic, "deepsleep_hallo_wereld_20190123/bmp1/debug") == 0 ){
    payload[length] = '\0'; // Make payload a string by NULL terminating it.
    if(String((char *)payload) == "1") {
      displaySensorDetails();
    }
  }
    
  Serial.print("topic:\t");
  Serial.print(topic);
  Serial.print("\tMessage:");
  for (int i = 0; i < length; i++) {
    char bericht1 = (char)payload[i];
    Serial.print(bericht1);
    oled.println(bericht1);
  }
 
  Serial.println();
  Serial.println("-----------------------");
  oled.println("-----------------------"); 
}

void publishBME(int whatPubliceren)
{

  DHT12 dht12;
  float temperature;
  float h = 0.0;
  float t = 0.0;

  switch(whatPubliceren)
  {
    case 0:
      float temperature;
      bmp1.getTemperature(&temperature);
      t = temperature;
      break;
    case 1:
      if(dht12.get()==0) {
        t = dht12.humidity;
      }
      break;
    case 2:
      sensors_event_t event1;
      bmp1.getEvent(&event1);
    
     if (event1.pressure) {
       float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
       t = bmp1.pressureToAltitude(seaLevelPressure, event1.pressure) * 1000;
     }
     break;
    case 3:
      t = millis();
      break;
    case 4:
      t = deepSleepInterval;
      break;
    default:
      break;
  }

  // Use String to convert the float to a string - theirs probably a better way but this is easy.
  String temperatureString = String(t);          
  int length = temperatureString.length();
  char waarde1[length];          
  temperatureString.toCharArray(waarde1, temperatureString.length());   // Convert the string to char[]

  switch(whatPubliceren)
  {
    case 0:
      client.publish("weatherStation/DHT/temperature", waarde1 );
      oled.print("pub temp: ");
      break;
    case 1:
      client.publish("weatherStation/DHT/humidity", waarde1 );
      oled.print("pub humidity: ");
      break;
    case 2:
      client.publish("weatherStation/DHT/luchtdruk", waarde1 );
      oled.print("pub luchtdruk: ");
      break;
    case 3:
      client.publish("deepsleep_hallo_wereld_20190123/DHT/millis", waarde1 );
      // oled.print("pub millis: ");
      // Serial.print("millis:");
      break;
    case 4:
      client.publish("weatherStation/deepSleepInterval", waarde1 );
      oled.print("pub deepSleepInterval: ");
      Serial.print("pub deepSleepInterval: ");
      Serial.println(deepSleepInterval);
      break;      
    default:
      break;
  }

  Serial.print(" waarde ");
  Serial.println(waarde1);
  oled.println(waarde1);
}


void verbindMQQT()
{
  while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
      String mqqtCon1 = String("verbind MQTT ") + String(mqttServer);
      oled.println(mqqtCon1);

      if (client.connect("ESP8266Client_stapel2", mqttUser, mqttPassword )) { 
        Serial.println("connected");     
        oled.println("MQQT verbonden");
        delay(10);
        
        client.subscribe("deepsleep_hallo_wereld_20190123/deepSleepIntervalSekonden/waarde");
        client.subscribe("deurbel/relay/status");
        client.subscribe("deepsleep_hallo_wereld_20190123/bmp1/debug");
        client.subscribe("SN76489/mmlMusic");
        
      } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        oled.println("MQQT geen verbinding");
        delay(5000);  
      }
  }  
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp1.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


void verbindWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  UDP.begin(8888);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    oled.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  oled.print("lokale ");
  Serial.print(ssid);
  oled.println(ssid);
  Serial.print(" IP addres: ");
  oled.print("IP: ");
  String strlokaleIP = String(WiFi.localIP());
  Serial.print(strlokaleIP);
  oled.println(strlokaleIP);

  if (mdns.begin("esp8266", WiFi.localIP())) {
   Serial.println("MDNS responder started");
   oled.println("MDNS responder started");
  }
  
}
