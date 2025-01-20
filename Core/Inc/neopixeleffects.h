#pragma once
#include "NeoPixel.h"

static void neopixel_fade() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    NeoPixel.setPixelColor(i, 229, 43, 62);
  }
  NeoPixel.show();
  NeoPixel.setBrightness(brightness++);
}
