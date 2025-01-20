#pragma once

#if defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#else
#error Unsupported board!
#endif
static const uint8_t SB              = 0x7E;
static 	const uint8_t VER             = 0xFF;
static const uint8_t LEN             = 0x6;
static const uint8_t VOL_ADJ         = 0x10;
static const uint8_t NO_FEEDBACK     = 0;
static const uint8_t VOL_ADJUST      = 0x10;
static const uint8_t USE_MP3_FOLDER  = 0x12;
static const uint8_t EB              = 0xEF;
typedef union
{
  struct
  {
		uint8_t start_byte;
		uint8_t version;
		uint8_t length;
		uint8_t commandValue;
		uint8_t feedbackValue;
		uint8_t paramMSB;
		uint8_t paramLSB;
		uint8_t checksumMSB;
		uint8_t checksumLSB;
		uint8_t end_byte;
  } stack;
  uint8_t bytes[80];
} DFPlayerStack_t;



struct dfplayermini {
	void(*init)(void);
	void(*volumeAdjustSet)(uint8_t gain);
	void(*playFromMP3Folder)(uint16_t trackNum);
};

extern const struct dfplayermini DFPlayerMini;
