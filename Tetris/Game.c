//#include <process.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <Windows.h>
#include "driverManager.h"
#include "Tetris.h"
#include "TickTimer.h"
#include "Point.h"

//void UpdateDevice();
//unsigned _stdcall Thread_Device(void* arg);

void GetUserInput(InputCollection* inputCollection);

int DotMatrix[10][7];

int main()
{
	TickTimer updateTimer;
	TetrisGame tetris;
	InputCollection userInput;
	bool map[MAP_ROW][MAP_COL];
	int row, col;

	OpenDriver_kappa();

	InitializeTetris(&tetris);
	InitializeTickTimer(&updateTimer, 100);

	RunTetris(&tetris);
//	_beginthreadex(NULL, 0, Thread_Device, 0, 0, NULL);
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

	CloseDriver_kappa();

	return 0;
}

void GetUserInput(InputCollection* inputCollection)
{
	int index;
	unsigned char switchStatus[INPUT_SOURCE_NUMBER];

	GetSwitchStatus(switchStatus);

	for (index = 0; index < INPUT_SOURCE_NUMBER; index++)
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

/* unsigned _stdcall Thread_Device(void* arg)
 {
 	while (true)
 	{
 		Sleep(10);
 		UpdateDevice();
 	}
 }

 void UpdateDevice()
 {
 	static int UPDATE_COUNTER = 0;

 	unsigned char led = GetLED();
 	unsigned char pushSwitch[9];
 	GetSwitchStatus(pushSwitch);
 	unsigned char dotMatrix[10][7];
 	GetDotMatrix(dotMatrix);

 	system("cls");
 	printf("LED\n");
 	for (int row = 1; row >= 0; row--)
 	{
 		for (int col = 3; col >= 0; col--)
 		{
 			if (led & 0x1 << (row * 4 + col))
 			{
 				printf("X");
 			}
 			else
 			{
 				printf("O");
 			}
 		}
 		printf("\n");
 	}
 	printf("\n");

 	printf("PushSwitch\n");
 	for (int index = 0; index < 9; index++)
 	{
 		printf("[%d:%d] ", index + 1, pushSwitch[index]);
 	}
 	printf("\n");
 	printf("\n");

 	printf("DotMatrix\n");
 	for (int row = 0; row < 10; row++)
 	{
 		for (int col = 0; col < 7; col++)
 		{
 			if (dotMatrix[row][col] == 0)
 			{
				printf("O ");
 			}
 			else
 			{
				printf("X ");
 			}
 		}
 		printf("\n");
 	}
 	printf("\n");
 	printf("Update Counter: '%d'\n", UPDATE_COUNTER++);
 }*/
