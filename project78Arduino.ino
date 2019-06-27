#include <SoftwareSerial.h>
#include <FastLED.h>
#include <ArduinoJson.h>
SoftwareSerial s(5,6);
#include "Adafruit_SGP30.h"
static char temperatureCO2[7];
Adafruit_SGP30 sgp;
long now = millis();
long lastMeasureConn = 0;
long lastMeasure = 0;
int neededLedsOn = 0;
int currentLedsOn = 0;
#define LED_PIN     4
#define NUM_LEDS    30
#define pause       300
CRGB leds[NUM_LEDS];
 
void setup() {
s.begin(9600);
FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
}
 
void loop() {
  now = millis();
  if(now-lastMeasure > 1000){
    lastMeasure = now;
    if (! sgp.IAQmeasure()) {
      Serial.println("Measurement failed");
      return;
    }
    float CO2 = sgp.eCO2;
    dtostrf(CO2, 6, 2, temperatureCO2);
      StaticJsonBuffer<1000> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["data1"] = sgp.eCO2;
      root["data2"] = 200;
      if(s.available()>0)
      {
       root.printTo(s);
      }
  }
  checkNeededLeds();
  ledStrip();
}

void ledStrip(){
  
  
  if(neededLedsOn > 0 && neededLedsOn <= 7){
    if(currentLedsOn > neededLedsOn){
      for(int i=currentLedsOn; i>neededLedsOn; i--){
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
    }
    }
    else{
    for(int i=currentLedsOn; i<neededLedsOn; i++){
      leds[i] = CRGB(0, 0, 255);
      FastLED.show();
    }
    }
    currentLedsOn = neededLedsOn;
  }
  
  else if(neededLedsOn > 7 && neededLedsOn <= 17){
    if(currentLedsOn > neededLedsOn){
      for(int i=currentLedsOn; i>neededLedsOn; i--){
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
    }
    }
    else{
    for(int i=currentLedsOn; i<neededLedsOn; i++){
      leds[i] = CRGB(255, 255, 0);
      FastLED.show();
    }
    }
    currentLedsOn = neededLedsOn;
  }
  
  else if(neededLedsOn > 17 && neededLedsOn <= 29){
    if(currentLedsOn > neededLedsOn){
      for(int i=currentLedsOn; i>neededLedsOn; i--){
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
    }
    }
    else{
    for(int i=currentLedsOn; i<neededLedsOn; i++){
      leds[i] = CRGB(255, 0, 0);
      FastLED.show();
    }
    }
    currentLedsOn = neededLedsOn;
  }
  
}
void checkNeededLeds(){
  Serial.println("1");
  
  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  
  if(sgp.eCO2 < 300){
    neededLedsOn = 0;
  }
  else if(sgp.eCO2 > 300 && sgp.eCO2 < 400){
    neededLedsOn = 1;
  }
  else if(sgp.eCO2 > 400 && sgp.eCO2 < 500){
    neededLedsOn = 2;
  }
  else if(sgp.eCO2 > 500 && sgp.eCO2 < 600){
    neededLedsOn = 3;
  }
  else if(sgp.eCO2 > 600 && sgp.eCO2 < 700){
    neededLedsOn = 4;
  }
  else if(sgp.eCO2 > 700 && sgp.eCO2 < 800){
    neededLedsOn = 5;
  }
  else if(sgp.eCO2 > 800 && sgp.eCO2 < 900){
    neededLedsOn = 6;
  }
  else if(sgp.eCO2 > 900 && sgp.eCO2 < 1000){
    neededLedsOn = 7;
  }
  else if(sgp.eCO2 > 1000 && sgp.eCO2 < 1100){
    neededLedsOn = 8;
  }
  else if(sgp.eCO2 > 1100 && sgp.eCO2 < 1200){
    neededLedsOn = 9;
  }
  else if(sgp.eCO2 > 1200 && sgp.eCO2 < 1300){
    neededLedsOn = 10;
  }
  else if(sgp.eCO2 > 1300 && sgp.eCO2 < 1400){
    neededLedsOn = 11;
  }
  else if(sgp.eCO2 > 1400 && sgp.eCO2 < 1500){
    neededLedsOn = 12;
  }
  else if(sgp.eCO2 > 1500 && sgp.eCO2 < 1600){
    neededLedsOn = 13;
  }
  else if(sgp.eCO2 > 1600 && sgp.eCO2 < 1700){
    neededLedsOn = 14;
  }
  else if(sgp.eCO2 > 1700 && sgp.eCO2 < 1800){
    neededLedsOn = 15;
  }
  else if(sgp.eCO2 > 1800 && sgp.eCO2 < 1900){
    neededLedsOn = 16;
  }
  else if(sgp.eCO2 > 1900 && sgp.eCO2 < 2000){
    neededLedsOn = 17;
  }
  else if(sgp.eCO2 > 2000 && sgp.eCO2 < 2250){
    neededLedsOn = 18;
  }
  else if(sgp.eCO2 > 2250 && sgp.eCO2 < 2500){
    neededLedsOn = 19;
  }
  else if(sgp.eCO2 > 2500 && sgp.eCO2 < 2750){
    neededLedsOn = 20;
  }
  else if(sgp.eCO2 > 2750 && sgp.eCO2 < 3000){
    neededLedsOn = 21;
  }
  else if(sgp.eCO2 > 3000 && sgp.eCO2 < 3250){
    neededLedsOn = 22;
  }
  else if(sgp.eCO2 > 3250 && sgp.eCO2 < 3500){
    neededLedsOn = 23;
  }
  else if(sgp.eCO2 > 3500 && sgp.eCO2 < 3750){
    neededLedsOn = 24;
  }
  else if(sgp.eCO2 > 3750 && sgp.eCO2 < 4000){
    neededLedsOn = 25;
  }
  else if(sgp.eCO2 > 4000 && sgp.eCO2 < 4250){
    neededLedsOn = 26;
  }
  else if(sgp.eCO2 > 4250 && sgp.eCO2 < 4500){
    neededLedsOn = 27;
  }
  else if(sgp.eCO2 > 4500 && sgp.eCO2 < 4750){
    neededLedsOn = 28;
  }
  else if(sgp.eCO2 > 4750){
    neededLedsOn = 29;
  }
  Serial.println("2");
}
