#include "application.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

extern SPI_HandleTypeDef hspi1;
// extern SPI_HandleTypeDef hspi2;
extern I2C_HandleTypeDef hi2c2;

static View* clock_view;

// void speak_parser(pb_istream_t* stream, const pb_field_t* type)
//{
//     Speak speak;
//     if (ProtoBuff.decode_union(stream, type, &speak)) {
//     	if(speak.data[0] == 'h'){
//     		return;
//     	}
//         //do stuff with decoded message
//     } else {
//     }
// }

// static void neopixel_fade(){
//	for (int i = 0; i < NUM_PIXELS; i++){
//		NeoPixel.setPixelColor(i,229,43,62);
//	}
//	NeoPixel.show();
//	NeoPixel.setBrightness(brightness++);
// }
// void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
//	if(hspi == &hspi2){
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,SET);
//	}
// }
static void init() {
  // put this somewhere else, just need the clock to start high
  uint8_t TX_Data[2] = {0};
  TX_Data[0] = 0x00;
  TX_Data[1] = 0x00;
  if (HAL_SPI_Transmit(&hspi1, TX_Data, sizeof(TX_Data), 1000) != HAL_OK) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);
  }

  //	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,SET);
  //	HAL_Delay(100);
  //	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,RESET);
  gfxInit();
  gdispGSetOrientation(gdispGetDisplay(0), GDISP_ROTATE_270);
  clock_view = ClockView.init();
  DFPlayerMini.init();

  //	ProtoBuff.add_handler(Speak_fields, &speak_parser);
  //	NeoPixel.begin();
  //	NeoPixel.fade_effect(229,43,62,10);
  //	NeoPixel.color_wipe(229,100,12,50);
  //	NeoPixel.theatre_chase(29,200,52,50);
  //	NeoPixel.rainbow(10);

  //	STM32SAM.init(5);
  //	STM32SAM.say("Hello world");
  //	STM32SAM.say("I am S T M 32 SAM, automated voice! ");
  //	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

  //	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim3);
  Timer.every(1000, clock_view->render);
}
static bool QUERY_ENCODER = false;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_1) {
    QUERY_ENCODER = true;
  }
}
static void query_encoder() {
  uint16_t SLAVE_ADDR = 0x10 << 1;
  uint8_t TxData[] = {0x08};
  HAL_I2C_Master_Transmit(&hi2c2, SLAVE_ADDR, TxData, 1, 1000);
  HAL_Delay(100);

  uint8_t RxData[5] = {0};
  HAL_I2C_Master_Receive(&hi2c2, SLAVE_ADDR, RxData, 3, 1000);
  int16_t encoder_val = (RxData[0] << 8) | (RxData[1] & 0xFF);
  bool switch_val = RxData[2];
  if (switch_val) {
    DFPlayerMini.playFromMP3Folder(1);
  }
  asm("nop");
}
static void run(void) {
  if (QUERY_ENCODER) {
    QUERY_ENCODER = false;
    query_encoder();
  }
  //	clock_view->render();
  //	uint8_t TX_Data[2] = {0x05, 0xE2};//"he";
  //	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,SET);
  //	HAL_Delay(100);
  //	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,RESET);
  //	if(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_READY){
  //		if(HAL_SPI_Transmit(&hspi1, TX_Data, sizeof(TX_Data), 1000) !=
  // HAL_OK){ 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,SET);
  //		}
  //	}
  //	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,SET);
  //	HAL_Delay(1000);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (htim == &htim3) {
    Timer.tick();
  }
}

const struct application Application = {
    .init = init,
    .run = run,
};
