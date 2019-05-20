#pragma once

#include <stdbool.h>
#include "BlockBag.h"
#include "Control.h"
#include "Map.h"
#include "Point.h"
#include "TickTimer.h"

typedef struct TetrisCore_t
{
	unsigned int Stage;
	unsigned int Score;
	unsigned int Gravity;
	unsigned int LockDelay;

	TickTimer GravityTimer;
	TickTimer LockTimer;
} TetrisCore;

typedef struct TetrisInfo_t
{
	unsigned int Score;
	bool IsRunning;
} TetrisInfo;

typedef struct TetrisGame_t
{
	TetrisMap GameMap;
	TetrisInfo GameInfo;
	TetrisBlockBag BlockBag;
} TetrisGame;

void InitializeTetris(TetrisGame* tetris);
void RunTetris(TetrisGame* tetris);

void UpdateTetris(TetrisGame* tetris);

void HandleUserInput(TetrisGame* tetris, UserInput input);