#pragma once

#include <stdbool.h>
#include "DeviceManager.h"
#include "TickTimer.h"

#define DOT_BIG_FONT_ROW 10
#define DOT_BIG_FONT_COL 7
#define DOT_SMALL_FONT_ROW 5
#define DOT_SMALL_FONT_COL 3
#define DOT_SCORE_FONT_ROW 5
#define DOT_SCORE_FONT_COL 24
#define BLINK_DELAY 500

typedef struct DotAnimator_t
{
	TickTimer BlinkTimer;
} DotAnimator;

void InitializeDotAnimator(DotAnimator* animator);
void DrawLevel(DotAnimator* animator, unsigned char matrix[][DOT_MATRIX_COL], unsigned int level);
void DrawPause(DotAnimator* animator, unsigned char matrix[][DOT_MATRIX_COL]);
void DrawScore(DotAnimator* animator, unsigned char matrix[][DOT_MATRIX_COL], unsigned int score);

extern const unsigned char DOT_BIG_NUMBER_FONT[10][10];
extern const unsigned char DOT_FULL[10];
extern const unsigned char DOT_BLANK[10];
extern const bool DOT_SMALL_NUMBER_FONT[10][DOT_SMALL_FONT_ROW][DOT_SMALL_FONT_COL];
extern const bool DOT_SCORE_FONT[DOT_SCORE_FONT_ROW][DOT_SCORE_FONT_COL];