#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>
#include <iostream>
#include <gurobi_c++.h>
using namespace std;

namespace utilities {
	struct coord
	{
		int x, y;
	};

	float distance(coord, coord);
	void processSolution(GRBModel &);
}

#endif // !UTILITIES_H