#pragma once

#include <stdbool.h>
#include "DeviceManager.h"

#define DOT_BIG_FONT_ROW 10
#define DOT_BIG_FONT_COL 7
#define DOT_SMALL_FONT_ROW 5
#define DOT_SMALL_FONT_COL 3
#define DOT_SCORE_FONT_ROW 5
#define DOT_SCORE_FONT_COL 24

extern const unsigned char DOT_BIG_NUMBER_FONT[10][10];
extern const unsigned char DOT_FULL[10];
extern const unsigned char DOT_BLANK[10];
extern const bool DOT_SMALL_NUMBER_FONT[10][DOT_SMALL_FONT_ROW][DOT_SMALL_FONT_COL];
extern const bool DOT_SCORE_FONT[DOT_SCORE_FONT_ROW][DOT_SCORE_FONT_COL];
extern const bool DOT_SMALL_ALPHABET_L_FONT[DOT_SMALL_FONT_ROW][DOT_SMALL_FONT_COL];
extern const bool DOT_PAUSE_FONT[DOT_MATRIX_ROW][DOT_MATRIX_COL];
