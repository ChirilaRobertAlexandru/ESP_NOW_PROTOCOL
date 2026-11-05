#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Adresa MAC a placii ESP 2 (cea cu display-ul)
uint8_t adresaDestinatar[] = {0xE0, 0x5A, 0x1B, 0xA7, 0xDC, 0x0C};
String messageBuffer = "";

void laTrimitere(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println(" -> [Comanda trimisa cu succes]");
  } else {
    Serial.println(" -> [Eroare la trimitere]");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Eroare initializare ESP-NOW");
    return;
  }

  esp_now_register_send_cb(laTrimitere);

  esp_now_peer_info_t infoPereche;
  memset(&infoPereche, 0, sizeof(infoPereche)); 
  infoPereche.ifidx = WIFI_IF_STA;              
  memcpy(infoPereche.peer_addr, adresaDestinatar, 6);
  infoPereche.channel = 0;  
  infoPereche.encrypt = false;
  
  if (esp_now_add_peer(&infoPereche) != ESP_OK){
    Serial.println("Eroare la adaugarea perechii");
    return;
  }
  
  Serial.println("\n--- ESP 1 (TELECOMANDA) PREGATIT ---");
  Serial.println("Scrie formatul: <LED> <Clipiri> <Ton> <Toff> (ex: 3 5 0.4 0.6)");
}

void loop() {
  while (Serial.available() > 0) {
    char litera = Serial.read(); 
    if (litera == '\n' || litera == '\r') {
      if (messageBuffer.length() > 0) {
        messageBuffer.trim();
        Serial.print("Trimit comanda: ");
        Serial.print(messageBuffer);
        esp_now_send(adresaDestinatar, (uint8_t *)messageBuffer.c_str(), messageBuffer.length());
        messageBuffer = ""; 
      }
    } else {
      messageBuffer += litera;
      Serial.print(litera);
    }
  }
}