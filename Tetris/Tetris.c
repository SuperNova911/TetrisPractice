#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Block.h"
#include "Control.h"
#include "Map.h"
#include "Point.h"
#include "Tetris.h"

const static unsigned int GRAVITY_DELAY_PRESET[TETRIS_MAX_LEVEL] =
{
	1400, 1200, 1000, 800, 700, 600, 500, 400, 300
};
const static unsigned int LOCK_DELAY_PRESET[TETRIS_MAX_LEVEL] =
{
	1500, 1500, 1500, 1000, 1000, 1000, 500, 500, 500
};

void InitializeTetris(TetrisGame* tetris)
{
	InitializeMap(&tetris->GameMap);

	tetris->GameInfo.Level = 1;
	tetris->GameInfo.Score = 0;
	tetris->GameInfo.TotalClearedLine = 0;
	tetris->GameInfo.Combo = -1;
	tetris->GameInfo.Gravity = GRAVITY_DELAY_PRESET[tetris->GameInfo.Level - 1];
	tetris->GameInfo.LockDelay = LOCK_DELAY_PRESET[tetris->GameInfo.Level - 1];
	tetris->GameInfo.SpawnDelay = 500;
	tetris->GameInfo.BlinkDelay = 100;
	tetris->GameInfo.IsRunning = false;
	tetris->GameInfo.IsGameOver = false;

	InitializeTickTimer(&tetris->GameCore.GravityTimer, tetris->GameInfo.Gravity);
	InitializeTickTimer(&tetris->GameCore.LockTimer, tetris->GameInfo.LockDelay);
	InitializeTickTimer(&tetris->GameCore.SpawnTimer, tetris->GameInfo.SpawnDelay);
	InitializeTickTimer(&tetris->GameCore.BlinkTimer, tetris->GameInfo.BlinkDelay);
	tetris->GameCore.WaitForLock = false;
	tetris->GameCore.RenderCurrentBlock = true;

	InitializeBlockBag(&tetris->BlockBag);
    PrepareNextBlock(&tetris->GameMap, GetNextBlock(&tetris->BlockBag));

	tetris->UserInput.Command = 0;
	tetris->UserInput.Type = InputType_None;
}

void RunTetris(TetrisGame* tetris)
{
	if (tetris->GameInfo.IsRunning == true)
	{
		printf("RunTetris: Game is already running\n");
		return;
	}

	RunTimer(&tetris->GameCore.GravityTimer);
	RunTimer(&tetris->GameCore.LockTimer);
	RunTimer(&tetris->GameCore.SpawnTimer);
	RunTimer(&tetris->GameCore.BlinkTimer);

	tetris->GameInfo.IsRunning = true;
}

void PauseTetris(TetrisGame* tetris)
{
	if (tetris->GameInfo.IsRunning == false)
	{
		printf("PauseTetris: Game is already paused\n");
		return;
	}

	PauseTimer(&tetris->GameCore.GravityTimer);
	PauseTimer(&tetris->GameCore.LockTimer);
	PauseTimer(&tetris->GameCore.SpawnTimer);

	tetris->GameInfo.IsRunning = false;
}

void ResumeTetris(TetrisGame* tetris)
{
	if (tetris->GameInfo.IsRunning == true)
	{
		printf("ResumeTetris: Game is already running\n");
		return;
	}

	ResumeTimer(&tetris->GameCore.GravityTimer);
	ResumeTimer(&tetris->GameCore.LockTimer);
	ResumeTimer(&tetris->GameCore.SpawnTimer);

	tetris->GameInfo.IsRunning = true;
}

void RenderTetrisMap(TetrisGame* tetris, unsigned char renderedMap[][MAP_COL])
{
	RenderMap(&tetris->GameMap, renderedMap, tetris->GameCore.RenderCurrentBlock);
}

bool IsTetrisGameOver(TetrisGame* tetris)
{
	return tetris->GameInfo.IsGameOver;
}

void UpdateTetris(TetrisGame* tetris)
{
	Block newBlock;

	HandleUserInput(tetris);

	if (tetris->GameMap.CurrentBlock.IsValid == true)
	{
		if (tetris->GameCore.WaitForLock == true)
		{
			UpdateLock(tetris);
		}
		else
		{
			UpdateGravity(tetris);
		}
	}
	else
	{
		if (IsTimerReady(&tetris->GameCore.SpawnTimer) == true)
		{
			if (SpawnBlock(&tetris->GameMap, &tetris->GameMap.NextBlock) == false)
			{
				tetris->GameInfo.IsGameOver = true;
			}
			newBlock = GetNextBlock(&tetris->BlockBag);
			PrepareNextBlock(&tetris->GameMap, newBlock);
			RestartTimer(&tetris->GameCore.GravityTimer);
			tetris->GameCore.RenderCurrentBlock = true;
		}
	}

	if (tetris->GameInfo.IsGameOver == true)
	{
		printf("Stage: '%d', Score: '%d'\n", tetris->GameInfo.Level, tetris->GameInfo.Score);
	}
}

void UpdateGravity(TetrisGame* tetris)
{
	if (IsTimerReady(&tetris->GameCore.GravityTimer) == true)
	{
		ControlBlockMovement(tetris, Move_Down);
	}
}

void UpdateLock(TetrisGame* tetris)
{
	if (IsLockAhead(&tetris->GameMap, &tetris->GameMap.CurrentBlock, &tetris->GameMap.CurrentBlock.Position) == true)
	{
		if (IsTimerReady(&tetris->GameCore.BlinkTimer) == true)
		{
			tetris->GameCore.RenderCurrentBlock = !tetris->GameCore.RenderCurrentBlock;
		}

		if (IsTimerReady(&tetris->GameCore.LockTimer) == true)
		{
			Lock(tetris);
		}
	}
	else
	{
		tetris->GameCore.WaitForLock = false;
		tetris->GameCore.RenderCurrentBlock = true;
	}
}

void Lock(TetrisGame* tetris)
{
	int clearedLine;

	AddBlock(&tetris->GameMap, &tetris->GameMap.CurrentBlock);
	tetris->GameMap.CurrentBlock.IsValid = false;
	tetris->GameCore.WaitForLock = false;
	RestartTimer(&tetris->GameCore.SpawnTimer);

	clearedLine = ClearFullLine(&tetris->GameMap);
	if (clearedLine < 0 || clearedLine > 4)
	{
		printf("Lock: Invalid clearedLine, line: '%d'\n", clearedLine);
		return;
	}

	LevelUp(tetris, clearedLine);
	AddScore(tetris, clearedLine);
}

void ReadUserInput(TetrisGame* tetris, InputCollection* InputCollection)
{
	int index;

	for (index = 0; index < INPUT_SOURCE_NUMBER; index++)
	{
		if (InputCollection->Input[index].Type == InputType_Click)
		{
			tetris->UserInput = InputCollection->Input[index];
			break;
		}
	}
}

void HandleUserInput(TetrisGame* tetris)
{
	if (tetris->UserInput.Type == InputType_None)
	{
		return;
	}

	if (tetris->GameMap.CurrentBlock.IsValid == true && tetris->GameInfo.IsRunning == true)
	{
		switch (tetris->UserInput.Command)
		{
		case Input_MoveUp:
			ControlBlockMovement(tetris, Move_Up);
			break;
		case Input_MoveRight:
			ControlBlockMovement(tetris, Move_Right);
			break;
		case Input_MoveDown:
			ControlBlockMovement(tetris, Move_Down);
			break;
		case Input_MoveLeft:
			ControlBlockMovement(tetris, Move_Left);
			break;
		case Input_RotateRight:
			ControlBlockRotation(tetris, Rotate_Right);
			break;
		case Input_RotateLeft:
			ControlBlockRotation(tetris, Rotate_Left);
			break;
		case Input_DropDown:
			ControlBlockDropDown(tetris);
			break;
        default:
            break;
		}
	}

	switch (tetris->UserInput.Command)
	{
	case Input_Pause:
		if (tetris->GameInfo.IsRunning == true)
		{
			PauseTetris(tetris);
		}
		else
		{
			ResumeTetris(tetris);
		}
		break;
	case Input_Special:
		break;
    default:
        break;
	}

	tetris->UserInput.Type = InputType_None;
}

void InputTest(TetrisGame* tetris, InputInfo* inputInfo)
{
	memcpy(&tetris->UserInput, inputInfo, sizeof(InputInfo));
}

void ControlBlockMovement(TetrisGame* tetris, MoveDirection direction)
{
	bool controlResult;
	controlResult = MoveBlock(&tetris->GameMap, direction);

	if (controlResult == true && direction == Move_Down)
	{
		RestartTimer(&tetris->GameCore.GravityTimer);
	}

	if (IsLockAhead(&tetris->GameMap, &tetris->GameMap.CurrentBlock, &tetris->GameMap.CurrentBlock.Position) == true)
	{
		if (tetris->GameCore.WaitForLock == false)
		{
			RestartTimer(&tetris->GameCore.LockTimer);
			RestartTimer(&tetris->GameCore.BlinkTimer);
			tetris->GameCore.WaitForLock = true;
		}
	}
	else
	{
		tetris->GameCore.WaitForLock = false;
		tetris->GameCore.RenderCurrentBlock = true;
	}
}

void ControlBlockRotation(TetrisGame* tetris, RotateDirection direction)
{
	RotateBlock(&tetris->GameMap, direction);

	if (IsLockAhead(&tetris->GameMap, &tetris->GameMap.CurrentBlock, &tetris->GameMap.CurrentBlock.Position) == true)
	{
		if (tetris->GameCore.WaitForLock == false)
		{
			RestartTimer(&tetris->GameCore.LockTimer);
			RestartTimer(&tetris->GameCore.BlinkTimer);
			tetris->GameCore.WaitForLock = true;
		}
	}
	else
	{
		tetris->GameCore.WaitForLock = false;
		tetris->GameCore.RenderCurrentBlock = true;
	}
}

void ControlBlockDropDown(TetrisGame* tetris)
{
	DropDownBlock(&tetris->GameMap);

	if (tetris->GameCore.WaitForLock == true)
	{
		Lock(tetris);
	}
	else
	{
		RestartTimer(&tetris->GameCore.LockTimer);
		tetris->GameCore.WaitForLock = true;
	}
}

void LevelUp(TetrisGame* tetris, unsigned int clearedLine)
{
	unsigned int newLevel;

	if (clearedLine == 0)
	{
		return;
	}

	tetris->GameInfo.TotalClearedLine += clearedLine;

	if (tetris->GameInfo.Level == TETRIS_MAX_LEVEL)
	{
		return;
	}

	newLevel = (tetris->GameInfo.TotalClearedLine / 10) + 1;
	if (newLevel > TETRIS_MAX_LEVEL)
	{
		newLevel = TETRIS_MAX_LEVEL;
	}

	if (newLevel > tetris->GameInfo.Level)
	{
		tetris->GameInfo.Level = newLevel;
		ChangeDelay(&tetris->GameCore.GravityTimer, GRAVITY_DELAY_PRESET[newLevel - 1]);
	    ChangeDelay(&tetris->GameCore.LockTimer, LOCK_DELAY_PRESET[newLevel - 1]);
	}
}

void AddScore(TetrisGame* tetris, unsigned int clearedLine)
{
	const unsigned int SCORE_PRESET[5] = { 0, 4, 10, 30, 120 };
	const unsigned int COMBO_SCORE = 5;

	if (clearedLine == 0)
	{
		tetris->GameInfo.Combo = -1;
	}
	else
	{
		tetris->GameInfo.Combo += (clearedLine - 1);
	}

    if (tetris->GameInfo.Combo > 0)
    {
        tetris->GameInfo.Score += COMBO_SCORE * tetris->GameInfo.Combo;
    }

	tetris->GameInfo.Score += SCORE_PRESET[clearedLine] * tetris->GameInfo.Level;
}
