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
#define WALLKICK_ENABLE true

void Initialize();
void SetOptions(int argc, char* argv[]);

void UpdateGame();

void UpdateInputAsync();
//void* UpdateSwitchInput(void* inputManager);
unsigned _stdcall UpdateSwitchInput(void* arg);

void DrawMap(unsigned char map[][MAP_COL]);
void DrawNextBlock();
void DrawGameOver();
void DrawFinalScore();
void UpdateDevice();

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

// Animator
DotAnimator Animator;
bool AnimateLevel;
bool AnimatePause;

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

	DrawGameOver();
	DrawFinalScore();

	CloseDevice();

	return 0;
}

void Initialize()
{
	InitializeTetris(&Tetris);
	InitializeTickTimer(&GameUpdateTimer, GAME_UPDATE_SPEED);
	CurrentLevel = 0;
	FinalScore = 0;

	InitializeInputManager(&Input);
	InitializeTickTimer(&InputUpdateTimer, INPUT_UPDATE_SPEED);
	//InputUpdateThread = pthread_create(&InputUpdateThread, NULL, UpdateSwitchInput, NULL);
	_beginthreadex(NULL, 0, UpdateSwitchInput, 0, 0, NULL);

	AnimateLevel = false;
	AnimatePause = false;
	InitializeDotAnimator(&Animator);
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

		if (CurrentLevel < Tetris.GameInfo.Level)
		{
			CurrentLevel = Tetris.GameInfo.Level;
			AnimateLevel = true;
		}
		AnimatePause = !Tetris.GameInfo.IsRunning;

		RenderTetrisMap(&Tetris, DotMatrix);

		if (AnimateLevel == true)
		{
			PauseTetris(&Tetris);
			PauseTimer(&GameUpdateTimer);
			PauseTimer(&InputUpdateTimer);

			DrawLevel(&Animator, DotMatrix, CurrentLevel);
			DrawMap(DotMatrix);
			Sleep(1500);

			ResumeTimer(&GameUpdateTimer);
			ResumeTimer(&InputUpdateTimer);
			ResumeTetris(&Tetris);

			AnimateLevel = false;
			continue;
		}

		if (AnimatePause == true)
		{
			DrawPause(&Animator, DotMatrix);
			DrawMap(DotMatrix);

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

void DrawFinalScore()
{
	const static int ITERATION = 3;
	int iterator;
	int pivot;
	int row, col;
	int marginRow = 2;
	bool *scoreNumberMatrix[DOT_SMALL_FONT_ROW];
	int scoreNumberColLength;

	memset(DotMatrix, 0, DOT_MATRIX_SIZE);
	scoreNumberColLength = MakeNumberMatrix(scoreNumberMatrix, FinalScore);
	
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