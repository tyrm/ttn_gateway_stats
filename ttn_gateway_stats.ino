#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <Fonts/FreeMono12pt7b.h>

#define BUTTON_A  0
#define BUTTON_B 16
#define BUTTON_C  2

Adafruit_FeatherOLED oled = Adafruit_FeatherOLED();

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  oled.init();
  oled.clearDisplay();
  
  // print the count value to the OLED
  oled.setFont(&FreeMono12pt7b);
  oled.setCursor(0, 20),
  oled.print("puphaus-1");

  // update the display with the new count
  oled.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

}
