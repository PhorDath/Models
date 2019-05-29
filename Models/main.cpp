#include <iostream>
#include <gurobi_c++.h>
#include "pmedians.h"

using namespace std;

int main() {
	pmedians p("Probs_5_med_70_50.txt");
	p.printData();
	return 0;
}