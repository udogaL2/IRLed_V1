/*
 * IRremoteESP8266: IRrecvDumpV2 - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input kRecvPin.
 *
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Copyright 2017-2019 David Conran
 *
 * Example circuit diagram:
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 *
 * Changes:
 *   Version 1.2 October, 2020
 *     - Enable easy setting of the decoding tolerance value.
 *   Version 1.0 October, 2019
 *     - Internationalisation (i18n) support.
 *     - Stop displaying the legacy raw timing info.
 *   Version 0.5 June, 2019
 *     - Move A/C description to IRac.cpp.
 *   Version 0.4 July, 2018
 *     - Minor improvements and more A/C unit support.
 *   Version 0.3 November, 2017
 *     - Support for A/C decoding for some protocols.
 *   Version 0.2 April, 2017
 *     - Decode from a copy of the data so we can start capturing faster thus
 *       reduce the likelihood of miscaptures.
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
 */

#include <Arduino.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

#ifdef ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 13;
#else
const uint16_t kRecvPin = 13;
#endif

const uint32_t kBaudRate = 115200;

const uint16_t kCaptureBufferSize = 1024;

#if DECODE_AC
const uint8_t kTimeout = 50;
#else
const uint8_t kTimeout = 15;
#endif
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;

#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results

int LED1 = 12;
bool LED1_STATE = false;
#define LED_PIN 14
bool LED2_STATE = false;

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_ALL_PINS_HARDWARE_SPI

#include "FastLED.h"
#define LED_NUM 1
CRGB leds[LED_NUM];

void setup() {
  #if defined(ESP8266)
    Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
  #else  // ESP8266
    Serial.begin(kBaudRate, SERIAL_8N1);
  #endif  // ESP8266
    while (!Serial)
      delay(50);
  assert(irutils::lowLevelSanityCheck() == 0);
  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
  #if DECODE_HASH
    irrecv.setUnknownThreshold(kMinUnknownSize);
  #endif
    irrecv.setTolerance(kTolerancePercentage);
    irrecv.enableIRIn();

  pinMode(LED1, OUTPUT);
  FastLED.addLeds< WS2812, LED_PIN, GRB>(leds, LED_NUM);
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == 16738455) // 1
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    if (results.value == 16750695) // 2
    {
      if (LED2_STATE)
        leds[0] = CRGB::Red;
      else
        FastLED.clear();

      FastLED.show();
      LED2_STATE = !LED2_STATE;
    }
    if (results.value == 16756815) // 3
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    if (results.value == 16724175) // 4
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    if (results.value == 16718055) // 5
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    if (results.value == 16743045) // 6
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    if (results.value == 16716015) // 7
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    if (results.value == 16726215) // 8
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    if (results.value == 16734885) // 9
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    if (results.value == 16728765) // *
    {
      digitalWrite(LED1, LED1_STATE); 
      LED1_STATE = !LED1_STATE;
    }
    yield();
  }
}
