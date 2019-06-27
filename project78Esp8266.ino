String lokaal = "1";
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <dht.h>
#define DHT11_PIN 13
#include "Adafruit_SGP30.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial s(D6,D5);
int data1;
static char temperatureCO2[7];
int neededLedsOn = 0;
bool firstConnect = true;
Adafruit_SGP30 sgp;


// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "S9_bas";
const char* password = "12345678";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "test.mosquitto.org";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);



// Initialize DHT sensor.
dht DHT;

// Timers auxiliar variables
long now = millis();
long lastMeasureConn = 0;
long lastMeasure = 0;

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void arduinoConnect() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
  data1=root["data1"];
  Serial.print("CO2: ");
  Serial.println(data1);
}

void pubValues(){

    int chk = DHT.read11(DHT11_PIN);
    
    float temp = DHT.temperature;
    static char temperatureTemp[7];
    dtostrf(temp, 6, 2, temperatureTemp);
    
    float hum = DHT.humidity;
    static char temperatureHum[7];
    dtostrf(hum, 6, 2, temperatureHum);
    
    float CO2 = data1;
    dtostrf(CO2, 6, 2, temperatureCO2);
    
    client.publish("classrooms/1/temperature", temperatureTemp, true);
    client.publish("classrooms/1/humidity", temperatureHum, true);
    client.publish("classrooms/1/carbondioxide", temperatureCO2, true);

    Serial.print("Humidity: ");
    Serial.println(temperatureHum);
    Serial.print(" %\t Temperature: ");
    Serial.println(temperatureTemp);
    Serial.print("Carbondioxide: ");
    Serial.println(temperatureCO2);
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  Wire.begin(4, 5);//(scl, sda)
  Serial.begin(9600);
  s.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  while (!Serial) continue;

  pinMode(D7,INPUT);

}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop() {
  //first time running connect to mqtt
  if(firstConnect == true){
    if (!client.connected()) {
    reconnect();
    }
    if(!client.loop())
    client.connect("ESP8266Client");
    firstConnect = false;
  }

  // after that every 50 seconds
  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now - lastMeasureConn > 7000) {
    lastMeasureConn = now;
    if (!client.connected()) {
    reconnect();
    }
    if(!client.loop())
    client.connect("ESP8266Client");

    pubValues();
  }
  else{
    arduinoConnect();
  }
  //if (now - lastMeasure > 500) {
   // lastMeasure = now;
    
  //}  
} 
