#pragma once

#include <stdbool.h>

typedef struct TickTimer_t
{
	unsigned int Delay;
	long LastTick;
} TickTimer;

void InitializeTickTimer(TickTimer* timer, unsigned int delay);
void WaitNextTick(TickTimer* timer);
void RestartTimer(TickTimer* timer);
bool IsReady(TickTimer* timer);

long GetTickCount_Tetris();