#include "WeatherView.h"

static View view;

static void write_text(int xoff, int yoff, char* fontname, char* text) {
  gCoord swidth, sheight;
  swidth = gdispGetWidth();
  sheight = gdispGetHeight();

  font_t loaded_font = gdispOpenFont(fontname);
  int text_width = gdispGetStringWidth(text, loaded_font) + 1;
  int text_height = gdispGetFontMetric(loaded_font, fontHeight) + 1;
  gdispDrawStringBox(xoff, yoff, text_width, text_height, text, loaded_font,
                     White, justifyCenter);
}
static void render() {
  gCoord swidth, sheight;
  swidth = gdispGetWidth();
  sheight = gdispGetHeight();
  gdispClear(Black);

  // Last Updated
  font_t DejaVuSans10 = gdispOpenFont("DejaVuSans10");
  char title_string[] = "Updated: Oct 15 2:07 PM";
  int title_width = gdispGetStringWidth(title_string, DejaVuSans10) + 1;
  int title_height = gdispGetFontMetric(DejaVuSans10, fontHeight) + 1;
  gdispDrawStringBox((swidth / 2) - (title_width / 2) + 8, 5, title_width,
                     title_height, title_string, DejaVuSans10, White,
                     justifyCenter);

  font_t DejaVuSans20 = gdispOpenFont("DejaVuSans20");
  char time_string[] = "2:17 PM";
  int time_width = gdispGetStringWidth(time_string, DejaVuSans20) + 1;
  int time_height = gdispGetFontMetric(DejaVuSans20, fontHeight) + 1;
  gdispDrawStringBox((swidth / 2) - (time_width / 2) + 8, 20, time_width,
                     time_height, time_string, DejaVuSans20, White,
                     justifyCenter);

  // ICON
  gdispImage weather_img;
  gdispImageOpenFile(&weather_img, "drizzle_48_48.bmp");
  gdispImageDraw(&weather_img, 15, 50, 48, 48, 0, 0);
  gdispImageClose(&weather_img);

  font_t DejaVuSans36 = gdispOpenFont("DejaVuSans36");
  char weather_string[] = "Clear Sky";
  int weather_width = gdispGetStringWidth(weather_string, DejaVuSans36) + 1;
  int weather_height = gdispGetFontMetric(DejaVuSans36, fontHeight) + 1;
  gdispDrawStringBox((swidth / 2) - (weather_width / 2) + 30, 60, weather_width,
                     weather_height, weather_string, DejaVuSans36, White,
                     justifyCenter);

  write_text(10,120,"DejaVuSans12","Low: 60F");
  write_text(10,140,"DejaVuSans12","Hi: 70F");

  write_text(70,120,"DejaVuSans12","Rise: 8:00AM");
  write_text(70,140,"DejaVuSans12","Set: 7:00PM");
  gdispGFlush(gdispGetDisplay(0));
}

static View* init() {
  view.render = render;

  return &view;
}

const struct weatherview WeatherView = {
    .init = init,
};
