#ifndef VRP_H
#define VRP_H

#include <fstream>
#include <string>
#include <vector>
#include <gurobi_c++.h>
#include "strf.h"
#include "utilities.h"

using namespace std;

class vrp
{
private:
	// problem parameters
	string fileName;
	int numClients;
	int n;
	int vehicleCapacity;
	utilities::coord depotCoord;
	vector<utilities::coord> coords;
	vector<float> q; // demand for each client
	vector<vector<int>> distance; // distance from node i to j
	// vector<vector<float>> C; // cost to travel from node i to j
	// vector<float> Q; // capacity of each vehicle

	// decision variables
	vector<vector<GRBVar>> x; // 1 if arch (i, j) is used, 0 otherwise
	vector<vector<GRBVar>> f; // amount of flow sent from i to j

	// model setup functions
	void varX(GRBModel &);
	void varF(GRBModel &);
	void fo(GRBModel &);
	void c1(GRBModel &);
	void c2(GRBModel &);
	void c3(GRBModel &); // number of arcs exiting the depot must be the same amount of arcs entering the depot
	void c4(GRBModel &); // flow conservation
	void c5(GRBModel &); // vehicle capacity must be respected

public:
	vrp(string);
	void setupModel();
	void getSolution(GRBModel &);
	void printData();
	~vrp();
};

#endif // !VRP_H