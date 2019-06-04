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
#include "useful.h"
#include "strf.h"
//#include "split.h"

using namespace std;

typedef vector<vector<GRBVar>> X;

class designation
{
private:
	// input data
	string fileName;
	int n; // task
	int m; // machines
	vector<vector<int>> C; // cost
	vector<vector<int>> A; // resources
	vector<int> B; // machines capacity
	
	// gurobi variables
	X x; // 1 if task j is assigned to machine i, 0 otherwise

	// decision variables setup
	void varX(GRBModel &);

	// objective function setup
	void fo(GRBModel &);

	// constrains setup
	void c1(GRBModel &);
	void c2(GRBModel &);

public:
	designation(string);
	void setupModel();
	void getSolution(GRBModel &);
	void printData();
	~designation();
};

#endif