#ifndef TSP_H
#define TSP_H

#include <string>
#include <vector>
#include <gurobi_c++.h>

using namespace std;

class tsp
{
private:
	// problem parameters
	vector<vector<float>> distance;

	// decision variable
	vector<vector<GRBVar>> x;

	void FO(GRBModel &);
	void c1(GRBModel &);
	void c2(GRBModel &);

public:
	tsp(string);
	void printData();
	~tsp();
};

#endif // !TSP_H