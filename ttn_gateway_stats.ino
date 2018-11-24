#include <ArduinoJson.h>

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

#define SIXTY_SECONDS 60000

const String apiUri = "http://noc.thethingsnetwork.org:8085/api/v2/gateways/";

const String gatewayA = "puphaus-1";
const String gatewayB = "smerty";
const String gatewayC = "skycentricslora";

// Hardware Bits
Adafruit_7segment displayReceived = Adafruit_7segment(); // Green
Adafruit_7segment displaySent = Adafruit_7segment(); // Blue
Adafruit_FeatherOLED oled = Adafruit_FeatherOLED();
ESP8266WiFiMulti wifi;

// Globals
char* currentGateway = "a";
unsigned long last_update;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // init 7-segments
  displayReceived.begin(0x71);
  displayReceived.print(0,DEC);
  displayReceived.writeDisplay();
  
  displaySent.begin(0x70);
  displaySent.print(0,DEC);
  displaySent.writeDisplay();

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
  
  setOledDisplay(gatewayA);
  updateDisplay();
}

void loop() {
  if(!digitalRead(BUTTON_A)) buttonA();
  if(!digitalRead(BUTTON_B)) buttonB();
  if(!digitalRead(BUTTON_C)) buttonC();
  
  if (millis() > last_update + SIXTY_SECONDS) {
    updateDisplay();
  }
}

void buttonA() {
  currentGateway = "a";
  setOledDisplay(gatewayA);
  updateDisplay();
}

void buttonB() {
  currentGateway = "b";
  setOledDisplay(gatewayB);
  updateDisplay();
}

void buttonC() {
  currentGateway = "c";
  setOledDisplay(gatewayC);
  updateDisplay();
}

void updateDisplay() {
  if ((wifi.run() == WL_CONNECTED)) {
    HTTPClient http;

    if (currentGateway == "a") {
      http.begin(apiUri + gatewayA);
    }
    else if (currentGateway == "b") {
      http.begin(apiUri + gatewayB);
    }
    else {
      http.begin(apiUri + gatewayC);
    }

    int httpCode = http.GET();
    
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        
        DynamicJsonBuffer jsonBuffer;
        JsonObject& gatewayInfo = jsonBuffer.parseObject(payload);
        
        if (!gatewayInfo.success()) {
          Serial.println("parseObject() failed");
        }
        else {
          String sent = gatewayInfo["downlink"];
          setSentDisplay(sent.toInt());
          
          String received = gatewayInfo["uplink"];
          setReceivedDisplay(received.toInt());
        }
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  }

  last_update = millis();
}

void setOledDisplay(String text) {
  oled.clearDisplay();
  oled.setFont(&FreeMono12pt7b);
  oled.setCursor(0, 20),
  oled.print(text);
  oled.display();
  
}

void setReceivedDisplay(unsigned long count) {
    Serial.print("Received: ");
    Serial.println(count, DEC);
    displayReceived.println(count % 10000, DEC);
    displayReceived.writeDisplay();
}

void setSentDisplay(unsigned long count) {
    Serial.print("Sent: ");
    Serial.println(count, DEC);
    displaySent.println(count % 10000, DEC);
    displaySent.writeDisplay();
}
