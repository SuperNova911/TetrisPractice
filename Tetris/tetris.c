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
	InitializeBlockBag(&tetris->BlockBag);
}

void RunTetris(TetrisGame* tetris)
{
}

void UpdateTetris(TetrisGame* tetris)
{
	// Move, Rotate

	Gravity();

	ClearLine();
}

void Gravity()
{
	if (CurrentBlock.IsValid == false)
	{
		return;
	}

	if (IsReady(&Core.GravityTimer) == true)
	{
		MoveBlock(Move_Down);
	}


}

void HandleUserInput(TetrisGame* tetris, UserInput input)
{
	switch (input)
	{
	case Input_MoveUp:
		MoveBlock(Move_Up);
		break;
	case Input_MoveRight:
		MoveBlock(Move_Right);
		break;
	case Input_MoveDown:
		MoveBlock(Move_Down);
		break;
	case Input_MoveLeft:
		MoveBlock(Move_Left);
		break;
	case Input_RotateRight:
		RotateBlock(Rotate_Right);
		break;
	case Input_RotateLeft:
		RotateBlock(Rotate_Left);
		break;
	case Input_DropDown:
		DropDownBlock();
		break;
	default:
		break;
	}
}