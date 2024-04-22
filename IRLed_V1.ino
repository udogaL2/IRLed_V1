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
decode_results results;

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_ALL_PINS_HARDWARE_SPI

#include "FastLED.h"

CRGB COLORS[] = {
  CRGB::White, CRGB::Red, CRGB::Chartreuse, CRGB::Coral, CRGB::Crimson, CRGB::DeepSkyBlue, 
  CRGB::GreenYellow, CRGB::Indigo, CRGB::Magenta, CRGB::Lime, CRGB::MediumSeaGreen, CRGB::MediumPurple, 
  CRGB::Orange, CRGB::PaleGreen, CRGB::PaleVioletRed, CRGB::Plaid, CRGB::Purple, CRGB::SandyBrown, 
  CRGB::SkyBlue, CRGB::Tomato, CRGB::Yellow
};
#define COLOR_COUNT 21

int brightness = 255;
bool reset_flag = false;

#define LED_1_PIN 5
#define LED_1_NUM 1
bool LED_1_STATE = false;
int LED_1_COLOR_ID = 0;
CRGB leds_1[LED_1_NUM];

#define LED_2_PIN 4
#define LED_2_NUM 1
bool LED_2_STATE = false;
int LED_2_COLOR_ID = 0;
CRGB leds_2[LED_2_NUM];

#define LED_3_PIN 0
#define LED_3_NUM 1
bool LED_3_STATE = false;
int LED_3_COLOR_ID = 0;
CRGB leds_3[LED_3_NUM];

#define LED_4_PIN 2
#define LED_4_NUM 1
bool LED_4_STATE = false;
int LED_4_COLOR_ID = 0;
CRGB leds_4[LED_4_NUM];

#define LED_5_PIN 14
#define LED_5_NUM 1
bool LED_5_STATE = false;
int LED_5_COLOR_ID = 0;
CRGB leds_5[LED_5_NUM];

#define LED_6_PIN 12
#define LED_6_NUM 1
bool LED_6_STATE = false;
int LED_6_COLOR_ID = 0;
CRGB leds_6[LED_6_NUM];

#define LED_7_PIN 15
#define LED_7_NUM 1
bool LED_7_STATE = false;
int LED_7_COLOR_ID = 0;
CRGB leds_7[LED_7_NUM];

#define LED_8_PIN 16
#define LED_8_NUM 1
bool LED_8_STATE = false;
int LED_8_COLOR_ID = 0;
CRGB leds_8[LED_8_NUM];

void my_clear(CRGB *leds, int led_num)
{
  for (int i = 0; i < led_num; i++)
  {
    leds[i] = CRGB::Black;
  }
}

void clear_all()
{
  LED_1_STATE = false;
  LED_1_COLOR_ID = 0;
  LED_2_STATE = false;
  LED_2_COLOR_ID = 0;
  LED_3_STATE = false;
  LED_3_COLOR_ID = 0;
  LED_4_STATE = false;
  LED_4_COLOR_ID = 0;
  LED_5_STATE = false;
  LED_5_COLOR_ID = 0;
  LED_6_STATE = false;
  LED_6_COLOR_ID = 0;
  LED_7_STATE = false;
  LED_7_COLOR_ID = 0;
  LED_8_STATE = false;
  LED_8_COLOR_ID = 0;

  FastLED.clear();
  FastLED.show();
}

void set_color(CRGB *leds, int led_num, int color_id)
{
  for (int i = 0; i < led_num; i++)
  {
    leds[i] = COLORS[color_id];
  }
}

void change_color_id(int &color_id)
{
  color_id = (color_id + 1) % COLOR_COUNT;
}

void my_led_action(CRGB *leds, int led_num, int &color_id, bool &led_stage)
{
  if (reset_flag)
  {
    reset_flag = false;
    color_id = 0;
    led_stage = true;
  }
  else
    led_stage = !led_stage;

  if (led_stage)
  {
    set_color(leds, led_num, color_id);
  }
  else
  {
    change_color_id(color_id);
    my_clear(leds, led_num);        
  }

  FastLED.show();
}

void setup() 
{
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

  FastLED.addLeds< WS2812, LED_1_PIN, GRB>(leds_1, LED_1_NUM);
  FastLED.addLeds< WS2812, LED_2_PIN, GRB>(leds_2, LED_2_NUM);
  FastLED.addLeds< WS2812, LED_3_PIN, GRB>(leds_3, LED_3_NUM);
  FastLED.addLeds< WS2812, LED_4_PIN, GRB>(leds_4, LED_4_NUM);
  FastLED.addLeds< WS2812, LED_5_PIN, GRB>(leds_5, LED_5_NUM);
  FastLED.addLeds< WS2812, LED_6_PIN, GRB>(leds_6, LED_6_NUM);
  FastLED.addLeds< WS2812, LED_7_PIN, GRB>(leds_7, LED_7_NUM);
  FastLED.addLeds< WS2812, LED_8_PIN, GRB>(leds_8, LED_8_NUM);

  clear_all();
}

void loop() 
{
  if (irrecv.decode(&results)) 
  {
    if (results.value == 16761405) // ->
    {
      if (brightness < 240)
        brightness = (brightness + 15) % 255;
      else if (brightness == 240)
        brightness = 255;
      FastLED.setBrightness(brightness);
      FastLED.show();
    }
    if (results.value == 16720605) // <-
    {
      if (brightness > 0)
        brightness = (brightness - 15) % 255;
      FastLED.setBrightness(brightness);
      FastLED.show();
    }
    if (results.value == 16736925) // стрелка вверх
    {

    }
    if (results.value == 16754775) // стрелка вниз
    {

    }
    if (results.value == 16738455) // 1
    {
      my_led_action(leds_1, LED_2_NUM, LED_2_COLOR_ID, LED_2_STATE);
    }
    if (results.value == 16750695) // 2
    {
      my_led_action(leds_2, LED_2_NUM, LED_2_COLOR_ID, LED_2_STATE);
    }
    if (results.value == 16756815) // 3
    {
      my_led_action(leds_3, LED_3_NUM, LED_3_COLOR_ID, LED_3_STATE);
    }
    if (results.value == 16724175) // 4
    {
      my_led_action(leds_4, LED_4_NUM, LED_4_COLOR_ID, LED_4_STATE);
    }
    if (results.value == 16718055) // 5
    {
      my_led_action(leds_5, LED_5_NUM, LED_5_COLOR_ID, LED_5_STATE);
    }
    if (results.value == 16743045) // 6
    {
      my_led_action(leds_6, LED_6_NUM, LED_6_COLOR_ID, LED_6_STATE);
    }
    if (results.value == 16716015) // 7
    {
      my_led_action(leds_7, LED_7_NUM, LED_7_COLOR_ID, LED_7_STATE);
    }
    if (results.value == 16726215) // 8
    {
      my_led_action(leds_8, LED_8_NUM, LED_8_COLOR_ID, LED_8_STATE);
    }
    if (results.value == 16734885) // 9
    {

    }
    if (results.value == 16728765) // *
    {
      clear_all();
    }
    if (results.value == 16732845) // #
    {
      reset_flag = !reset_flag;
    }
    yield();
  }
}
