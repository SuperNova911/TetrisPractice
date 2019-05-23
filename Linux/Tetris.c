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

	InitializeTickTimer(&tetris->GameCore.GravityTimer, 1000);
	InitializeTickTimer(&tetris->GameCore.LockTimer, 1500);
	tetris->GameCore.WaitForLock = false;

	tetris->GameInfo.IsRunning = false;

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

void UpdateTetris(TetrisGame* tetris)
{
	Block newBlock;

	if (tetris->GameMap.CurrentBlock.IsValid == false)
	{
		newBlock = GetNextBlock(&tetris->BlockBag);
		SpawnBlock(&tetris->GameMap, newBlock);
	}

	HandleUserInput(tetris);

	if (tetris->GameCore.WaitForLock == true)
	{
		Lock(tetris);
	}
	else
	{
		Gravity(tetris);
	}
}

void Gravity(TetrisGame* tetris)
{
	if (tetris->GameMap.CurrentBlock.IsValid == false)
	{
		return;
	}

	if (IsReady(&tetris->GameCore.GravityTimer) == true)
	{
		ControlBlockMovement(tetris, Move_Down);
	}
}

void Lock(TetrisGame* tetris)
{
	if (tetris->GameMap.CurrentBlock.IsValid == false)
	{
		return;
	}

	if (IsLockAhead(&tetris->GameMap, &tetris->GameMap.CurrentBlock, &tetris->GameMap.CurrentBlock.Position) == true &&
		IsReady(&tetris->GameCore.LockTimer) == true)
	{
		AddBlock(&tetris->GameMap, &tetris->GameMap.CurrentBlock);
		tetris->GameMap.CurrentBlock.IsValid = false;
        tetris->GameCore.WaitForLock = false;
		ClearFullLine(&tetris->GameMap);
	}
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
		DropDownBlock(&tetris->GameMap);
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
		break;
	default:
		break;
	}

	tetris->UserInput.Type = InputType_None;
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
