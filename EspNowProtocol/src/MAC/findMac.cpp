#include <Arduino.h>
#include <WiFi.h>

void setup(){
  Serial.begin(115200);
  
  // Pornim modulul Wi-Fi în modul Station
  WiFi.mode(WIFI_MODE_STA);
  
  // Afișăm adresa MAC în terminal
  Serial.println();
  Serial.print("Adresa MAC a placii este: ");
  Serial.println(WiFi.macAddress());
}

void loop(){
  // Nu este nevoie de nimic în loop
}