#pragma once

#include <stdbool.h>
#include "Block.h"
#include "Control.h"
#include "Utility.h"

#define MAP_ROW 10
#define MAP_COL 7
#define MAP_SIZE (sizeof(BlockTile) * MAP_ROW * MAP_COL)
#define BLOCK_BAG_NUMBER 4
#define BLOCK_BAG_SIZE (sizeof(BlockTile) * BLOCK_BAG_NUMBER)
#define LOCK_DELAY 1000

typedef struct GameTimer_t
{
	TickTimer Gravity;
	TickTimer Lock;

} GameTimer;

void InitializeTetris();
void UpdateTetris();

bool IsOutOfMap(Block* block, Point targetPosition);
bool IsCollide(Block* block, Point targetPosition);

void ClearMap();
void RenderToBoolMap(bool renderedMap[][MAP_COL]);
void SetGhostBlockVisibility(bool showGhostBlock);

void GenerateBlockBag();
void ClearBlockBag();
bool IsBlockBagEmpty();
Block PrepareNextBlock();

void SpawnNextBlock();
void SpawnGarbage();

void MoveBlock(MoveDirection direction);
void RotateBlock(RotateDirection direction);
void DropDownBlock();
Point GetDropDownPosition(Block* block);

void HandleUserInput(UserInput input);