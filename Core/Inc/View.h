#pragma once


#if defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#else
#error Unsupported board!
#endif

#include "gfx.h"


typedef struct {
	void(*render)(void);
} View;
