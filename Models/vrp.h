#ifndef VRP_H
#define VRP_H

#include <fstream>
#include <string>
#include <vector>
#include <gurobi_c++.h>
#include "strf.h"
#include "utilities.h"

#define TMAX 3600

using namespace std;

class vrp
{
private:
	// problem parameters
	string fileName;
	string directory = "";
	int numClients;
	int n;
	int vehicleCapacity;
	utilities::coord depotCoord;
	vector<utilities::coord> coords;
	vector<float> q; // demand for each client
	vector<vector<int>> distance; // distance from node i to j
	// vector<vector<float>> C; // cost to travel from node i to j
	// vector<float> Q; // capacity of each vehicle

	void readInstance(); // read the instances given by the professor

	// decision variables
	vector<vector<GRBVar>> x; // 1 if arch (i, j) is used, 0 otherwise
	vector<vector<GRBVar>> f; // amount of flow sent from i to j

	// model setup functions
	void varX(GRBModel &model);
	void varF(GRBModel &model);
	void fo(GRBModel &model);
	void c1(GRBModel &model);
	void c2(GRBModel &model);
	void c3(GRBModel &model); // number of arcs exiting the depot must be the same amount of arcs entering the depot
	void c4(GRBModel &model); // flow conservation
	void c5(GRBModel &model); // vehicle capacity must be respected

public:
	vrp(string fileName);
	vrp(string directory, string fileName);
	void setupModel();
	void getSolutionFull(GRBModel &model);
	void getSolution(GRBModel &model);
	void printData();
	~vrp();
};

#endif // !VRP_H