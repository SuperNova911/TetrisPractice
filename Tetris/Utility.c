#pragma once

#include <stdbool.h>
#include <time.h>
#include <Windows.h>
#include "Utility.h"

Point AddPoint(Point* source1, Point* source2)
{
	Point addResult;
	addResult.x = source1->x + source2->x;
	addResult.y = source1->y + source2->y;

	return addResult;
}

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
			break;
		}
	}
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