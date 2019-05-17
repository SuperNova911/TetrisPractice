#include "tetris.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned char I_Block_Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 0, 0, 0 },
	{ 1, 1, 1 },
	{ 0, 0, 0 },
};
unsigned char J_Block_Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 1, 0, 0 },
	{ 1, 1, 1 },
	{ 0, 0, 0 },
};
unsigned char S_Block_Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 1, 1, 0 },
	{ 0, 1, 1 },
	{ 0, 0, 0 },
};
unsigned char T_Block_Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
	{ 0, 1, 0 },
	{ 1, 1, 1 },
	{ 0, 0, 0 },
};

unsigned int Score;
unsigned int Stage;
float Speed;

BlockTile BlockBag[BLOCK_TILE_NUMBER];
int BlockBagCount;

BlockTile Map[MAP_ROW][MAP_COL];
BlockTile RenderedMap[MAP_ROW][MAP_COL];
Point SpawnPoint = { 3, 0 };
Block CurrentBlock;

void InitializeTetris()
{
	GenerateBlockBag();
}

void SpawnNextBlock()
{
	Block nextBlock;
	BlockTile nextTile;

	nextTile = GetNextBlockTile();
	if (nextTile < I || nextTile > T)
	{
		printf("Unknown tile, nextTile: '%d'\n", nextTile);
		return;
	}

	nextBlock.direction = Up;
	nextBlock.tile = nextTile;

	if (CheckCollision(&nextBlock, SpawnPoint) == true)
	{
		// TODO: GameOver
		printf("GameOver\n");
		return;
	}

	nextBlock.point = SpawnPoint;

	CurrentBlock = nextBlock;
}

void GenerateBlockBag()
{
	BlockTile randomTile;
	bool existCheck[BLOCK_TILE_NUMBER];

	memset(existCheck, 0, sizeof(existCheck));
	memset(BlockBag, 0, sizeof(BlockBag));
	BlockBagCount = 0;

	srand((unsigned)time(NULL));
	while (true)
	{
		randomTile = rand() % BLOCK_TILE_NUMBER + 1;

		if (existCheck[randomTile - 1] == true)
		{
			continue;
		}

		existCheck[randomTile - 1] = true;
		BlockBag[BlockBagCount] = randomTile;
		BlockBagCount++;

		if (BlockBagCount == BLOCK_TILE_NUMBER)
		{
			break;
		}
	}

	printf("BlockBag [");
	for (int index = 0; index < BlockBagCount; index++)
	{
		switch (BlockBag[index])
		{
		case I:
			printf("I ");
			break;
		case J:
			printf("J ");
			break;
		case S:
			printf("S ");
			break;
		case T:
			printf("T ");
			break;
		}
	}
	printf("]\n");
}

BlockTile GetNextBlockTile()
{
	BlockTile nextBlockTile;

	if (BlockBagCount <= 0)
	{
		GenerateBlockBag();
	}

	BlockBagCount--;
	nextBlockTile = BlockBag[BlockBagCount];
	BlockBag[BlockBagCount] = Empty;

	return nextBlockTile;
}

void GetBlockShape(BlockDirection blockDirection, BlockTile blockTile, unsigned char shape[][BLOCK_SHAPE_COL])
{
	int row, col;
	int targetRow, targetCol;
	unsigned char(*targetShape)[BLOCK_SHAPE_COL];

	switch (blockTile)
	{
	case I:
		targetShape = I_Block_Shape;
		break;

	case J:
		targetShape = J_Block_Shape;
		break;

	case S:
		targetShape = S_Block_Shape;
		break;

	case T:
		targetShape = T_Block_Shape;
		break;

	default:
		printf("Unknown BlockTile, blockTile: '%d'\n", blockTile);
		return;
	}

	switch (blockDirection)
	{
	case Up:
		for (row = 0, targetRow = 0; row < BLOCK_SHAPE_ROW && targetRow < BLOCK_SHAPE_ROW; row++, targetRow++)
		{
			for (col = 0, targetCol = 0; col < BLOCK_SHAPE_COL && targetCol < BLOCK_SHAPE_COL; col++, targetCol++)
			{
				shape[row][col] = targetShape[targetRow][targetCol];
			}
		}
		break;

	case Right:
		for (col = BLOCK_SHAPE_COL - 1, targetRow = 0; col >= 0 && targetRow < BLOCK_SHAPE_ROW; col--, targetRow++)
		{
			for (row = 0, targetCol = 0; row < BLOCK_SHAPE_ROW && targetCol < BLOCK_SHAPE_COL; row++, targetCol++)
			{
				shape[row][col] = targetShape[targetRow][targetCol];
			}
		}
		break;

	case Down:
		for (row = BLOCK_SHAPE_ROW - 1, targetRow = 0; row >= 0 && targetRow < BLOCK_SHAPE_ROW; row--, targetRow++)
		{
			for (col = BLOCK_SHAPE_COL - 1, targetCol = 0; col >= 0 && targetCol < BLOCK_SHAPE_COL; col--, targetCol++)
			{
				shape[row][col] = targetShape[targetRow][targetCol];
			}
		}
		break;

	case Left:
		for (col = 0, targetRow = 0; col < BLOCK_SHAPE_COL && targetRow < BLOCK_SHAPE_ROW; col++, targetRow++)
		{
			for (row = BLOCK_SHAPE_ROW - 1, targetCol = 0; row >= 0 && targetCol < BLOCK_SHAPE_COL; row--, targetCol++)
			{
				shape[row][col] = targetShape[targetRow][targetCol];
			}
		}
		break;
	}
}

void MoveBlock(BlockDirection direction)
{
	Point newPoint = CurrentBlock.point;

	switch (direction)
	{
	case Up:
		newPoint.y--;
		break;
	case Down:
		newPoint.y++;
		break;
	case Left:
		newPoint.x--;
		break;
	case Right:
		newPoint.x++;
		break;
	default:
		break;
	}

	if (CheckCollision(&CurrentBlock, newPoint) == true)
	{
		return;
	}

	CurrentBlock.point = newPoint;
}

void RotateBlock(BlockDirection direction)
{
	Block rotatedBlock = CurrentBlock;

	switch (direction)
	{
	case Right:
		rotatedBlock.direction++;
		rotatedBlock.direction %= 4;
		break;

	case Left:
		rotatedBlock.direction--;
		rotatedBlock.direction += 4;
		rotatedBlock.direction %= 4;
		break;

	default:
		return;
	}

	if (CheckCollision(&rotatedBlock, rotatedBlock.point) == true)
	{
		return;
	}

	CurrentBlock.direction = rotatedBlock.direction;
}

bool CheckCollision(Block* block, Point point)
{
	if (CheckOutOfMap(block, point) == true)
	{
		return true;
	}

	return false;
}

bool CheckOutOfMap(Block* block, Point point)
{
	int row, col;
	unsigned char blockShape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL];

	GetBlockShape(block->direction, block->tile, blockShape);

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			if (blockShape[row][col] == 0)
			{
				continue;
			}

			if (row + point.y < 0 || row + point.y >= MAP_ROW ||
				col + point.x < 0 || col + point.x >= MAP_COL)
			{
				return true;
			}
		}
	}

	return false;
}

void DrawMap()
{
	int row, col;

	for (row = 0; row < MAP_ROW; row++)
	{
		for (col = 0; col < MAP_COL; col++)
		{
			RenderedMap[row][col] = Map[row][col];
		}
	}

	unsigned char blockShape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL];
	GetBlockShape(CurrentBlock.direction, CurrentBlock.tile, blockShape);

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			RenderedMap[row + CurrentBlock.point.y][col + CurrentBlock.point.x] = blockShape[row][col];
		}
	}
}

void GetMap(BlockTile map[][MAP_COL])
{
	memcpy(map, RenderedMap, sizeof(RenderedMap));
}
