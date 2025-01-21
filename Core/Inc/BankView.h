#pragma once

#include <stdbool.h>
#include "View.h"

struct bankview {
  View* (*init)(void);
};
extern const struct bankview BankView;
