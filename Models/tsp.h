#ifndef TSP_H
#define TSP_H

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <gurobi_c++.h>
#include "utilities.h"
#include "strf.h"

#define TMAX 3600

using namespace std;

class tsp
{
private:
	// problem parameters
	string fileName;
	string directory;
	int numNodes;
	vector<utilities::coord> coordinates;
	vector<vector<int>> dMatrix;
	int bigM;

	vector<int> permutation;

	void readInstance(); // read professor instances
	void pimModel(GRBModel &model); // call only setup functions for the pim model
	void checkPermutation();

	// decision variable
	vector<vector<GRBVar>> x;
	vector<vector<GRBVar>> f;

	// model setup functions
	void varX(GRBModel &model); // 1 if edge from city i to j is used, 0 otherwise
	void varF(GRBModel &model); // this set of variable represents the flow, used on the Gavish and Graves model (PIM) to avoid subcicles
	void fo(GRBModel &model); // minimize the total distance
	void c1(GRBModel &model); // from each city arrives only one arc
	void c2(GRBModel &model); // from each city leaves only one arc
	// PIM model constrains
	void c3(GRBModel &model); // flow conservation
	void c4(GRBModel &model); // 

public:
	tsp(string fileName);
	tsp(string directory, string fileName);
	void setupModel();
	void getSolutionFull(GRBModel &model);
	void getSolution(GRBModel &model);
	void printData();
	~tsp();
};

#endif // !TSP_H