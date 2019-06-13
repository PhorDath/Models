#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <gurobi_c++.h>
#include "utilities.h"
#include "strf.h"

#define TMAX 3600

class batchScheduling
{
private:
	// problem parameters
	string fileName;
	string directory = "";
	int numJobs, n;
	int numMachines, m;
	int bigM;
	vector<int> Q; // machines capacity
	vector<vector<int>> p; // processing time of job i in machine j
	vector<int> s; // size of jobs
	vector<int> r; // ready time for jobs

	void readInstance(); // read the instances given by the professor

	// decision variables
	vector<vector<vector<GRBVar>>> x; // 
	vector<vector<GRBVar>> sb; //
	vector<vector<GRBVar>> pb; //
	GRBVar c; //

	// setup functions
	void varX(GRBModel &model);
	void varSB(GRBModel &model);
	void varPB(GRBModel &model);
	void varC(GRBModel &model);
	void fo(GRBModel &model);
	void c1(GRBModel &model); // each job j has a unique immediate predecessor job in a machine
	void c2(GRBModel &model); // each machine k, if used, has a unique processing sequence
	void c3(GRBModel &model); // each job h must have only one immediate predecessor job, excluding the fake initial job (0)
	void c4(GRBModel &model); // the first job (fake), must end in the time 0
	void c5(GRBModel &model); // calculates the end time of jobs in your respectives machines
	void c6(GRBModel &model); // calculates jobs delay
public:
	batchScheduling(string fileName);
	batchScheduling(string directory, string fileName);
	void setupModel();
	void getSolutionFull(GRBModel &model);
	void getSolution(GRBModel &model);
	void printData();
	~batchScheduling();
};

