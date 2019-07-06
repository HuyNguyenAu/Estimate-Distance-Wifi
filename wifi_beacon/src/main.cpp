#include <Arduino.h>
#include "ESP8266WiFi.h"

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.softAP("Beacon", "A Blinking Led");
}

void loop()
{
  delay(9000);
}