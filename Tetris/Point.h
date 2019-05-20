#pragma once

#include <stdbool.h>

typedef struct Point_t
{
	int x;
	int y;
} Point;

Point AddPoint(Point* point1, Point* point2);
bool IsEqualPoint(Point* point1, Point* point2);