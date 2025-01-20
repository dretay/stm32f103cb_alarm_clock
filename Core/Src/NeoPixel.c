#include "NeoPixel.h"

static PixelRGBW_t pixel[NUM_PIXELS] = {0};
static uint32_t dmaBuffer[DMA_BUFF_SIZE] = {0};
static uint32_t *pBuff;
static uint8_t brightness = 0;
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch1;
static volatile bool busy;
static int scheduled_effect = 0;
extern TIM_HandleTypeDef htim3;

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
//{
//    if(htim == &htim3){
//    	Timer.tick();
//    }
//}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
  busy=false;
}

static void setPixelColor(uint16_t n, PixelRGBW_t fade_color) {
	pixel[n].color.r = fade_color.color.r;
	pixel[n].color.g = fade_color.color.g;
	pixel[n].color.b = fade_color.color.b;
	pixel[n].color.w = 0;
}
static void setPixelDataFromRGB(uint16_t n, uint32_t _data) {
	uint8_t r = (uint8_t)(_data >> 16), g = (uint8_t)(_data >> 8), b = (uint8_t)_data;
	pixel[n].color.r = r;
	pixel[n].color.g = g;
	pixel[n].color.b = b;
	pixel[n].color.w = 0;
}
static bool show(){
	if(!busy){
		pBuff = dmaBuffer;
		int i, j;
		for (i = 0; i < NUM_PIXELS; i++){
			//since brightness can be set independent of color, must apply before render
			pixel[i].color.g = (pixel[i].color.g*brightness >> 8);
			pixel[i].color.r = (pixel[i].color.r*brightness >> 8);
			pixel[i].color.b = (pixel[i].color.b*brightness >> 8);
			pixel[i].color.w = (pixel[i].color.w*brightness >> 8);
			for (j = 31; j >= 0; j--){
				if ((pixel[i].data >> j) & 0x01){
					*pBuff = NEOPIXEL_ONE;
				}
				else{
					*pBuff = NEOPIXEL_ZERO;
				}
				pBuff++;
			}
		}
		dmaBuffer[DMA_BUFF_SIZE - 1] = 0; // last element must be 0!

		HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, dmaBuffer, DMA_BUFF_SIZE);
		return true;
	}
	return false;
}
static void clear(){
	memset(pixel,0,sizeof(pixel));
}
static void begin(){
	brightness = 100;
	busy = false;

}
static void setBrightness(uint8_t _brightness){
	brightness = _brightness;
}

//fade effect
static uint8_t fade_brightness =0;
static PixelRGBW_t fade_color;
static void _neopixel_fade(){
	for (int i = 0; i < NUM_PIXELS; i++){
		setPixelColor(i, fade_color);
	}
	show();
	setBrightness(fade_brightness++);
}
static void fade_effect(uint8_t r, uint8_t g, uint8_t b, uint8_t speed){
	fade_color.color.r = r;
	fade_color.color.g = g;
	fade_color.color.b = b;
	scheduled_effect = Timer.every(speed,&_neopixel_fade);
}

//color wipe
static PixelRGBW_t wipe_color;
static int wipe_wait=0;
static int wipe_index =0;
static void _color_wipe_clear(){
	clear();
	wipe_index=0;
	show();
}
static void _color_wipe(){
	setPixelColor(wipe_index++,wipe_color);
	show();

}
static void color_wipe(uint8_t r, uint8_t g, uint8_t b, int wait){
	wipe_color.color.r = r;
	wipe_color.color.g = g;
	wipe_color.color.b = b;
	wipe_wait = wait;
	Timer.every((wait*NUM_PIXELS+1),&_color_wipe_clear);
	Timer.every(wait,&_color_wipe);
}

//theatre chase
static PixelRGBW_t chase_color;
static int chase_wait=0;
static void theatre_chase(uint8_t r, uint8_t g, uint8_t b, int wait){
	chase_color.color.r = r;
	chase_color.color.g = g;
	chase_color.color.b = b;
	chase_wait = wait;
	for(int a=0; a<10; a++) {
		for(int b=0; b<3; b++) {
			clear();
			for(int c=b; c<NUM_PIXELS; c += 3) {
				setPixelColor(c, chase_color);
			}
			show();
			HAL_Delay(wait);
		}
	}
}

static uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val) {

  uint8_t r, g, b;


  hue = (hue * 1530L + 32768) / 65536;

  if (hue < 510) { // Red to Green-1
    b = 0;
    if (hue < 255) { //   Red to Yellow-1
      r = 255;
      g = hue;       //     g = 0 to 254
    } else {         //   Yellow to Green-1
      r = 510 - hue; //     r = 255 to 1
      g = 255;
    }
  } else if (hue < 1020) { // Green to Blue-1
    r = 0;
    if (hue < 765) { //   Green to Cyan-1
      g = 255;
      b = hue - 510;  //     b = 0 to 254
    } else {          //   Cyan to Blue-1
      g = 1020 - hue; //     g = 255 to 1
      b = 255;
    }
  } else if (hue < 1530) { // Blue to Red-1
    g = 0;
    if (hue < 1275) { //   Blue to Magenta-1
      r = hue - 1020; //     r = 0 to 254
      b = 255;
    } else { //   Magenta to Red-1
      r = 255;
      b = 1530 - hue; //     b = 255 to 1
    }
  } else { // Last 0.5 Red (quicker than % operator)
    r = 255;
    g = b = 0;
  }

  // Apply saturation and value to R,G,B, pack into 32-bit result:
  uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
  uint16_t s1 = 1 + sat;  // 1 to 256; same reason
  uint8_t s2 = 255 - sat; // 255 to 0
  return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
         (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
         (((((b * s1) >> 8) + s2) * v1) >> 8);
}

static void rainbow(int wait){
	for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
	    for(int i=0; i<NUM_PIXELS; i++) {
	      int pixelHue = firstPixelHue + (i * 65536L / NUM_PIXELS);
	      setPixelDataFromRGB(i, ColorHSV(pixelHue, 255,255));
	    }
	    show(); // Update strip with new contents
	    HAL_Delay(wait);  // Pause for a moment
	  }
}

const struct neopixel NeoPixel = {
	.setPixelColor = setPixelColor,
	.show = show,
	.clear = clear,
	.begin = begin,
	.setBrightness = setBrightness,
	.fade_effect = fade_effect,
	.color_wipe = color_wipe,
	.theatre_chase = theatre_chase,
	.rainbow = rainbow,

};
