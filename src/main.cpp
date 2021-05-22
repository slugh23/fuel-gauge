#include <Arduino.h>

#include "FastLED.h"

#define LEVEL_INPUT A1

const int GAUGE_LENGTH = 9;
const int GAUGE_OFFSET = 0;
const int STRIP_LENGTH = 24;

const int SAMPLE_PERIOD = 100;
const int BUFFER_SECONDS = 3;
const int BUFFER_LENGTH = BUFFER_SECONDS * 1000 / SAMPLE_PERIOD;


CRGB colors[GAUGE_LENGTH] = {
  CRGB::Red, CRGB::Red, CRGB::Red,
  CRGB::Yellow, CRGB::Yellow, CRGB::Yellow,
  CRGB::Green, CRGB::Green, CRGB::Green
  };

CRGB leds[STRIP_LENGTH];

uint8_t buffer[BUFFER_LENGTH];
int idx = 0;
uint32_t last = 0;

uint16_t average() {
  uint16_t sum = 0;
  for (int i = 0; i < BUFFER_LENGTH; ++i) {
    sum += buffer[i];
  }
  return sum / BUFFER_LENGTH;
}

uint16_t fuel_level() {
  return average() / 26;
}

bool do_sample() {
  uint32_t now = millis();
  if((now - last) > SAMPLE_PERIOD) {
    last = now;
    buffer[idx] = (analogRead(LEVEL_INPUT) & 0x3FF) >> 2;
    if (++idx > BUFFER_LENGTH) {
      idx = 0;
    }
    return true;
  }
  return false;
}

void start_up() {
  int i = 0;
  while(i < BUFFER_LENGTH)
  {
    if (do_sample()) {
      for (int l = 0; l < STRIP_LENGTH; ++l) {
        leds[l] = ((i + l) % 2) ? CRGB::Green : CRGB::Black;
      }
      FastLED.show();
      ++i;
    }
  }
}

void setup() {
  memset(buffer, 0, BUFFER_LENGTH);
  FastLED.addLeds<NEOPIXEL, 0>(leds, STRIP_LENGTH);
  FastLED.setBrightness(84);
  analogReference(DEFAULT);
  FastLED.showColor(CRGB::Black);
  start_up();
  for (int i = 0; i < STRIP_LENGTH; ++i) {
    leds[i] = CRGB::Black;
  }
}

bool on = true;
void loop() {
  if (do_sample())
  {
    uint16_t fl = fuel_level();
    if (fl > 0)
    {
      if (on) {
        FastLED.showColor(CRGB::Black);
        on = false;
      }
      for (unsigned i = 0; i < GAUGE_LENGTH; ++i) {
        leds[i + GAUGE_OFFSET] = (i <= fl) ? colors[i] : CRGB::Black;
      }
      FastLED.show();
    }
    else {
      on = !on;
      FastLED.showColor(on ? CRGB::Red : CRGB::Black);
    }
  }
}
