#ifndef TSP_H
#define TSP_H

#include <fstream>
#include <string>
#include <vector>
#include <gurobi_c++.h>
#include "utilities.h"
#include "strf.h"

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

	void readInstance(); // read professor instances

	// decision variable
	vector<vector<GRBVar>> x;
	vector<vector<GRBVar>> f;

	// model setup functions
	void varX(GRBModel &model); // 1 if edge from city i to j is used, 0 otherwise
	void varF(GRBModel &model); // this set of variable represents the flow, used on the Gavish and Graves model (PIM) to avoid subcicles
	void fo(GRBModel &model); // minimize the total distance
	void c1(GRBModel &model); // from each city arrives only one edge
	void c2(GRBModel &model); // from each city leaves only one edge
	// PIM model constrains
	void c3(GRBModel &model); //
	void c4(GRBModel &model); //

public:
	tsp(string fileName);
	tsp(string directory, string fileName);
	void setupModel();
	void pimModel(GRBModel &model); // call only setup functions for the pim model
	void getSolution(GRBModel &model);
	void printData();
	~tsp();
};

#endif // !TSP_H