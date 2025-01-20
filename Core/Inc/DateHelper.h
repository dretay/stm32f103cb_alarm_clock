#pragma once

#include "time.h"
#if defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#else
#error Unsupported board!
#endif
extern RTC_HandleTypeDef hrtc;

struct datehelper
{
	time_t(*get_epoch)(void);
	char*(*get_day_of_week)(void);
	char*(*get_month)();
	int(*get_year)();
};
extern const struct datehelper DateHelper;
