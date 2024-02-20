#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Failed to boot SSD1306");
    delay(500);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  display.print("Booting...");
  display.display();
  delay(2000);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 12);
  display.print("Ciao! :)");
  display.display();

  delay(2000);

  display.clearDisplay();
  display.setCursor(0, 12);
  display.print("Evviva! :D");
  display.display();

  delay(2000);
}
