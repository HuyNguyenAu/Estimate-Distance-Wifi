#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ESP8266WiFi.h"

const char beacon[7] = "Beacon";

Adafruit_SSD1306 display(128, 64, &Wire, -1);

const uint8_t size = 17;
const float_t errors[size] = {0.01, 0.0258, 0.0971, 0.1277, 0.0239, -0.0236, -0.0127, -0.1293, 0.4171, 0.0693, 0.1613, -0.1232, 0.028, -0.0137, -0.3118, -0.9605, -2.1052};
const int8_t rssis[size] = {-55, -59, -60, -62, -66, -68, -69, -71, -74, -79, -81, -83, -84, -85, -86, -87, -88};

// Match the rssi to an error.
float_t Error(const int32_t &rssi)
{
  int32_t rssis_difference = 0;
  int8_t matched_index = 0;

  // If the rssi is larger then -55 dbm, return the first error.
  if (rssi > rssis[0])
  {
    return errors[0];
  } 
  // If the rssi is smaller than -88 dbm, return the last error.
  else if (rssi < rssis[size - 1])
  {
    return errors[size - 1];
  }

  // Loop through each rssis and find the rssis that is closest.
  // Take that index, can use it to find the error.
  for (uint8_t i = 0; i < size - 1; i++)
  {
    const int8_t current_rssis = rssis[i];
    const int8_t next_rssis = rssis[i + 1];
    const int16_t current_rssis_difference = abs(rssi - current_rssis);
    const int16_t next_rssis_difference = abs(next_rssis - rssi);

    // If the rssi is closer to the rssi when are on.
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
    // The rssi is closer to the next rssi.
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
  return errors[matched_index];
}

// Estimated the distance based on RSSI given.
float_t DistanceFromRSSI(const int32_t &rssi)
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

void SetupWifi()
{
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
    int32_t average_rssi = 0;

    for (uint8_t i = 0; i < 10; i++)
    {
      for (uint8_t i = 0; i < networks; i++)
      {
        if (strcmp(WiFi.SSID(i).c_str(), beacon) == 0)
        {
          average_rssi += WiFi.RSSI(i);
        }
      }
    }

    average_rssi /= 10;
    ClearDisplay();
    display.println(F("SSID: "));
    display.println(beacon);
    display.println(F("RSSI: "));
    display.print(average_rssi);
    display.println(F(" dBm"));
    display.println(F("Estimated Distance: "));
    display.print(DistanceFromRSSI(average_rssi) + Error(average_rssi));
    display.println(F(" m"));
    display.display();
  }
  delay(500);
}