#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <gurobi_c++.h>
#include "strf.h"

using namespace std;

class flowshop
{
private:
	// problem parameters
	string fileName;
	int numTasks;
	int numMachines;
	vector<vector<int>> p;

	// model decision variables
	vector<vector<GRBVar>> x; // xij = 1 if task i is assigned to position j, 0 otherwise
	vector<vector<GRBVar>> s; // ski = start time of task's i processing in machine k
	GRBVar c; // makespan (maximum completion time)

	//
	void varX(GRBModel &);
	void varS(GRBModel &);
	void varC(GRBModel &);
	void fo(GRBModel &);
	void c1(GRBModel &); // each task i must be associated to a unique position j
	void c2(GRBModel &); // each position j must be associated to a unique task i
	void c3(GRBModel &); // task 0 must begin at time 0 in the machine 0
	void c4(GRBModel &); // calculates processing times for every tasks in machine 0
	void c5(GRBModel &); // calculates processing times of the first taks in machines 2 to m-1
	void c6(GRBModel &); // the task in the position j >= 2 cant be started in the next machine k+1 before be finished on your current machine
	void c7(GRBModel &); // the task in position j+1 will begin to be proccessed on machine k after the task of position j finish your processing in the same machine

public:
	flowshop(string);
	void setupModel();
	void getSolution(GRBModel &);
	void printData();
	~flowshop();
};

