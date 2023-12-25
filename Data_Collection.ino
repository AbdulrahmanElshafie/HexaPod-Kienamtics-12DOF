/*The providing code is for scan known neetworks and its RSSI whin a given location,

 to feed this data to a Naiev Bayes Model to train on it and then 

 make predction for the location based on the RSSI from known networks. 

 Known networks are network where their location is known*/

#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <Localization_inferencing.h>


String esp1 = "AAA", esp2 = "BBB", esp3 = "CCC";


void scanNetworks(String ap1, String ap2, String ap3) {
 int numNetworks = WiFi.scanNetworks();
 int rssi1 = 0, rssi2 = 0, rssi3 = 0;

  if (numNetworks != 0){
    for (int i = 0; i < numNetworks; ++i)
    {
          
          if(WiFi.SSID(i) == ap1)
            rssi1 = WiFi.RSSI(i);

          if(WiFi.SSID(i) == ap2)
            rssi2 = WiFi.RSSI(i);

          if(WiFi.SSID(i) == ap3)
            rssi3 = WiFi.RSSI(i);

    }
  }
 Serial.print(rssi1);
 Serial.print(" ");
 Serial.print(rssi2);
 Serial.print(" ");
 Serial.print(rssi3);
 Serial.println(" ");
}

void setup() {
  Serial.begin(115200);
  
}



void loop() {

 scanNetworks(esp1, esp2, esp3);
}














