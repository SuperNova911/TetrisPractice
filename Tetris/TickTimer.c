#include <stdbool.h>
#include <Windows.h>
#include "TickTimer.h"

void InitializeTickTimer(TickTimer* timer, unsigned int delay)
{
	timer->Delay = delay;
	timer->LastTick = GetTickCount64();
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
	timer->LastTick = GetTickCount64();
}

bool IsReady(TickTimer* timer)
{
	static long CurrentTick;
	CurrentTick = GetTickCount64();

	if (CurrentTick - timer->LastTick >= timer->Delay)
	{
		timer->LastTick = CurrentTick;
		return true;
	}

	return false;
}