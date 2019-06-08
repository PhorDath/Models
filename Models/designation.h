#ifndef DESIGNATION_H
#define DESIGNATION_H
/*
	this is a linear programing model for the generalized designation problem
	implemented using the gurobi solver
 */

//#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <gurobi_c++.h>
#include "utilities.h"
#include "useful.h"
#include "strf.h"

using namespace std;

class designation
{
private:
	// input data
	string fileName;
	string directory = "";
	int n; // task
	int m; // machines
	vector<vector<int>> C; // cost
	vector<vector<int>> A; // resources
	vector<int> B; // machines capacity
	
	void readInstance(); // read the instances given by the professor

	// gurobi variables
	vector<vector<GRBVar>> x; // 1 if task j is assigned to machine i, 0 otherwise

	// decision variables setup
	void varX(GRBModel &model);
	void fo(GRBModel &model);
	void c1(GRBModel &model); // the resource capacity on each machine must be respected
	void c2(GRBModel &model); // each task must be executed in only one machine

public:
	designation(string fileName);
	designation(string directory, string fileName);
	void setupModel();
	void getSolution(GRBModel &model);
	void printData();
	~designation();
};

#endif