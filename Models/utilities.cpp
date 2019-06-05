#include "utilities.h"

float utilities::distance(coord a, coord b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) * 1.0);
}
