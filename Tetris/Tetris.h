#pragma once

#include <stdbool.h>
#include "BlockBag.h"
#include "Control.h"
#include "Map.h"
#include "Point.h"
#include "TickTimer.h"

#define TETRIS_MAX_LEVEL 9

typedef struct TetrisCore_t
{
	TickTimer GravityTimer;
	TickTimer LockTimer;
	TickTimer SpawnTimer;
	bool WaitForLock;
} TetrisCore;

typedef struct TetrisInfo_t
{
	unsigned int Level;
	unsigned int Score;
	unsigned int TotalClearedLine;
	int Combo;
	unsigned int Gravity;
	unsigned int LockDelay;
	unsigned int SpawnDelay;
	bool IsRunning;
	bool IsGameOver;
} TetrisInfo;

typedef struct TetrisGame_t
{
	TetrisMap GameMap;
	TetrisCore GameCore;
	TetrisInfo GameInfo;
	TetrisBlockBag BlockBag;
	InputInfo UserInput;
} TetrisGame;

void InitializeTetris(TetrisGame* tetris);
void RunTetris(TetrisGame* tetris);
bool IsTetrisGameOver(TetrisGame* tetris);

void UpdateTetris(TetrisGame* tetris);
void UpdateGravity(TetrisGame* tetris);
void UpdateLock(TetrisGame* tetris);
void Lock(TetrisGame* tetris);

void ReadUserInput(TetrisGame* tetris, InputCollection* InputCollection);
void HandleUserInput(TetrisGame* tetris);
void InputTest(TetrisGame* tetirs, InputInfo* inputInfo);

void ControlBlockMovement(TetrisGame* tetris, MoveDirection direction);
void ControlBlockRotation(TetrisGame* tetris, RotateDirection direction);
void ControlBlockDropDown(TetrisGame* tetris);

void LevelUp(TetrisGame* tetris, unsigned int clearedLine);
void AddScore(TetrisGame* tetris, unsigned int clearedLine);