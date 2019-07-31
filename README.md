# Estimate-Distance-Wifi
An attempt to estimate distance using wifi based on the 'Estimate Distance Measurement using NodeMCU ESP8266 based on RSSI Technique' paper.  
https://www.researchgate.net/publication/322877438_Estimate_distance_measurement_using_NodeMCU_ESP8266_based_on_RSSI_technique  
A WiFi Beacon, will act as a soft AP. The WiFi scanner will scan for the soft AP and take the average RSSI of the soft AP. Using the average, the formula 'da' will be used to estimate the distance based on the WiFi signal strength.
## How It Works
The beacon will act as a soft AP with the name 'Beacon'. The WiFi scanner will constantly scan for an AP with the name 'Beacon'. It will then take the average RSSI of 'Beacon' 10 times. The average RSSI is used to calculate the estimated distance. An error is subtracted from the estimated distance, based on the average RSSI.  
This estimated distance is then displayed on an OLED screen.  
