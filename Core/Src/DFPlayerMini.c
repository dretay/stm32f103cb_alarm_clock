#include "DFPlayerMini.h"

static DFPlayerStack_t sendStack = {0};
extern UART_HandleTypeDef huart1;

static void init() {
  sendStack.stack.start_byte = SB;
  sendStack.stack.version = VER;
  sendStack.stack.length = LEN;
  sendStack.stack.end_byte = EB;
}

static void findChecksum(DFPlayerStack_t* _stack) {
  uint16_t checksum =
      (~(_stack->stack.version + _stack->stack.length +
         _stack->stack.commandValue + _stack->stack.feedbackValue +
         _stack->stack.paramMSB + _stack->stack.paramLSB)) +
      1;

  _stack->stack.checksumMSB = checksum >> 8;
  _stack->stack.checksumLSB = checksum & 0xFF;
}
static void sendData() {
  HAL_UART_Transmit(&huart1, sendStack.bytes, sizeof(sendStack.bytes), 100);
  //	_serial->write(sendStack.start_byte);
  //	_serial->write(sendStack.version);
  //	_serial->write(sendStack.length);
  //	_serial->write(sendStack.commandValue);
  //	_serial->write(sendStack.feedbackValue);
  //	_serial->write(sendStack.paramMSB);
  //	_serial->write(sendStack.paramLSB);
  //	_serial->write(sendStack.checksumMSB);
  //	_serial->write(sendStack.checksumLSB);
  //	_serial->write(sendStack.end_byte);
}

static void volumeAdjustSet(uint8_t gain) {
  if (gain <= 31) {
    sendStack.stack.commandValue = VOL_ADJ;
    sendStack.stack.feedbackValue = NO_FEEDBACK;
    sendStack.stack.paramMSB = 0;
    sendStack.stack.paramLSB = VOL_ADJUST + gain;

    findChecksum(&sendStack);
    sendData();
  }
}
static void playFromMP3Folder(uint16_t trackNum) {
  sendStack.stack.commandValue = USE_MP3_FOLDER;
  sendStack.stack.feedbackValue = NO_FEEDBACK;
  sendStack.stack.paramMSB = (trackNum >> 8) & 0xFF;
  sendStack.stack.paramLSB = trackNum & 0xFF;

  findChecksum(&sendStack);
  sendData();
}

const struct dfplayermini DFPlayerMini = {
    .init = init,
    .volumeAdjustSet = volumeAdjustSet,
    .playFromMP3Folder = playFromMP3Folder,
};
