#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ESP8266WiFi.h"

const char beacon[7] = "Beacon"; 

const uint16_t SCREEN_WIDTH = 128;
const uint16_t SCREEN_HEIGHT = 64;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const uint8_t size = 17;
const float_t average_errors[size] = {0.01, 0.0258, 0.0971, 0.1277, 0.0239, 0.0236, 0.0127, 0.1293, 0.4171, 0.0693, 0.1613, 0.1232, 0.028, 0.0137, 70.3118, 70.9605, 2.1052};
const int8_t rssis[size] = {-55, -59, -60, -62, -66, -68, -69, -71, -74, -79, -81, -83, -84, -85, -86, -87, -88};

float AverageError(const int &rssi)
{
  int rssis_difference = 0;
  int matched_index = 0;

  if (rssi > rssis[0])
  {
    return average_errors[0];
  }
  else if (rssi < rssis[size - 1])
  {
    return average_errors[size - 1];
  }

  for (uint8_t i = 0; i < size - 1; i++)
  {
    const int8_t current_rssis = rssis[i];
    const int8_t next_rssis = rssis[i + 1];
    const int16_t current_rssis_difference = abs(rssi - current_rssis);
    const int16_t next_rssis_difference = abs(next_rssis - rssi);

    if (current_rssis_difference >= next_rssis_difference)
    {
      if (i == 0)
      {
        rssis_difference = next_rssis_difference;
        matched_index = i + 1;
      }
      else if (next_rssis_difference < rssis_difference)
      {
        rssis_difference = next_rssis_difference;
        matched_index = i + 1;
      }
    }
    else
    {
      if (i == 0)
      {
        rssis_difference = current_rssis_difference;
        matched_index = i;
      }
      else if (current_rssis_difference < rssis_difference)
      {
        rssis_difference = current_rssis_difference;
        matched_index = i;
      }
    }
  }
  return average_errors[matched_index];
}

// Calculate the estimated distance based on RSSI.
float DistanceFromRSSI(const int8_t &rssi)
{
  return (-0.043 * pow(rssi, 5) - 4.92 * pow(rssi, 4) - 171.5 * pow(rssi, 3) - 600.8 * pow(rssi, 2) + (41.41 * rssi) - 0.84) / (pow(rssi, 4) + 250.4 * pow(rssi, 3) + 14780 * pow(rssi, 2) - (455.9 * rssi) + 12.24);
}

void ClearDisplay()
{
  display.clearDisplay();
  display.setCursor(0, 0);
}

void SetupDisplay()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
  }
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  ClearDisplay();
}

void SetupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void setup()
{
  SetupWifi();
  SetupDisplay();
}

void loop()
{
  const uint8_t networks = WiFi.scanNetworks();

  if (networks == 0)
  {
    ClearDisplay();
    display.println(F("Nothing found."));
  }
  else
  {
    for (uint8_t i = 0; i < networks; i++)
    {
      if (strcmp(WiFi.SSID(i).c_str(), beacon) == 0)
      {
        ClearDisplay();
        display.println(F("SSID: "));
        display.println(WiFi.SSID(i));
        display.println(F("RSSI: "));
        display.print(WiFi.RSSI(i));
        display.println(F(" dBm"));
        display.println(F("Estimated Distance: "));
        display.print(DistanceFromRSSI(WiFi.RSSI(i)) - AverageError(WiFi.RSSI(i)));
        display.println(F(" m"));
        display.display();
      }
    }
  }
  delay(500);
}