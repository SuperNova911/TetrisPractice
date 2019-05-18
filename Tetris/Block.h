#pragma once

#include <stdbool.h>
#include "Utility.h"

#define BLOCK_TILE_NUMBER 4
#define BLOCK_DIRECTION_NUMBER 4
#define MOVE_DIRECTION_NUMBER 4
#define ROTATE_DIRECTION_NUMBER 2
#define BLOCK_SHAPE_ROW 3
#define BLOCK_SHAPE_COL 3
#define BLOCK_SHAPE_SIZE (sizeof(bool) * BLOCK_SHAPE_ROW * BLOCK_SHAPE_COL)

typedef enum BlockTile_e
{
	BlockTile_Empty, BlockTile_I, BlockTile_J, BlockTile_S, BlockTile_T
} BlockTile;

typedef enum BlockDirection_e
{
	BlockDirection_Up, BlockDirection_Right, BlockDirection_Down, BlockDirection_Left
} BlockDirection;

typedef enum MoveDirection_e
{
	Move_Up, Move_Right, Move_Down, Move_Left
} MoveDirection;

typedef enum RotateDirection_e
{
	Rotate_Right, Rotate_Left
} RotateDirection;

typedef struct Block_t
{
	BlockTile Tile;
	BlockDirection Direction;
	Point Position;
	bool Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL];
	bool IsValid;
} Block;

void BuildBlock(Block* block, BlockTile tile, BlockDirection direction, Point position);
void ResetBlock(Block* block);
void RotateBlockShape(Block* block, RotateDirection direction);
void GetBlockShape(BlockTile tile, bool shape[][BLOCK_SHAPE_COL]);