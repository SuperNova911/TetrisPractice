#include <stdbool.h>
#include <string.h>
#include "DotAnimator.h"

const unsigned char DOT_BIG_NUMBER_FONT[10][10] = {
	{ 0x3e,0x7f,0x63,0x73,0x73,0x6f,0x67,0x63,0x7f,0x3e },    // 0
	{ 0x0c,0x1c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x1e },    // 1
	{ 0x7e,0x7f,0x03,0x03,0x3f,0x7e,0x60,0x60,0x7f,0x7f },    // 2
	{ 0xfe,0x7f,0x03,0x03,0x7f,0x7f,0x03,0x03,0x7f,0x7e },    // 3
	{ 0x66,0x66,0x66,0x66,0x66,0x66,0x7f,0x7f,0x06,0x06 },    // 4
	{ 0x7f,0x7f,0x60,0x60,0x7e,0x7f,0x03,0x03,0x7f,0x7e },    // 5
	{ 0x60,0x60,0x60,0x60,0x7e,0x7f,0x63,0x63,0x7f,0x3e },    // 6
	{ 0x7f,0x7f,0x63,0x63,0x03,0x03,0x03,0x03,0x03,0x03 },    // 7
	{ 0x3e,0x7f,0x63,0x63,0x7f,0x7f,0x63,0x63,0x7f,0x3e },    // 8
	{ 0x3e,0x7f,0x63,0x63,0x7f,0x3f,0x03,0x03,0x03,0x03 }     // 9
};

const unsigned char DOT_FULL[10] = {
	0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f
};

const unsigned char DOT_BLANK[10] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const bool DOT_SMALL_NUMBER_FONT[10][DOT_SMALL_FONT_ROW][DOT_SMALL_FONT_COL] =
{
	{{ 1, 1, 1 },	// 0
	{ 1, 0, 1 },
	{ 1, 0, 1 },
	{ 1, 0, 1 },
	{ 1, 1, 1 }},

	{{ 0, 1, 0 },	// 1
	{ 1, 1, 0 },
	{ 0, 1, 0 },
	{ 0, 1, 0 },
	{ 1, 1, 1 }},

	{{ 1, 1, 1 },	// 2
	{ 0, 0, 1 },
	{ 1, 1, 1 },
	{ 1, 0, 0 },
	{ 1, 1, 1 }},

	{{ 1, 1, 1 },	// 3
	{ 0, 0, 1 },
	{ 0, 1, 1 },
	{ 0, 0, 1 },
	{ 1, 1, 1 }},

	{{ 1, 0, 1 },	// 4
	{ 1, 0, 1 },
	{ 1, 1, 1 },
	{ 0, 0, 1 },
	{ 0, 0, 1 }},

	{{ 1, 1, 1 },	//5
	{ 1, 0, 0 },
	{ 1, 1, 1 },
	{ 0, 0, 1 },
	{ 1, 1, 1 }},

	{{ 1, 1, 1 },	// 6
	{ 1, 0, 0 },
	{ 1, 1, 1 },
	{ 1, 0, 1 },
	{ 1, 1, 1 }},

	{{ 1, 1, 1 },	// 7
	{ 1, 0, 0 },
	{ 1, 1, 1 },
	{ 0, 0, 1 },
	{ 1, 1, 1 }},

	{{ 1, 1, 1 },	// 8
	{ 1, 0, 1 },
	{ 1, 1, 1 },
	{ 1, 0, 1 },
	{ 1, 1, 1 }},

	{{ 1, 1, 1 },	// 9
	{ 1, 0, 1 },
	{ 1, 1, 1 },
	{ 0, 0, 1 },
	{ 1, 1, 1 }},
};

const bool DOT_SMALL_ALPHABET_L_FONT[DOT_SMALL_FONT_ROW][DOT_SMALL_FONT_COL] =
{
	{ 1, 0, 0 },
	{ 1, 0, 0 },
	{ 1, 0, 0 },
	{ 1, 0, 0 },
	{ 1, 1, 1 },
};

void ShowLevel(unsigned char matrix[][DOT_MATRIX_COL], unsigned int level)
{
	int row, col;
	const static int ALPHABET_X = 0;
	const static int ALPHABET_Y = 1;
	const static int NUMBER_X = 4;
	const static int NUMBER_Y = 1;

	for (row = 1; row <= 7; row++)
	{
		memset(matrix[row], 0, sizeof(unsigned char) * DOT_MATRIX_COL);
	}

	for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
	{
		for (col = 0; col < DOT_SMALL_FONT_COL; col++)
		{
			matrix[row + ALPHABET_Y][col + ALPHABET_X] = DOT_SMALL_ALPHABET_L_FONT[row][col];
			matrix[row + NUMBER_Y][col + NUMBER_X] = DOT_SMALL_NUMBER_FONT[level][row][col];
		}
	}
}