#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Block.h"

bool Block_Shape_I[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 0, 0, 0 },
	{ 1, 1, 1 },
	{ 0, 0, 0 },
};
bool Block_Shape_J[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 1, 0, 0 },
	{ 1, 1, 1 },
	{ 0, 0, 0 },
};
bool Block_Shape_L[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 0, 0, 1 },
	{ 1, 1, 1 },
	{ 0, 0, 0 },
};
bool Block_Shape_S[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 1, 1, 0 },
	{ 0, 1, 1 },
	{ 0, 0, 0 },
};
bool Block_Shape_T[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 0, 1, 0 },
	{ 1, 1, 1 },
	{ 0, 0, 0 },
};

void InitializeBlock(Block* block)
{
	block->Tile = BlockTile_I;
	block->Direction = BlockDirection_Up;
	block->Position.x = 0;
	block->Position.y = 0;
	GetBlockShape(block->Tile, block->Shape);

	block->IsValid = false;
}

void BuildBlock(Block* block, BlockTile tile, BlockDirection direction, Point position)
{
	block->Tile = tile;
	block->Direction = direction;
	block->Position = position;
	GetBlockShape(block->Tile, block->Shape);

	block->IsValid = false;
}

void ChangeBlockTile(Block* block, BlockTile newTile)
{
	block->Tile = newTile;
	block->Direction = BlockDirection_Up;
	GetBlockShape(block->Tile, block->Shape);
}

void RotateBlockShape(Block* block, RotateDirection direction)
{
	int row, col;
	int originRow, originCol;
	bool rotatedShape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL];
	memset(rotatedShape, 0, BLOCK_SHAPE_SIZE);

	switch (direction)
	{
	case Rotate_Right:
		for (row = 0, originCol = 0; row < BLOCK_SHAPE_ROW && originCol < BLOCK_SHAPE_COL; row++, originCol++)
		{
			for (col = 0, originRow = BLOCK_SHAPE_ROW - 1; col < BLOCK_SHAPE_COL && originRow >= 0; col++, originRow--)
			{
				rotatedShape[row][col] = block->Shape[originRow][originCol];
			}
		}
		block->Direction += 1;
		block->Direction %= BLOCK_DIRECTION_NUMBER;
		break;

	case Rotate_Left:
		for (row = 0, originCol = BLOCK_SHAPE_COL - 1; row < BLOCK_SHAPE_ROW && originCol >= 0; row++, originCol--)
		{
			for (col = 0, originRow = 0; col < BLOCK_SHAPE_COL && originRow < BLOCK_SHAPE_ROW; col++, originRow++)
			{
				rotatedShape[row][col] = block->Shape[originRow][originCol];
			}
		}
		block->Direction += -1 + BLOCK_DIRECTION_NUMBER;
		block->Direction %= BLOCK_DIRECTION_NUMBER;
		break;

	default:
		printf("RotateBlock: Unknown RotateDirection, direction: '%d'\n", direction);
		break;
	}

	memcpy(block->Shape, rotatedShape, BLOCK_SHAPE_SIZE);
}

void GetBlockShape(BlockTile tile, bool shape[][BLOCK_SHAPE_COL])
{
	bool(*matchShape)[BLOCK_SHAPE_COL];

	switch (tile)
	{
	case BlockTile_I:
		matchShape = Block_Shape_I;
		break;
	case BlockTile_J:
		matchShape = Block_Shape_J;
		break;
	case BlockTile_L:
		matchShape = Block_Shape_L;
		break;
	case BlockTile_S:
		matchShape = Block_Shape_S;
		break;
	case BlockTile_T:
		matchShape = Block_Shape_T;
		break;
	default:
		printf("GetBlockShape: Unknown BlockTile, tile: '%d'\n", tile);
		matchShape = Block_Shape_I;
		break;
	}

	memcpy(shape, matchShape, BLOCK_SHAPE_SIZE);
}
