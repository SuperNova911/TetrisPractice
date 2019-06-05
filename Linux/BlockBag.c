#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BlockBag.h"

void InitializeBlockBag(TetrisBlockBag* bag)
{
	ClearBlockBag(bag);
}

void GenerateBlockBag(TetrisBlockBag* bag)
{
	BlockTile randomTile;
	bool existChecker[BLOCK_TILE_NUMBER];

    srand((unsigned)time(NULL));
	memset(existChecker, 0, sizeof(bool) * BLOCK_TILE_NUMBER);
	ClearBlockBag(bag);

	while (true)
	{
		randomTile = rand() % BLOCK_TILE_NUMBER + 1;

		if (existChecker[randomTile - 1] == true)
		{
			continue;
		}

		existChecker[randomTile - 1] = true;
		AddBlockTileToBag(bag, randomTile);

		if (IsBlockBagFull(bag) == true)
		{
			break;
		}
	}
}

void AddBlockTileToBag(TetrisBlockBag* bag, BlockTile tile)
{
	if (IsBlockBagFull(bag) == true)
	{
		printf("AddBlockTileToBag: BlockBag is already full\n");
		return;
	}

	if (tile < BlockTile_I || tile > BlockTile_T)
	{
		printf("AddBlockTileToBag: Invalid BlockTile input, tile: '%d'\n", tile);
		return;
	}

	bag->Bag[bag->Count] = tile;
	bag->Count++;
}

void ClearBlockBag(TetrisBlockBag* bag)
{
	memset(bag->Bag, 0, BLOCK_BAG_SIZE);
	bag->Count = 0;
}

bool IsBlockBagEmpty(TetrisBlockBag* bag)
{
	return bag->Count == 0 ? true : false;
}

bool IsBlockBagFull(TetrisBlockBag* bag)
{
	return bag->Count == BLOCK_BAG_NUMBER ? true : false;
}

Block GetNextBlock(TetrisBlockBag* bag)
{
	Block nextBlock;
	BlockTile nextTile;

	InitializeBlock(&nextBlock);

	if (IsBlockBagEmpty(bag) == true)
	{
		GenerateBlockBag(bag);
	}

	bag->Count--;
	nextTile = bag->Bag[bag->Count];
	bag->Bag[bag->Count] = BlockTile_Empty;

	ChangeBlockTile(&nextBlock, nextTile);

	return nextBlock;
}
