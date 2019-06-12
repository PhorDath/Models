#ifndef PMEDIANS_H
#define PMEDIANS_H

//#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <gurobi_c++.h>
#include "useful.h"
#include "utilities.h"
#include "strf.h"
//#include "split.h"

#define TMAX 3600

using namespace std;

class pmedians
{
private:
	// input data
	//string dir = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/01/Instances/pmedianas/";
	string fileName = "";
	string directory = "";
	int numMedians;
	int numClients, n;
	int numPlaces, m;
	int placesCapacity;
	vector<int> demands;
	vector<coord> coordsClients;
	vector<coord> coordsPlaces;
	vector<vector<float>> distanceMatrix;
	
	void readInstance(); // read the instances given by the professor

	// model variables (Gurobi)
	//GRBModel *model;
	vector<vector<GRBVar>> x;
	vector<GRBVar> y;

	// model setup functions
	void varX(GRBModel &model);
	void varY(GRBModel &model);
	void fo(GRBModel &model);
	void c1(GRBModel &model); // each client i must be attended by only one facility
	void c2(GRBModel &model); // exactly p facilities must be opened
	void c3(GRBModel &model); // the demand's sum can't be higher then the facility capacity
	void c4(GRBModel &model); // each client must be designated to a opend facility

public:
			pmedians(string fileName);
			pmedians(string directory, string fileName);
	void	setupModel();
	void	getSolutionFull(GRBModel &model); // this function writes into a file the solution of the model as it is
	void	getSolution(GRBModel &model); // 
	void	printData();
			~pmedians();
};

#endif
