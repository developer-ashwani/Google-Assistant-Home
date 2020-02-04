#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <FirebaseArduino.h> 

// Set firebase credentials.
#define FIREBASE_HOST "Add Firebase Database link"
#define FIREBASE_AUTH "Add Auth Token"

#define WIFI_SSID "Add WiFi Name"
#define WIFI_PASS "Add WiFi Password"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "Add User Name"
#define MQTT_PASS "Add Auth Token"

String status;
String bulb1;
String bulb2;
String fanFirebase;
String nightLightFirebase;
int light1Pin = D1;
int light2Pin = D2;
int fanPin = D7;
int nightLight = D3;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Subscribe allLights = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/AllLights");
Adafruit_MQTT_Subscribe light1 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/light1");
Adafruit_MQTT_Subscribe light2 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/light2");
Adafruit_MQTT_Subscribe fan = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/fan");
Adafruit_MQTT_Subscribe nightlight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/nightlight");
Adafruit_MQTT_Publish LightsStatus = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/LightsStatus");


void setup()
{
  Serial.begin(9600);
  
  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi>");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(">");
    delay(50);
  }

  Serial.println("OK!");

  //Subscribe to the onoff topic
  mqtt.subscribe(&allLights);
  mqtt.subscribe(&light1);
  mqtt.subscribe(&light2);
  mqtt.subscribe(&fan);
  mqtt.subscribe(&nightlight);

  pinMode(light1Pin, OUTPUT);
  pinMode(light2Pin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(nightLight, OUTPUT);
  digitalWrite(light1Pin, HIGH);
  digitalWrite(light2Pin, HIGH);
  digitalWrite(fanPin, HIGH);
  digitalWrite(nightLight, HIGH);

// connecting to firebase.
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Connect/Reconnect to MQTT
  MQTT_connect();

}

void loop()
{
  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {

// configure all Lights.
    
    //If we're in here, a subscription updated...
    if (subscription == &allLights)
    {
      //Print the new value to the serial monitor
      Serial.print("All Light: ");
      Serial.println((char*) allLights.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) allLights.lastread, "ON"))
      {
        //active low logic
        digitalWrite(light1Pin, LOW);
        digitalWrite(light2Pin, LOW);
        digitalWrite(fanPin, LOW);
        digitalWrite(nightLight, LOW);
        Firebase.setString("on", "1");
        Firebase.setString("fanon", "1");
      }
      else if (!strcmp((char*) allLights.lastread, "OFF"))
      {
        digitalWrite(light1Pin, HIGH);
        digitalWrite(light2Pin, HIGH);
        digitalWrite(fanPin, HIGH);
        digitalWrite(nightLight, HIGH);
        Firebase.setString("on", "0");
        Firebase.setString("fanon", "0");
      }
    }

// configure light1.
    
    if(subscription == &light1)
    {
      //Print the new value to the serial monitor
      Serial.print("light1: ");
      Serial.println((char*) light1.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) light1.lastread, "ON"))
      {
        //active low logic
        digitalWrite(light1Pin, LOW);
//        Firebase.setString("bulboneon", "1");
      }
      else if (!strcmp((char*) light1.lastread, "OFF"))
      {
        digitalWrite(light1Pin, HIGH);
        Firebase.setString("on", "0");
//         Firebase.setString("bulboneon", "1");
      } 
    }


// configure light2.

    
     if(subscription == &light2)
    {
         //Print the new value to the serial monitor
      Serial.print("light2: ");
      Serial.println((char*) light2.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) light2.lastread, "ON"))
      {
        //active low logic
        digitalWrite(light2Pin, LOW);
//         Firebase.setString("bulbtwoon", "1");
      }
      else if (!strcmp((char*) light2.lastread, "OFF"))
      {
        digitalWrite(light2Pin, HIGH);
        Firebase.setString("on", "0");
//        Firebase.setString("bulbtwoon", "0");
      } 
      
    }

   
// configure nightlight.

     if(subscription == &nightlight)
    {
         //Print the new value to the serial monitor
      Serial.print("night light: ");
      Serial.println((char*) nightlight.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) nightlight.lastread, "ON"))
      {
        //active low logic
        digitalWrite(nightLight, LOW);
//        Firebase.setString("nightlighton", "1");
      }
      else if (!strcmp((char*) nightlight.lastread, "OFF"))
      {
        digitalWrite(nightLight, HIGH);
        Firebase.setString("on", "0");
//        Firebase.setString("nightlighton", "0");
      } 
      
    }

    
// configure fan.

     if(subscription == &fan)
    {
              //Print the new value to the serial monitor
      Serial.print("fan: ");
      Serial.println((char*) fan.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) fan.lastread, "ON"))
      {
        //active low logic
        digitalWrite(fanPin, LOW);
        Firebase.setString("fanon", "1");
      }
      else if (!strcmp((char*) fan.lastread, "OFF"))
      {
        digitalWrite(fanPin, HIGH);
        Firebase.setString("fanon", "0");
        Firebase.setString("on", "0");
      } 
       
    }

  }
// add Firebase functionality

 // get value
status = Firebase.getString("on");
fanFirebase = Firebase.getString("fanon");
if(status == "1"){
  Serial.println("All ON");
  digitalWrite(light1Pin, LOW);
digitalWrite(light2Pin, LOW);
digitalWrite(nightLight, LOW); 
Firebase.setString("on", "1");
  delay(10);
}if(status == "2"){
 Serial.println("All OFF");
 digitalWrite(light1Pin, HIGH);
digitalWrite(light2Pin, HIGH);
digitalWrite(nightLight, HIGH);
Firebase.setString("on", "0");
}
fanFirebase = Firebase.getString("fanon");
if(fanFirebase == "1"){
  Serial.println("Fan ON");
digitalWrite(fanPin, LOW);
delay(10);
}else{
  Serial.println("Fan OFF");
digitalWrite(fanPin, HIGH);
Firebase.setString("fanon", "0"); 
} 
}

void MQTT_connect()
{
  //  // Stop if already connected
  if (mqtt.connected() && mqtt.ping())
  {
    //    mqtt.disconnect();
    return;
  }

  int8_t ret;

  mqtt.disconnect();

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      ESP.reset();
    }
  }
  Serial.println("MQTT Connected!");
}
