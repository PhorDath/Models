#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <gurobi_c++.h>
#include "strf.h"
#include "utilities.h"

#define TMAX 3600

using namespace std;

class flowshop
{
private:
	// problem parameters
	string fileName;
	string directory = "";
	int numTasks;
	int numMachines;
	vector<vector<int>> p;

	void readInstance(); // read the instances given by the professor

	// model decision variables
	vector<vector<GRBVar>> x; // xij = 1 if task i is assigned to position j, 0 otherwise
	vector<vector<GRBVar>> s; // ski = start time of task's i processing in machine k
	GRBVar c; // makespan (maximum completion time)

	//
	void varX(GRBModel &model);
	void varS(GRBModel &model);
	void varC(GRBModel &model);
	void fo(GRBModel &model);
	void c1(GRBModel &model); // each task i must be associated to a unique position j
	void c2(GRBModel &model); // each position j must be associated to a unique task i
	void c3(GRBModel &model); // task 0 must begin at time 0 in the machine 0
	void c4(GRBModel &model); // calculates processing times for every tasks in machine 0
	void c5(GRBModel &model); // calculates processing times of the first taks in machines 2 to m-1
	void c6(GRBModel &model); // the task in the position j >= 2 cant be started in the next machine k+1 before be finished on your current machine
	void c7(GRBModel &model); // the task in position j+1 will begin to be proccessed on machine k after the task of position j finish your processing in the same machine

public:
	flowshop(string fileName);
	flowshop(string directory, string fileName);
	void setupModel();
	void getSolutionFull(GRBModel &model);
	void getSolution(GRBModel &model);
	void printData();
	~flowshop();
};

