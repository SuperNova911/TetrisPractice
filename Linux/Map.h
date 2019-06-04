#pragma once

#include <stdbool.h>
#include "Block.h"
#include "Point.h"

#define MAP_ROW 10
#define MAP_COL 7
#define MAP_SIZE (sizeof(BlockTile) * MAP_ROW * MAP_COL)

typedef struct TetrisMap_t
{
	BlockTile Map[MAP_ROW][MAP_COL];
	Block CurrentBlock;
	Block NextBlock;
	Block HoldBlock;
	bool AllowWallKick;
	bool RenderGhostBlock;
} TetrisMap;

void InitializeMap(TetrisMap* map);

void ClearMap(TetrisMap* map);
void RenderMap(TetrisMap* map, unsigned char renderedMap[][MAP_COL]);
void RenderNextBlock(TetrisMap* map, unsigned char* renderedNextBlock);

bool IsOutOfMap(TetrisMap* map, Block* block, Point* targetPosition);
bool IsCollide(TetrisMap* map, Block* block, Point* targetPosition, bool checkOutOfMap);
bool IsLockAhead(TetrisMap* map, Block* block, Point* targetPosition);

bool IsLineEmpty(TetrisMap* map, int row);
bool IsLineFull(TetrisMap* map, int row);
bool ShiftLine(TetrisMap* map, int originRow, int targetRow);
void ClearLine(TetrisMap* map, int row);
int ClearFullLine(TetrisMap* map);

void PrepareNextBlock(TetrisMap* map, Block block);
bool SpawnBlock(TetrisMap* map, Block block);
void SpawnGarbage(TetrisMap* map);

bool AddBlock(TetrisMap* map, Block* block);

bool MoveBlock(TetrisMap* map, MoveDirection direction);
bool RotateBlock(TetrisMap* map, RotateDirection direction);
bool DropDownBlock(TetrisMap* map);
Point GetDropDownPosition(TetrisMap* map, Block* block);