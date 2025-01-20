
#pragma once

#include "View.h"
#include <string.h>
#include "math.h"
#include "time.h"
#include "DateHelper.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct clockview
{
	View*(*init)(void);
};
extern const struct clockview ClockView;
