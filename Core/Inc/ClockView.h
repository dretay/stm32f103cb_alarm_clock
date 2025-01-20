
#pragma once

#include <string.h>
#include "DateHelper.h"
#include "View.h"
#include "math.h"
#include "time.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct clockview {
  View* (*init)(void);
};
extern const struct clockview ClockView;
