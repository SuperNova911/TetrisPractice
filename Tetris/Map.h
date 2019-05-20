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
	bool RenderGhostBlock;
} TetrisMap;

void InitializeMap(TetrisMap* map);

void ClearMap(TetrisMap* map);
void RenderToBoolMap(TetrisMap* map, bool renderedMap[][MAP_COL]);
void RenderGhostBlock(TetrisMap* map, bool renderGhostBlock);

bool IsOutOfMap(TetrisMap* map, Block* block, Point targetPosition);
bool IsCollide(TetrisMap* map, Block* block, Point targetPosition);

bool IsLineEmpty(TetrisMap* map, int row);
bool IsLineFull(TetrisMap* map, int row);
bool ShiftLine(TetrisMap* map, int originRow, int targetRow);
int ClearFullLine(TetrisMap* map);

bool SpawnBlock(TetrisMap* map, Block block);
void SpawnGarbage(TetrisMap* map);

bool MoveBlock(TetrisMap* map, MoveDirection direction);
bool RotateBlock(TetrisMap* map, RotateDirection direction);
bool DropDownBlock(TetrisMap* map);
Point GetDropDownPosition(TetrisMap* map, Block* block);