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
	InitializeTickTimer(&tetris->GameCore.GravityTimer, 1500);
	InitializeTickTimer(&tetris->GameCore.LockTimer, 1000);
	InitializeBlockBag(&tetris->BlockBag);
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

	//Gravity(tetris);

	ClearFullLine(&tetris->GameMap);
}

void Gravity(TetrisGame* tetris)
{
	if (tetris->GameMap.CurrentBlock.IsValid == false)
	{
		return;
	}

	if (IsReady(&tetris->GameCore.GravityTimer) == true)
	{
		MoveBlock(&tetris->GameMap, Move_Down);
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
	bool actionResult;

	if (tetris->UserInput.Type == InputType_None)
	{
		return;
	}

	switch (tetris->UserInput.Command)
	{
	case Input_MoveUp:
		MoveBlock(&tetris->GameMap, Move_Up);
		break;
	case Input_MoveRight:
		MoveBlock(&tetris->GameMap, Move_Right);
		break;
	case Input_MoveDown:
		actionResult = MoveBlock(&tetris->GameMap, Move_Down);
		if (actionResult == true)
		{
			RestartTimer(&tetris->GameCore.GravityTimer);
		}
		break;
	case Input_MoveLeft:
		MoveBlock(&tetris->GameMap, Move_Left);
		break;
	case Input_RotateRight:
		RotateBlock(&tetris->GameMap, Rotate_Right);
		break;
	case Input_RotateLeft:
		RotateBlock(&tetris->GameMap, Rotate_Left);
		break;
	case Input_DropDown:
		DropDownBlock(&tetris->GameMap);
		break;
	default:
		break;
	}

	tetris->UserInput.Type = InputType_None;
}