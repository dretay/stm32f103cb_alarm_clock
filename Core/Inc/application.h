#pragma once

#include "ClockView.h"
#include "DFPlayerMini.h"
#include "NeoPixel.h"
#include "PacketSerial.h"
#include "ProtoBuff.h"
#include "STM32SAM.h"
#include "gfx.h"

#if defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#else
#error Unsupported board!
#endif
struct application {
  void (*init)(void);
  void (*run)(void);
};

extern const struct application Application;
