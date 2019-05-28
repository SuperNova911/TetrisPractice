#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Control.h"
#include "DeviceManager.h"
#include "Tetris.h"
#include "TickTimer.h"
#include "Point.h"

void GetUserInput(InputCollection* inputCollection);
void* GetUserInput_Thread(void* inputManager);

unsigned char DotMatrix[10][7];

int main()
{
	TickTimer updateTimer;
	TetrisGame tetris;
	bool map[MAP_ROW][MAP_COL];
	int row, col;
    pthread_t inputThread;
    int status;
    InputManager inputManager;
    InputInfo inputInfo;

	OpenDevice();

	InitializeTetris(&tetris);
	InitializeTickTimer(&updateTimer, 100);
    InitializeInputManager(&inputManager);

	RunTetris(&tetris);

    inputThread = pthread_create(&inputThread, NULL, GetUserInput_Thread, (void *)&inputManager);
    if (inputThread < 0)
    {
        printf("Failed to create UserInput thread, id: '%d'\n", (int)inputThread);
    }

    pthread_join(inputThread, (void **)&status);

	while (true)
	{
		WaitNextTick(&updateTimer);

        inputInfo = Dequeue(&inputManager.InputQueue);
        InputTest(&tetris, &inputInfo);

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

void* GetUserInput_Thread(void* inputManager)
{
    InputManager* manager = (InputManager *)inputManager;
    unsigned char switchStatus[INPUT_SOURCE_NUMBER];
    TickTimer inputTimer;

    InitializeTickTimer(&inputTimer, 10);
    memset(switchStatus, 0, sizeof(switchStatus));

    while (true)
    {
        WaitNextTick(&inputTimer);
        GetSwitchStatus(switchStatus);
        HandleInput(manager, switchStatus);
    }
}
