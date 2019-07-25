#include <Arduino.h>
#include "ESP8266WiFi.h"

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.softAP("Beacon", "A Blinking Led");
   // Set power lower to better match RSSI vs Actual distance.
  //.platformio\packages\framework-arduinoespressif8266\libraries\ESP8266WiFi\src\ESP8266WiFiGeneric.cpp
  WiFi.setOutputPower(0);
}

void loop()
{
  delay(9000);
}