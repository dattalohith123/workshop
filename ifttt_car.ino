#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define mot1            D1
#define mot2            D2
#define mot3            D3
#define mot4            D4

#define WLAN_SSID       "Vyshu "             // Your SSID
#define WLAN_PASS       "Nagaraju"        // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "Attila123"            // Username
#define AIO_KEY         "aio_RjQn06N1uQefT6BA6OLiuK8vTdIZ"   // Auth Key

//WIFI CLIENT
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe motor1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/mot1"); // Feeds name should be same everywhere
Adafruit_MQTT_Subscribe motor2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mot2");
Adafruit_MQTT_Subscribe motor3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mot3");
Adafruit_MQTT_Subscribe motor4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mot4");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(mot1, OUTPUT);
  pinMode(mot2, OUTPUT);
  pinMode(mot3, OUTPUT);
  pinMode(mot4, OUTPUT);
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&motor1);
  mqtt.subscribe(&motor3);
  mqtt.subscribe(&motor2);
  mqtt.subscribe(&motor4);
}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &motor1) {
      Serial.print(F("Got: "));
      Serial.println((char *)motor1.lastread);
      int motor1_State = atoi((char *)motor1.lastread);
      digitalWrite(mot1, motor1_State);
     
    }
    if (subscription == &motor2) {
      Serial.print(F("Got: "));
      Serial.println((char *)motor2.lastread);
      int motor2_State = atoi((char *)motor2.lastread);
      digitalWrite(mot2, motor2_State);
    }
    if (subscription == &motor3) {
      Serial.print(F("Got: "));
      Serial.println((char *)motor3.lastread);
      int motor3_State = atoi((char *)motor3.lastread);
      digitalWrite(mot3, motor3_State);
    }
    if (subscription == &motor4) {
      Serial.print(F("Got: "));
      Serial.println((char *)motor4.lastread);
      int motor4_State = atoi((char *)motor4.lastread);
      digitalWrite(mot4, motor4_State);
      
    }
  }

  
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
