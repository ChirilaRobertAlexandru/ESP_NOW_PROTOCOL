#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_SDA 14
#define I2C_SCL 15

// Declarăm lățimea maximă a cipului (128) pentru a curăța toată memoria
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String messageBuffer = "";

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Eroare la initializarea SSD1306"));
    for(;;);
  }
  
  // Curățăm imediat TOATĂ memoria video a cipului la pornire
  display.clearDisplay();
  display.display(); 
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(true); 
  
  // Dacă textul începe prea în stânga/dreapta, modificăm primul 0 din setCursor
  display.setCursor(0, 10);
  display.println("Astept mesaje...");
  display.display();
  
  Serial.println("\nScrie un mesaj in terminal si apasa Enter:");
}

void loop() {
  while (Serial.available() > 0) {
    char incomingChar = Serial.read(); 
    
    if (incomingChar == '\n' || incomingChar == '\r') {
      if (messageBuffer.length() > 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(messageBuffer);
        display.display();
        
        Serial.println("\nAfisat pe ecran: " + messageBuffer);
        messageBuffer = ""; 
      }
    } else {
      messageBuffer += incomingChar;
      // Trimitem litera înapoi în terminal pentru a vedea ce tastezi
      //Serial.print(incomingChar);
    }
  }
}