#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DeviceManager.h"
#include "Tetris.h"
#include "TickTimer.h"
#include "Point.h"

void GetUserInput(InputCollection* inputCollection);

unsigned char DotMatrix[10][7];

int main()
{
	TickTimer updateTimer;
	TetrisGame tetris;
	InputCollection userInput;
	bool map[MAP_ROW][MAP_COL];
	int row, col;

	OpenDevice();

	InitializeTetris(&tetris);
	InitializeTickTimer(&updateTimer, 100);

	RunTetris(&tetris);

	while (true)
	{
		WaitNextTick(&updateTimer);

	    GetUserInput(&userInput);
		ReadUserInput(&tetris, &userInput);

		UpdateTetris(&tetris);

		RenderToBoolMap(&tetris.GameMap, map);

		for (row = 0; row < MAP_ROW; row++)
		{
			for (col = 0; col < MAP_COL; col++)
			{
				DotMatrix[row][col] = map[row][col] == true ? 1 : 0;
			}
		}
		SetDotMatrix(DotMatrix);
	}

	CloseDevice();

	return 0;
}

void GetUserInput(InputCollection* inputCollection)
{
	int index;
	unsigned char switchStatus[PUSH_SWITCH_NUMBER];

    GetSwitchStatus(switchStatus);

	for (index = 0; index < PUSH_SWITCH_NUMBER; index++)
	{
		inputCollection->Input[index].Command = index;

		if (switchStatus[index] > 0)
		{
			inputCollection->Input[index].Type = InputType_Click;
		}
		else
		{
			inputCollection->Input[index].Type = InputType_None;
		}
	}
}
