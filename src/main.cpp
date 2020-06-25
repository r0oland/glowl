#include <Arduino.h>
#include <ESP8266WiFi.h>

// OTA updates
#include "SecureOTA.h"
const uint16_t OTA_CHECK_INTERVAL = 5000; // ms
uint32_t _lastOTACheck = 0;

// for OLED screen
#include <U8g2lib.h> 
U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,D1,D2); // full buffer size
const uint16_t FRAME_RATE = 3; // frames per second
const uint16_t FRAME_TIME_MILLIS = 1000./FRAME_RATE; // time per frame in milis.
const uint8_t LINE_SPACING = 8; // we can print an new, non-overlapping line


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(SERIAL_SPEED);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Device version: v.");
  Serial.println(VERSION);
  Serial.print("Connecting to " + String(WIFI_SSID));

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println(" connected!");
  _lastOTACheck = millis();

  // your setup code goes here
}

void loop()
{
  if ((millis() - OTA_CHECK_INTERVAL) > _lastOTACheck) {
    _lastOTACheck = millis();
    checkFirmwareUpdates(); // takes ~1s, make sure this is not a problem in your code
  }

  // your loop code goes here
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
}
