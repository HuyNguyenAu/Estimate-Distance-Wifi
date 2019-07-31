# Estimate-Distance-Wifi

An attempt to estimate distance using wifi based on the 'Estimate Distance Measurement using NodeMCU ESP8266 based on RSSI Technique' paper. 
 A WiFi Beacon, will act as a soft AP. The WiFi scanner will scan for the soft AP and take the average RSSI of the soft AP. Using the average, the formula 'da' will be used to estimate the distance based on the WiFi signal strength.

