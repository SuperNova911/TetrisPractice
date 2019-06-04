#pragma once

#include <stdbool.h>
#include "DeviceManager.h"

#define DOT_BIG_FONT_ROW 10
#define DOT_BIG_FONT_COL 7
#define DOT_SMALL_FONT_ROW 5
#define DOT_SMALL_FONT_COL 3

extern const unsigned char DOT_BIG_NUMBER_FONT[10][10];
extern const unsigned char DOT_FULL[10];
extern const unsigned char DOT_BLANK[10];

void ShowLevel(unsigned char matrix[][DOT_MATRIX_COL], unsigned int level);