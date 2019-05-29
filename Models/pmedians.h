#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <gurobi_c++.h>
//#include "split.h"
#include "useful.h"

using namespace std;

typedef vector<vector<GRBVar>> X;
typedef vector<GRBVar> Y;

class pmedians
{
private:
	// input data
	string dir = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/01/Instances/pmedianas/";
	string fileName;
	int numMedians;
	int numClients, n;
	int numPlaces, m;
	int placesCapacity;
	vector<int> demands;
	vector<coord> coordsClients;
	vector<coord> coordsPlaces;
	vector<vector<float>> distanceMatrix;
	
	// model variables (Gurobi)
	//GRBModel *model;
	X x;
	Y y;

	// decision variables setup
	void varX(GRBModel &);
	void varY(GRBModel &);

	// objective function setup
	void fo(GRBModel &);

	// constrains setup
	void c1(GRBModel &);
	void c2(GRBModel &);
	void c3(GRBModel &);
	void c4(GRBModel &);

public:
			pmedians(string);
	void	setupModel();
	void	getSolution(GRBModel &);
	void	printData();
			~pmedians();
};

