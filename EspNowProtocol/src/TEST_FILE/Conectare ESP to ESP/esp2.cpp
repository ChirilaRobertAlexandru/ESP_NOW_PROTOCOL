#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>

// Pinii si setarile pentru Display
#define I2C_SDA 14
#define I2C_SCL 15
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pinii pentru LED-uri
#define LED1 12
#define LED2 13
#define LED3 2

// Variabile pentru preluarea mesajului
String comandaPrimita = "";
bool comandaNoua = false;

// Functia care ruleaza automat cand vine un mesaj de la ESP1
void laPrimire(const uint8_t * mac, const uint8_t *datePrimite, int lungime) {
  comandaPrimita = "";
  for (int i = 0; i < lungime; i++) {
    comandaPrimita += (char)datePrimite[i];
  }
  comandaNoua = true; // Semnalam procesorului ca avem de munca
}

void setup() {
  Serial.begin(115200);
  
  // Setam pinii LED
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // Initializare I2C si OLED
  Wire.begin(I2C_SDA, I2C_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Eroare OLED"));
    for(;;);
  }
  
  // Initializare Wi-Fi si ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Eroare ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(laPrimire);

  // Ecran de pornire
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("ESP 2 RECEPTOR");
  display.println("Astept comenzi...");
  display.display();
  
  Serial.println("\n--- ESP 2 PREGATIT ---");
}

void loop() {
  // Daca am primit o comanda noua, o descifram
  if (comandaNoua) {
    comandaPrimita.trim();
    Serial.println("\nMesaj brut primit: " + comandaPrimita);

    // Gasim pozitiile spatiilor libere pentru a separa numerele
    int spatiu1 = comandaPrimita.indexOf(' ');
    int spatiu2 = comandaPrimita.indexOf(' ', spatiu1 + 1);
    int spatiu3 = comandaPrimita.indexOf(' ', spatiu2 + 1);

    // Verificam daca formatul a fost respectat (adica daca exista toate cele 3 spatii)
    if (spatiu1 > 0 && spatiu2 > 0 && spatiu3 > 0) {
      
      // Extragem valorile
      int numarLed = comandaPrimita.substring(0, spatiu1).toInt();
      int nrClipiri = comandaPrimita.substring(spatiu1 + 1, spatiu2).toInt();
      float tOn = comandaPrimita.substring(spatiu2 + 1, spatiu3).toFloat();
      float tOff = comandaPrimita.substring(spatiu3 + 1).toFloat();

      // Identificam pinul fizic dorit
      int pinAles = -1;
      if (numarLed == 1) pinAles = LED1;
      else if (numarLed == 2) pinAles = LED2;
      else if (numarLed == 3) pinAles = LED3;

      // Daca s-a cerut un LED valid (1, 2 sau 3), executam comanda
      if (pinAles != -1 && nrClipiri > 0) {
        
        // 1. Afisam informatiile pe display
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("ledul "); display.print(numarLed); display.println(" ON");
        display.print("clipeste de "); display.print(nrClipiri); display.println(" ori");
        display.print("Ton="); display.print(tOn); display.print(" Toff="); display.println(tOff);
        display.display();

        // 2. Executam fizic aprinderea/stingerea
        for (int i = 0; i < nrClipiri; i++) {
          digitalWrite(pinAles, HIGH);
          delay(tOn * 1000); // Inmultim cu 1000 pentru a transforma secundele in milisecunde
          digitalWrite(pinAles, LOW);
          delay(tOff * 1000);
        }

        // 3. Resetam display-ul
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Comanda executata!");
        display.println("Astept alta...");
        display.display();
      } else {
        Serial.println("Eroare: Numar LED invalid sau clipiri 0.");
      }
    } else {
      Serial.println("Eroare: Format incorect. Foloseste: LED NR_CLIPIRI TON TOFF");
    }

    // Marcam comanda ca fiind procesata
    comandaNoua = false;
  }
}