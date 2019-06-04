// #include <process.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <Windows.h>
#include "Control.h"
#include "DeviceManager.h"
#include "DotAnimator.h"
#include "Point.h"
#include "Tetris.h"
#include "TickTimer.h"

#define GAME_UPDATE_SPEED 100
#define INPUT_UPDATE_SPEED 1
#define WALLKICK_ENABLE true

void Initialize();
void SetOptions(int argc, char* argv[]);

void UpdateGame();

void UpdateInputAsync();
void* UpdateSwitchInput(void* inputManager);

void DrawMap(unsigned char map[][MAP_COL]);
void DrawNextBlock();
// void UpdateDevice();
// unsigned _stdcall UpdateSwitchInput(void* arg);

// Tetris
TetrisGame Tetris;
TickTimer GameUpdateTimer;
unsigned int CurrentLevel;

// Input
InputManager Input;
TickTimer InputUpdateTimer;
pthread_t InputUpdateThread;

// Device
unsigned char DotMatrix[DOT_MATRIX_ROW][DOT_MATRIX_COL];

int main(int argc, char* argv[])
{
	OpenDevice();

	Initialize();
	SetOptions(argc, argv);

	RunTetris(&Tetris);
	RunTimer(&GameUpdateTimer);
	RunTimer(&InputUpdateTimer);

	UpdateInputAsync();
	UpdateGame();

	CloseDevice();

	return 0;
}

void Initialize()
{
	InitializeTetris(&Tetris);
	InitializeTickTimer(&GameUpdateTimer, GAME_UPDATE_SPEED);
	CurrentLevel = 0;

	InitializeInputManager(&Input);
	InitializeTickTimer(&InputUpdateTimer, INPUT_UPDATE_SPEED);
	InputUpdateThread = pthread_create(&InputUpdateThread, NULL, UpdateSwitchInput, NULL);
	// _beginthreadex(NULL, 0, UpdateSwitchInput, 0, 0, NULL);
}

void SetOptions(int argc, char* argv[])
{
	Tetris.GameMap.AllowWallKick = WALLKICK_ENABLE;
}

void UpdateGame()
{
	InputInfo inputInfo;

	while (true)
	{
		//WaitNextTick(&GameUpdateTimer);
		if (IsTimerReady(&GameUpdateTimer) == false)
		{
			continue;
		}

		inputInfo = Dequeue(&Input.InputQueue);
		InputTest(&Tetris, &inputInfo);

		UpdateTetris(&Tetris);

		RenderTetrisMap(&Tetris, DotMatrix);
		DrawMap(DotMatrix);
		DrawNextBlock();
		// UpdateDevice();

		if (IsTetrisGameOver(&Tetris) == true)
		{
			break;
		}
	}
}

void UpdateInputAsync()
{
	int result = 0;
	pthread_join(InputUpdateThread, (void**)& result);
}

void* UpdateSwitchInput(void* unused)
{
    unsigned char switchStatus[PUSH_SWITCH_NUMBER];
    memset(switchStatus, 0, sizeof(switchStatus));

    while (true)
    {
        WaitNextTick(&InputUpdateTimer);
        GetSwitchStatus(switchStatus);
        HandleInput(&Input, switchStatus);
    }
}

void DrawMap(unsigned char map[][MAP_COL])
{
	SetDotMatrix(map);
}

void DrawNextBlock()
{
	unsigned char renderedNextBlock = 0;

	RenderNextBlock(&Tetris.GameMap, &renderedNextBlock);
	SetLED(renderedNextBlock);
}

// void UpdateDevice()
// {
// 	static const char *T = "●";
// 	static const char *F = "○";
// 	static int UPDATE_COUNTER = 0;

// 	unsigned char led = GetLED();
// 	unsigned char pushSwitch[PUSH_SWITCH_NUMBER];
// 	GetSwitchStatus(pushSwitch);
// 	unsigned char dotMatrix[DOT_MATRIX_ROW][DOT_MATRIX_COL];
// 	GetDotMatrix(dotMatrix);

// 	system("cls");
// 	printf("LED\n");
// 	for (int row = 1; row >= 0; row--)
// 	{
// 		for (int col = 3; col >= 0; col--)
// 		{
// 			if (led & 0x1 << (row * 4 + col))
// 			{
// 				printf("%s", T);
// 			}
// 			else
// 			{
// 				printf("%s", F);
// 			}
// 		}
// 		printf("\n");
// 	}
// 	printf("\n");

// 	printf("PushSwitch\n");
// 	for (int index = 0; index < PUSH_SWITCH_NUMBER; index++)
// 	{
// 		printf("[%d:%d] ", index + 1, pushSwitch[index]);
// 	}
// 	printf("\n");
// 	printf("\n");

// 	printf("DotMatrix\n");
// 	for (int row = 0; row < DOT_MATRIX_ROW; row++)
// 	{
// 		for (int col = 0; col < DOT_MATRIX_COL; col++)
// 		{
// 			if (dotMatrix[row][col] == 0)
// 			{
// 				printf("%s", F);
// 			}
// 			else
// 			{
// 				printf("%s", T);
// 			}
// 		}
// 		printf("\n");
// 	}
// 	printf("\n");
// 	printf("Update Counter: '%d'\n", UPDATE_COUNTER++);
// }

// unsigned _stdcall UpdateSwitchInput(void* arg)
// {
// 	unsigned char switchStatus[PUSH_SWITCH_NUMBER];
// 	memset(switchStatus, 0, sizeof(switchStatus));

// 	while (true)
// 	{
// 		if (IsTimerReady(&InputUpdateTimer) == false)
// 		{
// 			continue;
// 		}

// 		GetSwitchStatus(switchStatus);
// 		HandleInput(&Input, switchStatus);
// 	}
// }