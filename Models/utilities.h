#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>

namespace utilities {
	struct coord
	{
		int x, y;
	};

	float distance(coord, coord);
}

#endif // !UTILITIES_H