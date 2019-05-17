#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <stdbool.h>
#include <stdio.h>

#define BLOCK_TILE_NUMBER 4
#define BLOCK_SHAPE_ROW 3
#define BLOCK_SHAPE_COL 3
#define MAP_ROW 10
#define MAP_COL 7

typedef enum BlockDirection_e
{
	Up, Right, Down, Left
} BlockDirection;

typedef enum BlockTile_e
{
	Empty = 0, I, J, S, T
} BlockTile;

//typedef enum Key_t
//{
//	None, Up, Down, Left, Right, Drop, RotateLeft, RotateRight
//};

typedef struct Point_t
{
	int x;
	int y;
} Point;

typedef struct Block_t
{
	BlockDirection direction;
	BlockTile tile;
	Point point;
} Block;

void InitializeTetris();

void SpawnNextBlock();
void GenerateBlockBag();
BlockTile GetNextBlockTile();
void GetBlockShape(BlockDirection blockDirection, BlockTile blockTile, unsigned char shape[][BLOCK_SHAPE_COL]);

void MoveBlock(BlockDirection direction);
void RotateBlock(BlockDirection direction);
bool CheckCollision(Block* block, Point point);
bool CheckOutOfMap(Block* block, Point point);

void DrawMap();
void GetMap(BlockTile map[][MAP_COL]);

#endif
