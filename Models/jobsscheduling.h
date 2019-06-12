#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <gurobi_c++.h>
#include "strf.h"

#define TMAX 3600

class jobsScheduling
{
private:
	// problem parameters
	string fileName;
	string directory = "";
	int numJobs;
	int numMachines;
	int bigM;
	vector<int> Q; // machines capacity
	vector<vector<int>> p; // processing time of job i in machine j
	vector<int> s; // size of jobs
	vector<int> r; // ready time for jobs

	void readInstance(); // read the instances given by the professor

	// decision variables
	vector<vector<vector<GRBVar>>> x; // xijk = 1 if job i precede job j in machine k
	vector<vector<GRBVar>> c; // cik = finishing time of job i in machine k
	vector<GRBVar> t; // delay on job i

	// setup functions
	void varX(GRBModel &model);
	void varC(GRBModel &model);
	void varT(GRBModel &model);
	void fo(GRBModel &model);
	void c1(GRBModel &model); // each job j has a unique immediate predecessor job in a machine
	void c2(GRBModel &model); // each machine k, if used, has a unique processing sequence
	void c3(GRBModel &model); // each job h must have only one immediate predecessor job, excluding the fake initial job (0)
	void c4(GRBModel &model); // the first job (fake), must end in the time 0
	void c5(GRBModel &model); // calculates the end time of jobs in your respectives machines
	void c6(GRBModel &model); // calculates jobs delay

public:
	jobsScheduling(string fileName);
	jobsScheduling(string directory, string fileName);
	void setupModel();
	void getSolutionFull(GRBModel &model);
	void getSolution(GRBModel &model);
	void printData();
	~jobsScheduling();
};

