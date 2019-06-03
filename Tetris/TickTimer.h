#pragma once

#include <stdbool.h>

typedef struct TickTimer_t
{
	unsigned int Delay;
	long LastTick;
	long RemainTick;
	bool IsRunning;
} TickTimer;

void InitializeTickTimer(TickTimer* timer, unsigned int delay);
void ChangeDelay(TickTimer* timer, unsigned int delay);

void RunTimer(TickTimer* timer);
void PauseTimer(TickTimer* timer);
void ResumeTimer(TickTimer* timer);
void RestartTimer(TickTimer* timer);

//void WaitNextTick(TickTimer* timer);
bool IsTimerReady(TickTimer* timer);

long GetTickCount_Windows();