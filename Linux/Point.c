#include <stdbool.h>
#include "Point.h"

Point AddPoint(Point* point1, Point* point2)
{
	Point addResult;
	addResult.x = point1->x + point2->x;
	addResult.y = point1->y + point2->y;

	return addResult;
}

bool IsEqualPoint(Point* point1, Point* point2)
{
	return point1->x == point2->x && point1->y == point2->y;
}
