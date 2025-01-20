#pragma once

#include <stdbool.h>
#if defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#else
#error Unsupported board!
#endif
#include "Timer.h"
// https://forum.digikey.com/t/controlling-neopixels-with-stm32/20527

typedef union {
  struct {
    uint8_t w;
    uint8_t b;
    uint8_t r;
    uint8_t g;
  } color;
  uint32_t data;
} PixelRGBW_t;
// (ARR+1)*.25
#define NEOPIXEL_ZERO 23
//(ARR+1)*.5
#define NEOPIXEL_ONE 45

#define NUM_PIXELS 12
#define DMA_BUFF_SIZE (NUM_PIXELS * 32) + 1

struct neopixel {
  void (*setPixelColor)(uint16_t n, PixelRGBW_t fade_color);
  bool (*show)(void);
  void (*clear)(void);
  void (*begin)(void);
  void (*setBrightness)(uint8_t _brightness);
  void (*fade_effect)(uint8_t r, uint8_t g, uint8_t b, uint8_t speed);
  void (*color_wipe)(uint8_t r, uint8_t g, uint8_t b, int wait);
  void (*theatre_chase)(uint8_t r, uint8_t g, uint8_t b, int wait);
  void (*rainbow)(int wait);
};

extern const struct neopixel NeoPixel;
