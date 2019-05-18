#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Block.h"
#include "Control.h"
#include "Tetris.h"
#include "Utility.h"

const Point SPAWN_POSITION = { MAP_COL / 2 - BLOCK_SHAPE_COL / 2, 0 };

BlockTile Map[MAP_ROW][MAP_COL];
bool ShowGhostBlock;

BlockTile BlockBag[BLOCK_BAG_NUMBER];
int BlockBagCount;

Block CurrentBlock;
Block NextBlock;

void InitializeTetris()
{
	srand((unsigned)time(NULL));
	
	ShowGhostBlock = false;

	ClearMap();
	ClearBlockBag();
}

void UpdateTetris()
{
	// 
}

bool IsOutOfMap(Block* block, Point targetPosition)
{
	int row, col;

	// Quick Check
	if (block->Position.y < 0 || block->Position.y + BLOCK_SHAPE_ROW > MAP_ROW ||
		block->Position.x < 0 || block->Position.x + BLOCK_SHAPE_COL > MAP_COL)
	{
		return true;
	}

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			if (block->Shape[row][col] == 0)
			{
				continue;
			}

			if (block->Position.y + row < 0 || block->Position.y + row > MAP_ROW ||
				block->Position.x + col < 0 || block->Position.x + col > MAP_COL)
			{
				return true;
			}
		}
	}

	return false;
}

bool IsCollide(Block* block, Point targetPosition)
{
	int row, col;

	if (IsOutOfMap(block, targetPosition) == true)
	{
		return true;
	}

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			if (block->Shape[row][col] == 0)
			{
				continue;
			}

			// IsOutOfMap() 에서 Map 범위를 벗어나는지 검사하므로 따로 범위검사 안 함
			if (Map[block->Position.y + row][block->Position.x + col] != BlockTile_Empty)
			{
				return true;
			}
		}
	}

	return false;
}

void ClearMap()
{
	memset(Map, 0, MAP_SIZE);
}

void RenderToBoolMap(bool renderedMap[][MAP_COL])
{
	int row, col;

	memcpy(renderedMap, Map, MAP_SIZE);

	if (CurrentBlock.IsValid == false)
	{
		return;
	}

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			renderedMap[row + CurrentBlock.Position.y][col + CurrentBlock.Position.x] = CurrentBlock.Shape[row][col];
		}
	}
}

void SetGhostBlockVisibility(bool showGhostBlock)
{
	ShowGhostBlock = showGhostBlock;
}

void GenerateBlockBag()
{
	BlockTile randomTile;
	bool existChecker[BLOCK_TILE_NUMBER];

	ClearBlockBag();
	memset(existChecker, 0, sizeof(bool) * BLOCK_TILE_NUMBER);

	while (true)
	{
		randomTile = rand() % BLOCK_TILE_NUMBER + 1;

		if (existChecker[randomTile - 1] == true)
		{
			continue;
		}

		existChecker[randomTile - 1] = true;
		BlockBag[BlockBagCount] = randomTile;
		BlockBagCount++;

		if (BlockBagCount == BLOCK_TILE_NUMBER)
		{
			break;
		}
	}
}

void ClearBlockBag()
{
	memset(BlockBag, 0, BLOCK_BAG_SIZE);
	BlockBagCount = 0;
}

bool IsBlockBagEmpty()
{
	return BlockBagCount <= 0 ? true : false;
}

Block PrepareNextBlock()
{
	BlockTile nextBlockTile;
	const Point DEFAULT_POSITION = { 0, 0 };

	if (IsBlockBagEmpty() == true)
	{
		GenerateBlockBag();
	}

	BlockBagCount--;
	nextBlockTile = BlockBag[BlockBagCount];
	BlockBag[BlockBagCount] = BlockTile_Empty;

	BuildBlock(&NextBlock, nextBlockTile, BlockDirection_Up, DEFAULT_POSITION);
}

void SpawnNextBlock()
{
	CurrentBlock = NextBlock;
	CurrentBlock.Position = SPAWN_POSITION;

	PrepareNextBlock();

	if (IsCollide(&CurrentBlock, CurrentBlock.Position) == true)
	{
		// TODO: GameOver
		printf("GameOver\n");
		return;
	}

	CurrentBlock.IsValid = true;
}

void SpawnGarbage()
{
}

void MoveBlock(MoveDirection direction)
{
	const Point PRESET[MOVE_DIRECTION_NUMBER] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
	Point movedPosition;

	if (CurrentBlock.IsValid == false)
	{
		return;
	}

	if (direction < Move_Up || direction > Move_Left)
	{
		return;
	}

	movedPosition = AddPoint(&CurrentBlock.Position, &PRESET[direction]);

	if (IsCollide(&CurrentBlock, movedPosition) == true)
	{
		return;
	}

	CurrentBlock.Position = movedPosition;
}

void RotateBlock(RotateDirection direction)
{
	if (CurrentBlock.IsValid == false)
	{
		return;
	}

	RotateBlockShape(&CurrentBlock, direction);
}

void DropDownBlock()
{
	if (CurrentBlock.IsValid == false)
	{
		return;
	}

	CurrentBlock.Position = GetDropDownPosition(&CurrentBlock);
}

Point GetDropDownPosition(Block* block)
{
	int x, y;
	Point dropDownPosition, testPosition;

	dropDownPosition = block->Position;

	for (x = block->Position.x, y = block->Position.y; y <= MAP_ROW - BLOCK_SHAPE_ROW; y++)
	{
		testPosition.x = x;
		testPosition.y = y;

		if (IsCollide(block, testPosition) == true)
		{
			break;
		}

		dropDownPosition = testPosition;
	}

	return dropDownPosition;
}

void HandleUserInput(UserInput input)
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