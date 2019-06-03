#include <stdbool.h>
#include <stdio.h>
//#include <time.h>
#include <Windows.h>
#include "TickTimer.h"

void InitializeTickTimer(TickTimer* timer, unsigned int delay)
{
	timer->Delay = delay;
	timer->LastTick = GetTickCount_Windows();
	timer->RemainTick = 0;
	timer->IsRunning = false;
}

void ChangeDelay(TickTimer* timer, unsigned int delay)
{
	timer->Delay = delay;
}

void RunTimer(TickTimer* timer)
{
	if (timer->IsRunning == true)
	{
		printf("RunTimer: Timer is already running\n");
		return;
	}

	timer->IsRunning = true;
}

void PauseTimer(TickTimer* timer)
{
	if (timer->IsRunning == false)
	{
		printf("PauseTimer: Timer is already paused\n");
		return;
	}

	timer->RemainTick = GetTickCount_Windows() - timer->LastTick;
	timer->IsRunning = false;
}

void ResumeTimer(TickTimer* timer)
{
	if (timer->IsRunning == true)
	{
		printf("ResumeTimer: Timer is already running\n");
		return;
	}

	timer->LastTick = GetTickCount_Windows() - timer->RemainTick;
	timer->IsRunning = true;
}

void RestartTimer(TickTimer* timer)
{
	timer->LastTick = GetTickCount_Windows();
	timer->IsRunning = true;
}

bool IsTimerReady(TickTimer* timer)
{
	static long CurrentTick;

	if (timer->IsRunning == false)
	{
		return false;
	}

	CurrentTick = GetTickCount_Windows();

	if (CurrentTick - timer->LastTick >= (long)timer->Delay)
	{
		timer->LastTick = CurrentTick;
		return true;
	}

	return false;
}

long GetTickCount_Windows()
{
	/*struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_nsec / 1000000) + (ts.tv_sec * 1000);*/

	return (long)GetTickCount64();
}
