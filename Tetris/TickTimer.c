#include <stdbool.h>
//#include <time.h>
#include <Windows.h>
#include "TickTimer.h"

void InitializeTickTimer(TickTimer* timer, unsigned int delay)
{
	timer->Delay = delay;
	timer->LastTick = GetTickCount_Windows();
}

void WaitNextTick(TickTimer* timer)
{
	while (true)
	{
		if (IsReady(timer) == true)
		{
			return;
		}
	}
}

void RestartTimer(TickTimer* timer)
{
	timer->LastTick = GetTickCount_Windows();
}

bool IsReady(TickTimer* timer)
{
	static long CurrentTick;
	CurrentTick = GetTickCount_Windows();

	if (CurrentTick - timer->LastTick >= timer->Delay)
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
