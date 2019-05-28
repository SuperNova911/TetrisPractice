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
	map->AllowWallKick = true;
}

void ClearMap(TetrisMap* map)
{
	memset(map->Map, 0, MAP_SIZE);
}

void RenderToBoolMap(TetrisMap* map, bool renderedMap[][MAP_COL])
{
	int row, col;

	for (row = 0; row < MAP_ROW; row++)
	{
		for (col = 0; col < MAP_COL; col++)
		{
			renderedMap[row][col] = map->Map[row][col];
		}
	}

	if (map->CurrentBlock.IsValid == false)
	{
		return;
	}

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			if (map->CurrentBlock.Shape[row][col] != BlockTile_Empty)
			{
				renderedMap[row + map->CurrentBlock.Position.y][col + map->CurrentBlock.Position.x] = map->CurrentBlock.Shape[row][col];
			}
		}
	}
}

void RenderGhostBlock(TetrisMap* map, bool renderGhostBlock)
{
	map->RenderGhostBlock = renderGhostBlock;
}

bool IsOutOfMap(TetrisMap* map, Block* block, Point* targetPosition)
{
	int row, col;

	if (targetPosition->y >= 0 && targetPosition->y + BLOCK_SHAPE_ROW <= MAP_ROW &&
		targetPosition->x >= 0 && targetPosition->x + BLOCK_SHAPE_COL <= MAP_COL)
	{
		return false;
	}

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			if (block->Shape[row][col] == 0)
			{
				continue;
			}

			if (targetPosition->y + row < 0 || targetPosition->y + row > MAP_ROW - 1 ||
				targetPosition->x + col < 0 || targetPosition->x + col > MAP_COL - 1)
			{
				return true;
			}
		}
	}

	return false;
}

bool IsCollide(TetrisMap* map, Block* block, Point* targetPosition, bool checkOutOfMap)
{
	int row, col;

	if (checkOutOfMap == true && IsOutOfMap(map, block, targetPosition) == true)
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
			if (map->Map[targetPosition->y + row][targetPosition->x + col] != BlockTile_Empty)
			{
				return true;
			}
		}
	}

	return false;
}

bool IsLockAhead(TetrisMap* map, Block* block, Point* targetPosition)
{
	int row, col;
	int nextRow, nextCol;
	int lowestRow[BLOCK_SHAPE_COL];

	if (IsOutOfMap(map, block, targetPosition) == true)
	{
		printf("IsLockAhead: Block is out of map, targetPosition: (x: '%d', y: '%d')\n",
			targetPosition->x, targetPosition->y);
		return false;
	}

	for (col = 0; col < BLOCK_SHAPE_COL; col++)
	{
		lowestRow[col] = -1;
		for (row = BLOCK_SHAPE_ROW - 1; row >= 0; row--)
		{
			if (block->Shape[row][col] == 1)
			{
				lowestRow[col] = row;
				break;
			}
		}
	}

	for (col = 0; col < BLOCK_SHAPE_COL; col++)
	{
		if (lowestRow[col] < 0)
		{
			continue;
		}
		
		nextRow = targetPosition->y + lowestRow[col] + 1;
		nextCol = targetPosition->x + col;
		if (nextRow >= MAP_ROW || map->Map[nextRow][nextCol] != BlockTile_Empty)
		{
			return true;
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

void ClearLine(TetrisMap* map, int row)
{
	if (row < 0 || row >= MAP_ROW)
	{
		printf("ClearLine: Out of index, row: '%d'\n", row);
		return;
	}

	memset(map->Map[row], 0, MAP_SIZE / MAP_ROW);
}

int ClearFullLine(TetrisMap* map)
{
	int clearedLine = 0;
	int emptyLineNumber = 0;
	int row, emptyRow, targetRow;
	bool emptyLineChecker[MAP_ROW];

	memset(emptyLineChecker, false, sizeof(bool) * MAP_ROW);

	for (row = MAP_ROW - 1; row >= 0; row--)
	{
		if (IsLineFull(map, row) == true)
		{
			ClearLine(map, row);
			clearedLine++;
			emptyLineNumber++;
			emptyLineChecker[row] = true;
		}
		else if (emptyLineNumber > 0)
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

	if (IsCollide(map, &block, &block.Position, true) == true)
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

bool AddBlock(TetrisMap* map, Block* block)
{
	int row, col;

	if (IsOutOfMap(map, block, &block->Position) == true)
	{
		printf("AddBlock: Block is out of map, Block: (Position: (x: '%d', y: '%d'))\n",
			block->Position.x, block->Position.y);
		return false;
	}

	for (row = 0; row < BLOCK_SHAPE_ROW; row++)
	{
		for (col = 0; col < BLOCK_SHAPE_COL; col++)
		{
			if (block->Shape[row][col] != 0)
			{
				map->Map[block->Position.y + row][block->Position.x + col] = block->Tile;
			}
		}
	}

	return true;
}

bool MoveBlock(TetrisMap* map, MoveDirection direction)
{
	const Point PRESET[MOVE_DIRECTION_NUMBER] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
	Point movedPosition, shiftAmount;

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

	shiftAmount = PRESET[direction];
	movedPosition = AddPoint(&map->CurrentBlock.Position, &shiftAmount);

	if (IsCollide(map, &map->CurrentBlock, &movedPosition, true) == true)
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

	if (map->AllowWallKick == true && IsOutOfMap(map, &rotatedBlock, &rotatedBlock.Position) == true)
	{
		if (rotatedBlock.Position.x < MAP_COL / 2)
		{
			rotatedBlock.Position.x++;
		}
		else
		{
			rotatedBlock.Position.x--;
		}
	}

	if (IsCollide(map, &rotatedBlock, &rotatedBlock.Position, false) == true)
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
		/*printf("DropDownBlock: Cannot drop down anymore, DropDownPosition: (x: '%d', y: '%d')\n",
			dropDownPosition.x, dropDownPosition.y);*/
		return false;
	}

	map->CurrentBlock.Position = dropDownPosition;
	return true;
}

Point GetDropDownPosition(TetrisMap* map, Block* block)
{
	int y;
	Point dropDownPosition, testPosition;

	dropDownPosition = block->Position;
	testPosition.x = block->Position.x;

	for (y = block->Position.y; y < MAP_ROW; y++)
	{
		testPosition.y = y;

		if (IsCollide(map, block, &testPosition, true) == true)
		{
			break;
		}

		dropDownPosition = testPosition;
	}

	return dropDownPosition;
}