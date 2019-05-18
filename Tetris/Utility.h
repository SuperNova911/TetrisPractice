#pragma once

#include <stdbool.h>

typedef struct Point_t
{
	int x;
	int y;
} Point;

Point AddPoint(Point* source1, Point* source2);

typedef struct TickTimer_t
{
	unsigned int Delay;
	long LastTick;
} TickTimer;

void InitializeTickTimer(TickTimer* timer, unsigned int delay);
void WaitNextTick(TickTimer* timer);
bool IsReady(TickTimer* timer);