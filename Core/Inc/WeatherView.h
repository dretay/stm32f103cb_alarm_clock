#pragma once

#include <stdbool.h>
#include "View.h"

struct weatherview {
  View* (*init)(void);
};
extern const struct weatherview WeatherView;
