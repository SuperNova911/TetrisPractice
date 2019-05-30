#pragma once

#include <stdbool.h>
#include "BlockBag.h"
#include "Control.h"
#include "Map.h"
#include "Point.h"
#include "TickTimer.h"

typedef struct TetrisCore_t
{
	TickTimer GravityTimer;
	TickTimer LockTimer;
	bool WaitForLock;
} TetrisCore;

typedef struct TetrisInfo_t
{
	unsigned int Stage;
	unsigned int Score;
	int Combo;
	unsigned int Gravity;
	unsigned int LockDelay;
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

void AddScore(TetrisGame* tetris, unsigned int clearedLine);