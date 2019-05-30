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

void InitializeTetris(TetrisGame* tetris)
{
	InitializeMap(&tetris->GameMap);

	tetris->GameInfo.Stage = 1;
	tetris->GameInfo.Score = 0;
	tetris->GameInfo.Gravity = 1000;
	tetris->GameInfo.LockDelay = 1500;
	tetris->GameInfo.IsRunning = false;
	tetris->GameInfo.IsGameOver = false;

	InitializeTickTimer(&tetris->GameCore.GravityTimer, tetris->GameInfo.Gravity);
	InitializeTickTimer(&tetris->GameCore.LockTimer, tetris->GameInfo.LockDelay);
	tetris->GameCore.WaitForLock = false;

	InitializeBlockBag(&tetris->BlockBag);

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

	tetris->GameInfo.IsRunning = true;
}

bool IsTetrisGameOver(TetrisGame* tetris)
{
	return tetris->GameInfo.IsGameOver;
}

void UpdateTetris(TetrisGame* tetris)
{
	Block newBlock;

	if (tetris->GameMap.CurrentBlock.IsValid == true)
	{
		HandleUserInput(tetris);

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
		tetris->GameInfo.IsGameOver = !SpawnBlock(&tetris->GameMap, tetris->GameMap.NextBlock);
		newBlock = GetNextBlock(&tetris->BlockBag);
		PrepareNextBlock(&tetris->GameMap, newBlock);
		RestartTimer(&tetris->GameCore.GravityTimer);
	}

	if (tetris->GameInfo.IsGameOver == true)
	{
		printf("Stage: '%d', Score: '%d'\n", tetris->GameInfo.Stage, tetris->GameInfo.Score);
	}
}

void UpdateGravity(TetrisGame* tetris)
{
	if (IsReady(&tetris->GameCore.GravityTimer) == true)
	{
		ControlBlockMovement(tetris, Move_Down);
	}
}

void UpdateLock(TetrisGame* tetris)
{
	if (IsLockAhead(&tetris->GameMap, &tetris->GameMap.CurrentBlock, &tetris->GameMap.CurrentBlock.Position) == true &&
		IsReady(&tetris->GameCore.LockTimer) == true)
	{
		Lock(tetris);
	}
}

void Lock(TetrisGame* tetris)
{
	int clearedLine;

	AddBlock(&tetris->GameMap, &tetris->GameMap.CurrentBlock);
	tetris->GameMap.CurrentBlock.IsValid = false;
	tetris->GameCore.WaitForLock = false;

	clearedLine = ClearFullLine(&tetris->GameMap);
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
			tetris->GameCore.WaitForLock = true;
		}
	}
	else
	{
		tetris->GameCore.WaitForLock = false;
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
			tetris->GameCore.WaitForLock = true;
		}
	}
	else
	{
		tetris->GameCore.WaitForLock = false;
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

void AddScore(TetrisGame* tetris, unsigned int clearedLine)
{
	const unsigned int SCORE_PRESET[5] = { 0, 40, 100, 300, 1200 };
	const unsigned int COMBO_SCORE = 50;

	if (clearedLine < 0 || clearedLine > 4)
	{
		printf("AddScore: Invalid clearedLine, line: '%d'\n", clearedLine);
		return;
	}

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

	tetris->GameInfo.Score += SCORE_PRESET[clearedLine] * tetris->GameInfo.Stage;
}
