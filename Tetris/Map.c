#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Block.h"
#include "Control.h"
#include "Map.h"
#include "Point.h"

const Point SPAWN_POSITION = { MAP_COL / 2 - BLOCK_SHAPE_COL / 2, 0 };

void InitializeMap(TetrisMap* map)
{
	ClearMap(map);
	InitializeBlock(&map->CurrentBlock);
	map->RenderGhostBlock = false;
}

void ClearMap(TetrisMap* map)
{
	memset(map->Map, 0, MAP_SIZE);
}

void RenderToBoolMap(TetrisMap* map, bool renderedMap[][MAP_COL])
{
	int row, col;

	memcpy(renderedMap, map->Map, MAP_SIZE);

	if (map->CurrentBlock.IsValid == false)
	{
		return;
	}

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			renderedMap[row + map->CurrentBlock.Position.y][col + map->CurrentBlock.Position.x] = map->CurrentBlock.Shape[row][col];
		}
	}
}

void RenderGhostBlock(TetrisMap* map, bool renderGhostBlock)
{
	map->RenderGhostBlock = renderGhostBlock;
}

bool IsOutOfMap(TetrisMap* map, Block* block, Point targetPosition)
{
	int row, col;

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			if (block->Shape[row][col] == 0)
			{
				continue;
			}

			if (targetPosition.y + row < 0 || targetPosition.y + row > MAP_ROW - 1 ||
				targetPosition.x + col < 0 || targetPosition.x + col > MAP_COL - 1)
			{
				return true;
			}
		}
	}

	return false;
}

bool IsCollide(TetrisMap* map, Block* block, Point targetPosition)
{
	int row, col;

	if (IsOutOfMap(map, block, targetPosition) == true)
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
			if (map->Map[block->Position.y + row][block->Position.x + col] != BlockTile_Empty)
			{
				return true;
			}
		}
	}

	return false;
}

bool IsLineEmpty(TetrisMap* map, int row)
{
	int col;

	if (row < 0 || row >= MAP_ROW)
	{
		printf("IsLineEmpty: Out of index, row: '%d'\n", row);
		return false;
	}

	for (col = 0; col < MAP_COL; col++)
	{
		if (map->Map[row][col] != BlockTile_Empty)
		{
			return false;
		}
	}

	return true;
}

bool IsLineFull(TetrisMap* map, int row)
{
	int col;

	if (row < 0 || row >= MAP_ROW)
	{
		printf("IsLineFull: Out of index, row: '%d'\n", row);
		return false;
	}

	for (col = 0; col < MAP_COL; col++)
	{
		if (map->Map[row][col] == BlockTile_Empty)
		{
			return false;
		}
	}

	return true;
}

bool ShiftLine(TetrisMap* map, int originRow, int targetRow)
{
	if (originRow < 0 || originRow >= MAP_ROW || 
		targetRow < 0 || targetRow >= MAP_ROW)
	{
		printf("ShiftLine: Out of index, originRow: '%d', targetRow: '%d'\n", originRow, targetRow);
		return false;
	}

	if (originRow == targetRow)
	{
		printf("ShiftLine: Shift to same row, originRow: '%d', targetRow: '%d'\n", originRow, targetRow);
		return false;
	}

	memcpy(map->Map[targetRow], map->Map[originRow], MAP_SIZE / MAP_ROW);
	memset(map->Map[originRow], 0, MAP_SIZE / MAP_ROW);
	
	return true;
}

int ClearFullLine(TetrisMap* map)
{
	int clearedLine = 0;
	int row, emptyRow, targetRow;
	bool emptyLineChecker[MAP_ROW];

	memset(emptyLineChecker, false, sizeof(bool) * MAP_ROW);

	for (row = 0; row < MAP_ROW; row++)
	{
		if (IsLineFull(map, row) == true)
		{
			clearedLine++;
			emptyLineChecker[row] = true;
		}
		else
		{
			targetRow = row;
			for (emptyRow = row + 1; emptyRow < MAP_ROW; emptyRow++)
			{
				if (emptyLineChecker[emptyRow] == true)
				{
					targetRow = emptyRow;
				}
			}

			if (row < targetRow)
			{
				ShiftLine(map, row, targetRow);
				emptyLineChecker[row] = true;
				emptyLineChecker[targetRow] = false;
			}
		}
	}

	return clearedLine;
}

bool SpawnBlock(TetrisMap* map, Block block)
{
	if (map->CurrentBlock.IsValid == true)
	{
		printf("SpawnBlock: Map already has valid Block, Block: (Tile: '%d', Position: (x: '%d', y: '%d'))\n", 
			map->CurrentBlock.Tile, map->CurrentBlock.Position.x, map->CurrentBlock.Position.y);
		return false;
	}

	block.Position = SPAWN_POSITION;

	if (IsCollide(map, &block, block.Position) == true)
	{
		// TODO: GameOver
		printf("GameOver\n");
		return false;
	}

	map->CurrentBlock = block;
	map->CurrentBlock.IsValid = true;

	return true;
}

void SpawnGarbage(TetrisMap* map)
{
}

bool MoveBlock(TetrisMap* map, MoveDirection direction)
{
	const Point PRESET[MOVE_DIRECTION_NUMBER] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
	Point movedPosition;

	if (map->CurrentBlock.IsValid == false)
	{
		printf("MoveBlock: No valid block to move\n");
		return false;
	}

	if (direction < Move_Up || direction > Move_Left)
	{
		printf("MoveBlock: Unknown MoveDirection, direction: '%d'\n", direction);
		return false;
	}

	movedPosition = AddPoint(&map->CurrentBlock.Position, &PRESET[direction]);

	if (IsCollide(map, &map->CurrentBlock, movedPosition) == true)
	{
		return false;
	}

	map->CurrentBlock.Position = movedPosition;

	return true;
}

bool RotateBlock(TetrisMap* map, RotateDirection direction)
{
	Block rotatedBlock;

	if (map->CurrentBlock.IsValid == false)
	{
		printf("RotateBlock: No valid block to rotate\n");
		return false;
	}

	if ((direction != Rotate_Right && direction != Rotate_Left) == true)
	{
		printf("MoveBlock: Unknown MoveDirection, direction: '%d'\n", direction);
		return false;
	}

	rotatedBlock = map->CurrentBlock;
	RotateBlockShape(&rotatedBlock, direction);

	if (IsCollide(map, &rotatedBlock, rotatedBlock.Position) == true)
	{
		return false;
	}

	map->CurrentBlock = rotatedBlock;

	return true;
}

bool DropDownBlock(TetrisMap* map)
{
	Point dropDownPosition;

	if (map->CurrentBlock.IsValid == false)
	{
		printf("DropDownBlock: No valid block to drop down\n");
		return false;
	}

	dropDownPosition = GetDropDownPosition(map, &map->CurrentBlock);

	if (IsEqualPoint(&dropDownPosition, &map->CurrentBlock.Position) == true)
	{
		printf("DropDownBlock: Cannot drop down anymore, DropDownPosition: (x: '%d', y: '%d')\n",
			dropDownPosition.x, dropDownPosition.y);
		return false;
	}

	map->CurrentBlock.Position = dropDownPosition;
	return true;
}

Point GetDropDownPosition(TetrisMap* map, Block* block)
{
	int x, y;
	Point dropDownPosition, testPosition;

	dropDownPosition = block->Position;

	for (x = block->Position.x, y = block->Position.y; y <= MAP_ROW - BLOCK_SHAPE_ROW; y++)
	{
		testPosition.x = x;
		testPosition.y = y;

		if (IsCollide(map, block, testPosition) == true)
		{
			break;
		}

		dropDownPosition = testPosition;
	}

	return dropDownPosition;
}
