#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Control.h"
#include "DeviceManager.h"
#include "Point.h"
#include "Tetris.h"
#include "TickTimer.h"

#define GAME_UPDATE_SPEED 100
#define INPUT_UPDATE_SPEED 10
#define WALLKICK_ENABLE true

void Initialize();
void SetOptions(int argc, char* argv[]);

void UpdateGame();

void UpdateInputAsync();
void* UpdateSwitchInput(void* inputManager);

void DrawMap();
void DrawNextBlock();

// Tetris
TetrisGame Tetris;
TickTimer GameUpdateTimer;

// Input
InputManager Input;
TickTimer InputUpdateTimer;
pthread_t InputUpdateThread;

// Device
unsigned char DotMatrix[10][7];

int main(int argc, char* argv[])
{
	OpenDevice();

	Initialize();
	SetOptions(argc, argv);

	RunTetris(&Tetris);
	UpdateInputAsync();
	UpdateGame();

	CloseDevice();

	return 0;
}

void Initialize()
{
	InitializeTetris(&Tetris);
	InitializeTickTimer(&GameUpdateTimer, GAME_UPDATE_SPEED);

	InitializeInputManager(&Input);
	InitializeTickTimer(&InputUpdateTimer, INPUT_UPDATE_SPEED);
	InputUpdateThread = pthread_create(&InputUpdateThread, NULL, UpdateSwitchInput, NULL);
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
		WaitNextTick(&GameUpdateTimer);

		inputInfo = Dequeue(&Input.InputQueue);
		InputTest(&Tetris, &inputInfo);

		UpdateTetris(&Tetris);

		DrawMap();
		DrawNextBlock();

		if (IsTetrisGameOver(&tetris) == true)
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

void DrawMap()
{
	int row, col;
	bool renderedMap[MAP_ROW][MAP_COL];
	unsigned char convertedMap[MAP_ROW][MAP_COL];

	RenderToBoolMap(&Tetris.GameMap, renderedMap);

	for (row = 0; row < MAP_ROW; row++)
	{
		for (col = 0; col < MAP_COL; col++)
		{
			convertedMap[row][col] = renderedMap[row][col] == true ? 1 : 0;
		}
	}

	SetDotMatrix(convertedMap);
}

void DrawNextBlock()
{
	unsigned char renderedNextBlock = 0;

	RenderNextBlock(&Tetris.GameMap, &renderedNextBlock);
	SetLED(renderedNextBlock);
}