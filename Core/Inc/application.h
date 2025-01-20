#pragma once

#include "PacketSerial.h"
#include "ProtoBuff.h"
#include "NeoPixel.h"
#include "STM32SAM.h"
#include "DFPlayerMini.h"
#include "gfx.h"
#include "ClockView.h"


#if defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#else
#error Unsupported board!
#endif
struct application {
	void(*init)(void);
	void(*run)(void);
};

extern const struct application Application;
