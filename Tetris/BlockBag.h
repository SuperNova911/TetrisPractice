#pragma once
#include <stdbool.h>
#include "Block.h"

#define BLOCK_BAG_NUMBER 4
#define BLOCK_BAG_SIZE (sizeof(BlockTile) * BLOCK_BAG_NUMBER)

typedef struct TetrisBlockBag_t
{
	BlockTile Bag[BLOCK_BAG_NUMBER];
	int Count;
} TetrisBlockBag;

void InitializeBlockBag(TetrisBlockBag* bag);

void GenerateBlockBag(TetrisBlockBag* bag);
void AddBlockTileToBag(TetrisBlockBag* bag, BlockTile tile);
void ClearBlockBag(TetrisBlockBag* bag);
bool IsBlockBagEmpty(TetrisBlockBag* bag);
bool IsBlockBagFull(TetrisBlockBag* bag);

Block GetNextBlock(TetrisBlockBag* bag);