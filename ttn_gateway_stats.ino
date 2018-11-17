#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_FeatherOLED.h>
#include <Fonts/FreeMono12pt7b.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define BUTTON_A  0
#define BUTTON_B 16
#define BUTTON_C  2

const char* apiUri = "http://noc.thethingsnetwork.org:8085/api/v2/gateways/";

const char* gateway_a = "puphaus-1"
const char* gateway_b = "puphaus-2"
const char* gateway_c = "puphaus-1"

Adafruit_7segment display_received = Adafruit_7segment(); // Green
Adafruit_7segment display_sent = Adafruit_7segment(); // Blue
Adafruit_FeatherOLED oled = Adafruit_FeatherOLED();
ESP8266WiFiMulti wifi;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // init 7-segments
  display_received.begin(0x70);
  display_received.print(0,DEC);
  display_received.writeDisplay();
  
  display_sent.begin(0x71);
  display_sent.print(0,DEC);
  display_sent.writeDisplay();
  

  // init display
  oled.init();
  oled.clearDisplay();
  oled.setFont(&FreeMono12pt7b);
  oled.setCursor(0, 20),
  oled.print("hello!");
  oled.display();

  delay(1000);

  // init wifi
  oled.clearDisplay();
  oled.setFont();
  WiFi.mode(WIFI_STA);

  wifi.addAP("SSID", "password");

  oled.setCursor(0, 0),
  oled.print("Connecting Wifi");
  oled.display();

  while (wifi.run() != WL_CONNECTED) {
    oled.print(".");
    oled.display();
    delay(1000);
  }
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  oled.clearDisplay();
  oled.setCursor(0, 0),
  oled.println("WiFi connected");
  oled.println("IP address: ");
  oled.println(WiFi.localIP());
  oled.display();
  
  delay(1000);
}

void loop() {
  oled.clearDisplay();
  oled.setFont(&FreeMono12pt7b);
  oled.setCursor(0, 20),
  oled.print("meow!");
  oled.display();
  
  // put your main code here, to run repeatedly:
  delay(1000);
  
  if (wifi.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    delay(1000);
  }

}
