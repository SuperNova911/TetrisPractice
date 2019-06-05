#include <getopt.h>
#include <math.h>
#include <process.h>
//#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <Windows.h>
#include "Control.h"
#include "DeviceManager.h"
#include "DotAnimator.h"
#include "Point.h"
#include "Tetris.h"
#include "TickTimer.h"

#define GAME_UPDATE_SPEED 100
#define INPUT_UPDATE_SPEED 1
#define BLINK_DELAY 500
#define WALLKICK_ENABLE true
#define GRAVITY_ENABLE true

void Initialize();
void SetOptions(int argc, char* argv[]);

void UpdateGame();

void UpdateInputAsync();
//void* UpdateSwitchInput(void* inputManager);
unsigned _stdcall UpdateSwitchInput(void* arg);

void DrawMap(unsigned char map[][MAP_COL]);
void UpdateDevice();
void DrawNextBlock();

void DrawLevelUp(unsigned int level);
void DrawPause();
void DrawGameOver();
void DrawFinalScore(unsigned int finalScore);

// Tetris
TetrisGame Tetris;
TickTimer GameUpdateTimer;
unsigned int CurrentLevel;
unsigned int FinalScore;

// Input
InputManager Input;
TickTimer InputUpdateTimer;
//pthread_t InputUpdateThread;

// Device
unsigned char DotMatrix[DOT_MATRIX_ROW][DOT_MATRIX_COL];

// Animation
TickTimer BlinkTimer;

int main(int argc, char* argv[])
{
	OpenDevice();

	Initialize();
	SetOptions(argc, argv);

	RunTetris(&Tetris);
	RunTimer(&GameUpdateTimer);
	RunTimer(&InputUpdateTimer);
	RunTimer(&BlinkTimer);

	UpdateInputAsync();
	UpdateGame();

	DrawGameOver();
	DrawFinalScore(FinalScore);

	CloseDevice();

	return 0;
}

void Initialize()
{
	// Tetris
	InitializeTetris(&Tetris);
	InitializeTickTimer(&GameUpdateTimer, GAME_UPDATE_SPEED);
	CurrentLevel = 0;
	FinalScore = 0;

	// Input
	InitializeInputManager(&Input);
	InitializeTickTimer(&InputUpdateTimer, INPUT_UPDATE_SPEED);
	//InputUpdateThread = pthread_create(&InputUpdateThread, NULL, UpdateSwitchInput, NULL);
	_beginthreadex(NULL, 0, UpdateSwitchInput, 0, 0, NULL);

	// Animation
	InitializeTickTimer(&BlinkTimer, BLINK_DELAY);
}

void SetOptions(int argc, char* argv[])
{
	static struct option gameOption[] =
	{
		{ "no_wallkick", 0, 0, 0 },
		{ "no_gravity", 0, 0, 0 },
		{ 0, 0, 0, 0 },
	};
	int option;
	int optionIndex;
	bool wallKickOption = WALLKICK_ENABLE;
	bool gravityOption = GRAVITY_ENABLE;

	while ((option = getopt_long(argc, argv, "", gameOption, &optionIndex)) != -1)
	{
		switch (option)
		{
		case 0:
			if (strcmp(gameOption[index].name, "no_wallkick") == 0)
			{
				wallKickOption = false;
				break;
			}
			else if (strcmp(gameOption[index].name, "no_gravity") == 0)
			{
				gravityOption = false;
				break;
			}
			printf("SetOptions: Invaild command-line argument, option: '%s'\n",
				gameOption[index].name);
			break;
		default:
			printf("SetOptions: Invaild command-line argument, option: '%c'\n",
				option);
			break;
		}
	}

	Tetris.GameMap.AllowWallKick = wallKickOption;
	Tetris.GameCore.EnableGravity = gravityOption;
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
		ReadUserInput(&Tetris, &inputInfo);

		UpdateTetris(&Tetris);
		RenderTetrisMap(&Tetris, DotMatrix);

		if (CurrentLevel < Tetris.GameInfo.Level)
		{
			CurrentLevel = Tetris.GameInfo.Level;
			DrawLevelUp(CurrentLevel);
		}

		if (Tetris.GameInfo.IsRunning == false)
		{
			DrawPause();
			continue;
		}

		DrawMap(DotMatrix);
		DrawNextBlock();
		//UpdateDevice();

		if (IsTetrisGameOver(&Tetris) == true)
		{
			CurrentLevel = Tetris.GameInfo.Level;
			FinalScore = Tetris.GameInfo.Score;
			break;
		}
	}
}

void UpdateInputAsync()
{
	int result = 0;
	//pthread_join(InputUpdateThread, (void**)& result);
}

//void* UpdateSwitchInput(void* unused)
//{
//    unsigned char switchStatus[PUSH_SWITCH_NUMBER];
//    memset(switchStatus, 0, sizeof(switchStatus));
//
//    while (true)
//    {
//        if (IsTimerReady(&InputUpdateTimer) == false)
//        {
//            continue;
//        }
//
//        GetSwitchStatus(switchStatus);
//        HandleInput(&Input, switchStatus);
//    }
//}

void DrawMap(unsigned char map[][MAP_COL])
{
	SetDotMatrix(map);
	UpdateDevice();
}

void DrawNextBlock()
{
	unsigned char renderedNextBlock = 0;

	RenderNextBlock(&Tetris.GameMap, &renderedNextBlock);
	SetLED(renderedNextBlock);
}

void UpdateDevice()
{
	static const char* T = "●";
	static const char* F = "○";
	static int UPDATE_COUNTER = 0;

	unsigned char led = GetLED();
	unsigned char pushSwitch[PUSH_SWITCH_NUMBER];
	GetSwitchStatus(pushSwitch);
	unsigned char dotMatrix[DOT_MATRIX_ROW][DOT_MATRIX_COL];
	GetDotMatrix(dotMatrix);

	system("cls");
	printf("LED\n");
	for (int row = 1; row >= 0; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (led & 0x1 << (row * 4 + col))
			{
				printf("%s", T);
			}
			else
			{
				printf("%s", F);
			}
		}
		printf("\n");
	}
	printf("\n");

	printf("PushSwitch\n");
	for (int index = 0; index < PUSH_SWITCH_NUMBER; index++)
	{
		printf("[%d:%d] ", index + 1, pushSwitch[index]);
	}
	printf("\n");
	printf("\n");

	printf("DotMatrix\n");
	for (int row = 0; row < DOT_MATRIX_ROW; row++)
	{
		for (int col = 0; col < DOT_MATRIX_COL; col++)
		{
			if (dotMatrix[row][col] == 0)
			{
				printf("%s", F);
			}
			else
			{
				printf("%s", T);
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("Update Counter: '%d'\n", UPDATE_COUNTER++);
}

unsigned _stdcall UpdateSwitchInput(void* arg)
{
	unsigned char switchStatus[PUSH_SWITCH_NUMBER];
	memset(switchStatus, 0, sizeof(switchStatus));

	while (true)
	{
		if (IsTimerReady(&InputUpdateTimer) == false)
		{
			continue;
		}

		GetSwitchStatus(switchStatus);
		HandleInput(&Input, switchStatus);
	}
}

void DrawLevelUp(unsigned int level)
{
	int row, col;
	const static int ALPHABET_X = 0;
	const static int ALPHABET_Y = 0;
	const static int NUMBER_X = 4;
	const static int NUMBER_Y = 0;

	for (row = 0; row <= 5; row++)
	{
		memset(DotMatrix[row], 0, sizeof(unsigned char) * DOT_MATRIX_COL);
	}

	for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
	{
		for (col = 0; col < DOT_SMALL_FONT_COL; col++)
		{
			DotMatrix[row + ALPHABET_Y][col + ALPHABET_X] = DOT_SMALL_ALPHABET_L_FONT[row][col];
			DotMatrix[row + NUMBER_Y][col + NUMBER_X] = DOT_SMALL_NUMBER_FONT[level][row][col];
		}
	}

	PauseTetris(&Tetris);
	PauseTimer(&GameUpdateTimer);
	PauseTimer(&InputUpdateTimer);

	DrawMap(DotMatrix);
	Sleep(1500);

	ResumeTimer(&GameUpdateTimer);
	ResumeTimer(&InputUpdateTimer);
	ResumeTetris(&Tetris);
}

void DrawPause()
{
	int row, col;
	const static int ALPHABET_X = 0;
	const static int ALPHABET_Y = 0;
	static bool blinker = false;

	memset(DotMatrix, 0, DOT_MATRIX_SIZE);

	if (IsTimerReady(&BlinkTimer) == true)
	{
		blinker = !blinker;
	}

	if (blinker == false)
	{
		memcpy(DotMatrix, DOT_PAUSE_FONT, DOT_MATRIX_SIZE);
	}

	DrawMap(DotMatrix);
}

void DrawGameOver()
{
	int row, col;
	unsigned char matrix[DOT_MATRIX_ROW][DOT_MATRIX_COL];
	unsigned char fullCol[DOT_MATRIX_COL] = { 1, 1, 1 ,1 ,1, 1 ,1 };

	Sleep(1000);
}

int MakeNumberMatrix(bool* matrix[DOT_SMALL_FONT_ROW], int number)
{
	int digitCount;
	int colLength;
	int row, col;
	int index, pivot;

	digitCount = (number == 0) ? 1 : (int)log10(number) + 1;
	colLength = digitCount * (DOT_SMALL_FONT_COL + 1);

	for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
	{
		matrix[row] = (bool*)malloc(sizeof(bool) * colLength);
		memset(matrix[row], 0, sizeof(bool) * colLength);
	}

	for (index = digitCount - 1, pivot = 0; index >= 0; index--, pivot += DOT_SMALL_FONT_COL + 1)
	{
		for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
		{
			for (col = 0; col < DOT_SMALL_FONT_COL; col++)
			{
				matrix[row][pivot + col] = DOT_SMALL_NUMBER_FONT[number / (int)pow(10, index)][row][col];
			}
		}
		number -= number / (int)pow(10, index) * (int)pow(10, index);
	}

	return colLength;
}

void DrawFinalScore(unsigned int finalScore)
{
	const static int ITERATION = 3;
	int iterator;
	int pivot;
	int row, col;
	int marginRow = 2;
	bool *scoreNumberMatrix[DOT_SMALL_FONT_ROW];
	int scoreNumberColLength;

	memset(DotMatrix, 0, DOT_MATRIX_SIZE);
	scoreNumberColLength = MakeNumberMatrix(scoreNumberMatrix, finalScore);
	
	for (iterator = 0; iterator < ITERATION; iterator++)
	{
		for (pivot = -DOT_MATRIX_COL; pivot <= DOT_SCORE_FONT_COL; pivot++)
		{
			for (row = 0; row < DOT_SCORE_FONT_ROW; row++)
			{
				for (col = 0; col < DOT_MATRIX_COL; col++)
				{
					if (pivot + col < 0 || pivot + col >= DOT_SCORE_FONT_COL)
					{
						DotMatrix[row + marginRow][col] = 0;
					}
					else
					{
						DotMatrix[row + marginRow][col] = DOT_SCORE_FONT[row][col + pivot];
					}
				}
			}
			DrawMap(DotMatrix);
			Sleep(100);
		}

		for (pivot = -DOT_MATRIX_COL; pivot <= scoreNumberColLength; pivot++)
		{
			for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
			{
				for (col = 0; col < DOT_MATRIX_COL; col++)
				{
					if (pivot + col < 0 || pivot + col >= scoreNumberColLength)
					{
						DotMatrix[row + marginRow][col] = 0;
					}
					else
					{
						DotMatrix[row + marginRow][col] = scoreNumberMatrix[row][col + pivot];
					}
				}
			}
			DrawMap(DotMatrix);
			Sleep(200);
		}
		Sleep(1000);
	}
}
