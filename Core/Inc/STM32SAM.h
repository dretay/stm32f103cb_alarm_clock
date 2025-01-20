#pragma once
// SAM Text-To-Speech (TTS), ported from https://github.com/s-macke/SAM

#if defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#else
#error Unsupported board!
#endif
#include "micros.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct stm32sam {
	void(*init)(uint32_t STM32SAM_SPEED);
    void(*begin)(void);
    void(*sam)( const char *argv, unsigned char phonetic , unsigned char singmode , unsigned char pitch , unsigned char speed , unsigned char  mouth , unsigned char throat );
    void(*say)(const char *argv );
    void(*sing)(const char *argv );
    void(*sayPhonetic)(const char *argv );
    void(*singPhonetic)(const char *argv );
    void(*setVoice)(unsigned char _pitch, unsigned char _speed, unsigned char  _mouth , unsigned char _throat) ;
    void(*setPitch)(unsigned char _pitch);
    void(*setSpeed)( unsigned char _speed);
    void(*setMouth)(unsigned char  _mouth);
    void(*setThroat)( unsigned char _throat);
};

extern const struct stm32sam STM32SAM;
