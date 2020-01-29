#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define in1            D1
#define in2            D2
#define in3            D3
#define in4            D4

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

Adafruit_MQTT_Subscribe front = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/front"); // Feeds name should be same everywhere
Adafruit_MQTT_Subscribe left = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/left");
Adafruit_MQTT_Subscribe right = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/right");
Adafruit_MQTT_Subscribe back = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/back");
Adafruit_MQTT_Subscribe stopping = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/stopping");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
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
 
  mqtt.subscribe(&front);
  mqtt.subscribe(&right);
  mqtt.subscribe(&left);
  mqtt.subscribe(&back);
  mqtt.subscribe(&stopping);
}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &front) {
      back->save(LOW);
      right->save(LOW);
      left->save(LOW);
      stopping->save(LOW);
      Serial.print(F("Got: "));
      Serial.println((char *)front.lastread);
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
     
   }
    if (subscription == &left) {
      front->save(LOW);
      back->save(LOW);
      right->save(LOW);
      stopping->save(LOW);
      Serial.print(F("Got: "));
      Serial.println((char *)left.lastread);
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
    }
    if (subscription == &right) {
      Serial.print(F("Got: "));
      Serial.println((char *)right.lastread);
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      digitalWrite(in3,LOW);
      digitalWrite(in4,LOW);
}
    if (subscription == &back) {
      front->save(LOW);
      right->save(LOW);
      left->save(LOW);
      stopping->save(LOW);
      Serial.print(F("Got: "));
      Serial.println((char *)back.lastread);
      digitalWrite(in2,HIGH);
      digitalWrite(in1,LOW);
      digitalWrite(in4,HIGH);
      digitalWrite(in3,LOW);
 
    }
    if (subscription == &stopping) {
      front->save(LOW);
      back->save(LOW);
      right->save(LOW);
      left->save(LOW);
      Serial.print(F("Got: "));
      Serial.println((char *)back.lastread);
      digitalWrite(in2,LOW);
      digitalWrite(in1,LOW);
      digitalWrite(in4,LOW);
      digitalWrite(in3,LOW);
 
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
