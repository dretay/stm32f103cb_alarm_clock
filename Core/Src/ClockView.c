#include "ClockView.h"

static View view;
const int clock_radius = 43; //global adjustment for the clock, default 90
const int clock_xoffset = 88;  //global adjustment for the clock, default 120
const int clock_yoffset = 65; //global adjustment for the clock, default 195
int point_xpos = 0;
int point_ypos = 0;
int sec_arrow_xpos = 0;
int sec_arrow_ypos  = 0;
int min_arrow_xpos  = 0;
int min_arrow_ypos  = 0;
int hour_arrow_xpos = 0;
int hour_arrow_ypos = 0;

int copy_sec_arrow_xpos  = 0;
int copy_sec_arrow_ypos  = 0;
int copy_min_arrow_xpos  = 0;
int copy_min_arrow_ypos  = 0;
int copy_hour_arrow_xpos  = 0;
int copy_hour_arrow_ypos  = 0;
float alfa;

static void draw_hour(char* hour_string, int x, int y)
{
	coord_t width = gdispGetWidth();
	coord_t height = gdispGetHeight();

	font_t font = gdispOpenFont("DejaVuSansBold12");
	int hour_width = gdispGetStringWidth(hour_string, font) + 2;
	int hour_height = gdispGetFontMetric(font, fontHeight) + 1;
	int hour_x = x - (hour_width / 8)+1;
	int hour_y = y-1;

	if (strlen(hour_string) > 1 )
	{
		hour_x -= 3;
	}

	gdispDrawStringBox(hour_x, hour_y, hour_width, hour_height, hour_string, font, White, justifyCenter);

}
void SetLines(int xl1pos, int yl1pos, int xl2pos, int yl2pos) {
	//	tft.drawLine(xl1pos, yl1pos, xl2pos, yl2pos, color);
		gdispDrawLine(xl1pos, yl1pos, xl2pos, yl2pos, White);
}
void SetThickLines(int xl1pos, int yl1pos, int xl2pos, int yl2pos) {
	//	tft.drawLine(xl1pos, yl1pos, xl2pos, yl2pos, color);
		gdispDrawThickLine(xl1pos, yl1pos, xl2pos, yl2pos, White, 3, TRUE);
}
static void draw_clock(int hour, int minute, int second)
{

	alfa = (270 + (30 * hour + (0.5 * minute))) * 0.017453293; // pi/180=0.017453293
	hour_arrow_xpos = (cos(alfa) * clock_radius * 0.7) + clock_xoffset;
	hour_arrow_ypos = (sin(alfa) * clock_radius * 0.7) + clock_yoffset;

	alfa = (270 + (6 * minute)) * 0.017453293; // pi/180=0.017453293
	min_arrow_xpos = (cos(alfa) * clock_radius * 0.9) + clock_xoffset;
	min_arrow_ypos = (sin(alfa) * clock_radius * 0.9) + clock_yoffset;

	alfa = (270 + (6 * second)) * 0.017453293; // pi/180=0.017453293
	sec_arrow_xpos = (cos(alfa) * clock_radius * 0.9) + clock_xoffset;
	sec_arrow_ypos = (sin(alfa) * clock_radius * 0.9) + clock_yoffset;

	if ((copy_hour_arrow_xpos != hour_arrow_xpos) || (copy_hour_arrow_ypos != hour_arrow_ypos)) {
		//SetLines(clock_xoffset, clock_yoffset, copy_hour_arrow_xpos, copy_hour_arrow_ypos);
	}

	if ((copy_min_arrow_xpos != min_arrow_xpos) || (copy_min_arrow_ypos != min_arrow_ypos)) {
		//SetLines(clock_xoffset, clock_yoffset, copy_min_arrow_xpos, copy_min_arrow_ypos);
	}

	//SetLines(clock_xoffset, clock_yoffset, copy_sec_arrow_xpos, copy_sec_arrow_ypos);
	SetThickLines(clock_xoffset, clock_yoffset, hour_arrow_xpos, hour_arrow_ypos);
	copy_hour_arrow_xpos = hour_arrow_xpos;
	copy_hour_arrow_ypos = hour_arrow_ypos;

	SetThickLines(clock_xoffset, clock_yoffset, min_arrow_xpos, min_arrow_ypos);
	copy_min_arrow_xpos = min_arrow_xpos;
	copy_min_arrow_ypos = min_arrow_ypos;

	SetLines(clock_xoffset, clock_yoffset, sec_arrow_xpos, sec_arrow_ypos);
	copy_sec_arrow_xpos = sec_arrow_xpos;
	copy_sec_arrow_ypos = sec_arrow_ypos;

}
void SetPoint(int xppos, int yppos) {
	gdispDrawPixel(xppos, yppos, White);
}
void SetFilledCircle(int xcpos, int ycpos, int radius) {
	gdispFillCircle(xcpos, ycpos, radius, White);
}
void face() {

	for (int i = 0; i <= 59; i++) {
		alfa = 6 * i * 0.017453293;   // pi/180=0.017453293
		point_xpos = (cos(alfa) * clock_radius) + clock_xoffset;
		point_ypos = (sin(alfa) * clock_radius) + clock_yoffset;
		SetPoint(point_xpos, point_ypos);
		if ((i % 5) == 0)
		{
			SetFilledCircle(point_xpos, point_ypos, 2);
			alfa = 6 * (i+45) * 0.017453293;    // pi/180=0.017453293
			point_xpos = (cos(alfa) * (clock_radius+12)) + clock_xoffset-5;
			point_ypos = (sin(alfa) * (clock_radius+12)) + clock_yoffset-5;

			char buffer[3] = {'\0'};
			int hour = (i / 5);
			if (hour == 0)
			{
				hour = 12;
			}
			itoa(hour, buffer, 10);
			draw_hour(buffer, point_xpos, point_ypos);
		}
	}
	gdispDrawCircle(clock_xoffset-1, clock_yoffset-1, clock_radius+20, White);
}



static void render()
{
    coord_t width = gdispGetWidth();
    coord_t height = gdispGetHeight();
    RTC_TimeTypeDef currentTime;
    RTC_DateTypeDef currentDate;
	HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);

    gdispClear(Black);
    draw_clock(currentTime.Hours, currentTime.Minutes, currentTime.Seconds);
    face();

    font_t DejaVuSans32 = gdispOpenFont("DejaVuSans32");

    char time_string[10] = {'\0'};
    int showHours =currentTime.Hours;
    if(showHours == 0){
    	showHours = 12;
    }
    else if(showHours> 12){
    	showHours -= 12;
    }
    snprintf(time_string,10,"%d:%02d %s",showHours,currentTime.Minutes,currentTime.Hours >12?"PM":"AM");

    int time_width = gdispGetStringWidth(time_string, DejaVuSans32) + 1;
    int time_height = gdispGetFontMetric(DejaVuSans32, fontHeight) + 1;

    gdispDrawStringBox((width / 2) - (time_width / 2) + 8,
                    height - time_height + 5,
                    time_width,
                    time_height,
                    time_string,
                    DejaVuSans32,
                    White,
                    justifyCenter);
    gdispGFlush(gdispGetDisplay(0));
}

static View* init()
{
	view.render = render;
    sec_arrow_xpos = clock_xoffset;
    sec_arrow_ypos = clock_yoffset - clock_radius;
    min_arrow_xpos = clock_xoffset;
    min_arrow_ypos = clock_yoffset - clock_radius;
    hour_arrow_xpos = clock_xoffset;
    hour_arrow_ypos = clock_yoffset - clock_radius;
	return &view;
}

const struct clockview ClockView= {
	.init = init,
};

