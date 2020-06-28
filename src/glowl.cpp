#include <Arduino.h>
#include <WiFi.h>


// for OTA updates -------------------------------------------------------------
#include "SecureOTA.h"
const uint16_t OTA_CHECK_INTERVAL = 5000; // ms
uint32_t lastOTACheck = 0;

// for OLED screen -------------------------------------------------------------
#include <U8g2lib.h> 
U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,/* clock=*/ 22, /* data=*/ 21); // full buffer size

const uint16_t FRAME_RATE = 3; // frames per second
const uint16_t FRAME_TIME_MILLIS = 1000./FRAME_RATE; // time per frame in milis.
const uint8_t LINE_SPACING = 8; // we can print an new, non-overlapping line

// LED control -----------------------------------------------------------------
#include <FastLED.h> 
FASTLED_USING_NAMESPACE
// defin LED parameters
#define DATA_PIN 12
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 135 // 144 - 8, 8 are in the eyes ;-), nee to add those back later
#define BRIGHTNESS 255 // [0 - 256]
CRGB leds[NUM_LEDS]; // contains led info, we set this first, then we call led show

// LED control function declarations
void setup_leds();
void set_led_status(uint8_t status);
void pulse_leds(uint8_t nPulses, uint8_t pulseSpeed);

// setup time ------------------------------------------------------------------
#include <time.h>
const int8_t TIME_ZONE = -2; // central european time

void setup()
{
  // setup serial in case we need it for debugging later...
  Serial.begin(SERIAL_SPEED);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("GLOWL 0.1");

  // setup OLED & font to use
  uint8_t thisLine = 1;
  u8g2.begin(); 
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x8_mr); 
  // hello world :-)
  u8g2.setCursor(0,LINE_SPACING*thisLine++); // start at first line, then increment
  u8g2.print("GLOWL 0.1");
  u8g2.setCursor(0,LINE_SPACING*8); // print signature in last line
  u8g2.print("Laser Hannes 2020");
  u8g2.sendBuffer();
  delay(250);

  // setting up leds ===========================================================
  u8g2.setCursor(0,LINE_SPACING*thisLine++); 
  u8g2.print("Setting up leds...");
  u8g2.sendBuffer();
  setup_leds();
  u8g2.print("done!");
  set_led_status(1); // working

  // setting up OTA / WIFI =====================================================
  u8g2.setCursor(0,LINE_SPACING*thisLine++); 
  u8g2.print("Setting Wifi and OTA:");
  u8g2.sendBuffer();

  u8g2.setCursor(0,LINE_SPACING*thisLine++); 
  u8g2.print("Firmware version V.");
  u8g2.print(VERSION);
  u8g2.sendBuffer();

  u8g2.setCursor(0,LINE_SPACING*thisLine++); 
  u8g2.print("Connecting to Wifi:");
  u8g2.setCursor(0,LINE_SPACING*thisLine++); 
  u8g2.print(String(WIFI_SSID));
  u8g2.sendBuffer();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    u8g2.print(".");
    u8g2.sendBuffer();
    delay(250);
  }
  u8g2.print("done!"); 
  u8g2.sendBuffer();
  lastOTACheck = millis();
}

void loop()
{
  if ((millis() - OTA_CHECK_INTERVAL) > lastOTACheck) {
    lastOTACheck = millis();
    checkFirmwareUpdates(); // takes ~1s, make sure this is not a problem in your code
  }
}

void setup_leds()
{
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  uint16_t initTime = 2000; // % time for full "load" of all LEDS
  uint16_t ledDelay = initTime / NUM_LEDS;

  FastLED.clear();
  FastLED.show();
  for (int led = 0; led < NUM_LEDS; led++)
  {
    leds[led] = CRGB::White;
    delay(ledDelay);
    FastLED.show();
  }
  delay(100);
  pulse_leds(3, 5);
  FastLED.clear();
  FastLED.show();
}

void pulse_leds(uint8_t nPulses, uint8_t pulseSpeed)
{
  uint8_t ledFade = 255;      // start with LEDs off
  int8_t additionalFade = -5; // start with LEDs getting brighter
  uint8_t iPulse = 0;

  while (iPulse < nPulses)
  {
    for (uint8_t iLed = 0; iLed < NUM_LEDS; iLed++)
    {
      leds[iLed].setRGB(255, 255, 255);
      leds[iLed].fadeLightBy(ledFade);
    }
    FastLED.show();
    ledFade = ledFade + additionalFade;
    // reverse the direction of the fading at the ends of the fade:
    if (ledFade == 0 || ledFade == 255)
      additionalFade = -additionalFade;
    if (ledFade == 255)
      iPulse++;
    delay(pulseSpeed); // This delay sets speed of the fade. I usually do from 5-75 but you can always go higher.
  }
}

void set_led_status(uint8_t status)
{
  // first led in array displays overall status
  // (0 = all good, 1 = working, 2 = error)
  switch (status)
  {
  case 0:
    leds[0].setRGB(0, 255, 0); // all good = green
    FastLED.show();
    break;
  case 1:
    leds[0].setRGB(200, 165, 0); // working == orange
    FastLED.show();
    break;
  case 2:
    leds[0].setRGB(255, 0, 0); // error == red
    FastLED.show();
    break;

  default:
    leds[0].setRGB(255, 0, 0);
    FastLED.show();
    break;
  }
}
