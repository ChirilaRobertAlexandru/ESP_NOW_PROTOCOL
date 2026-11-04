#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_SDA 14
#define I2C_SCL 15
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LED1 12
#define LED2 13
#define LED3 2

String messageBuffer = "";

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Eroare la initializarea SSD1306"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Astept un numar...");
  display.display();
  
  Serial.println("\nIntrodu un numar (ex: 3) si apasa Enter:");
}

void loop() {
  while (Serial.available() > 0) {
    char incomingChar = Serial.read(); 
    
    // Verificăm dacă ai apăsat Enter
    if (incomingChar == '\n' || incomingChar == '\r') {
      if (messageBuffer.length() > 0) {
        
        // Transformăm textul stocat în număr întreg
        int numarBlinkuri = messageBuffer.toInt(); 

        if (numarBlinkuri > 0) {
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Numar primit: ");
          display.println(numarBlinkuri);
          display.println("Clipesc...");
          display.display();

          Serial.print("\nAm primit numarul: ");
          Serial.println(numarBlinkuri);

          for (int i = 0; i < numarBlinkuri; i++) {
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, HIGH);
            digitalWrite(LED3, HIGH);
            delay(500); 

            digitalWrite(LED1, LOW);
            digitalWrite(LED2, LOW);
            digitalWrite(LED3, LOW);
            delay(500); 
          }

          display.clearDisplay();
          display.setCursor(0, 10);
          display.println("Gata!");
          display.println("Astept alt numar...");
          display.display();
          
          Serial.println("Gata! Introdu alt numar:");
        } else {
          Serial.println("\nEroare: Trebuie sa introduci un numar mai mare ca 0.");
        }
        
        // Curățăm memoria pentru următorul număr
        messageBuffer = ""; 
      }
    } else {
      // Adăugăm litera la mesaj
      messageBuffer += incomingChar;
      // Trimitem litera înapoi în terminal (Ecou pentru a vedea ce scrii)
      Serial.print(incomingChar); 
    }
  }
}