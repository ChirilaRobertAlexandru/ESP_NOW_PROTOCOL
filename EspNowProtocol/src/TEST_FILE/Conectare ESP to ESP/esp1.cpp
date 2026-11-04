#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Adresa MAC a plăcii ESP 2
uint8_t adresaDestinatar[] = {0xE0, 0x5A, 0x1B, 0xA7, 0xDC, 0x0C};

String messageBuffer = "";

// Funcție apelată automat când primești un mesaj
void laPrimire(const uint8_t * mac, const uint8_t *datePrimite, int lungime) {
  Serial.print("\n[Mesaj primit]: ");
  for (int i = 0; i < lungime; i++) {
    Serial.print((char)datePrimite[i]);
  }
  Serial.println("\n");
}

// Funcție apelată automat ca să-ți confirme dacă mesajul a ajuns
void laTrimitere(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? " -> [Mesaj livrat cu succes]" : " -> [Eroare la livrare]");
}

void setup() {
  Serial.begin(115200);
  
  // ESP-NOW funcționează doar cu antena Wi-Fi pornită în modul Station
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Eroare la initializarea ESP-NOW");
    return;
  }

  // Înregistrăm funcțiile care ascultă trimiterea/primirea
  esp_now_register_send_cb(laTrimitere);
  esp_now_register_recv_cb(laPrimire);

  // Configurăm „prietenul” (placa țintă)
  esp_now_peer_info_t infoPereche;
  memcpy(infoPereche.peer_addr, adresaDestinatar, 6);
  infoPereche.channel = 0;  
  infoPereche.encrypt = false;
  
  if (esp_now_add_peer(&infoPereche) != ESP_OK){
    Serial.println("Eroare la adaugarea perechii");
    return;
  }
  
  Serial.println("\n--- ESP 1 PREGĂTIT ---");
  Serial.println("Scrie un mesaj si apasa Enter:");
}

void loop() {
  while (Serial.available() > 0) {
    char litera = Serial.read(); 
    
    if (litera == '\n' || litera == '\r') {
      if (messageBuffer.length() > 0) {
        // Trimitem textul adunat
        esp_now_send(adresaDestinatar, (uint8_t *)messageBuffer.c_str(), messageBuffer.length());
        messageBuffer = ""; 
      }
    } else {
      messageBuffer += litera;
      Serial.print(litera); // Ecou local pentru a vedea ce scrii
    }
  }
}