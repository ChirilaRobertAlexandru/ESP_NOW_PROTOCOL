#include <Arduino.h>

#define LED1 12
#define LED2 13
#define LED_FLASH 2

void setup() {
  // Setăm pinii ca ieșire
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED_FLASH, OUTPUT);
}

void loop() {
  // Aprindem toate LED-urile
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED_FLASH, HIGH);
  
  delay(1000); // Menținem starea 1 secundă (1000 ms)

  // Stingem toate LED-urile
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED_FLASH, LOW);
  
  delay(1000); // Menținem starea 1 secundă
}