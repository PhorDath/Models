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
	void varX(GRBModel &model); // xjbk = 1 if job is designated to batch b on machine k
	void varSB(GRBModel &model); // sbk = begining time of batch b on machine k
	void varPB(GRBModel &model); // pbk = processing time of batch b on machine k
	void varC(GRBModel &model); // makespan
	void fo(GRBModel &model); // minimize the makespan
	void c1(GRBModel &model); // each job j must be on a unique batch
	void c2(GRBModel &model); // the sum of the jobs in the batch cant be higher then the machine capacity
	void c3(GRBModel &model); // a batch can only begging after all his jobs are available
	void c4(GRBModel &model); // a batch batch should start after the previous batch ends
	void c5(GRBModel &model); // The processing time of a batch is the longest of its tasks
	void c6(GRBModel &model); // Determines is the longest completion time on each machine

public:
	batchScheduling(string fileName);
	batchScheduling(string directory, string fileName);
	void setupModel();
	void getSolutionFull(GRBModel &model);
	void getSolution(GRBModel &model);
	void printData();
	~batchScheduling();
};

