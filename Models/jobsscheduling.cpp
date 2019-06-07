#include "jobsScheduling.h"

jobsScheduling::jobsScheduling(string fName)
{
	fileName = fName;
	fstream file(fName, ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		exit(1);
	}

	string line;
	int counter{ 1 };
	while (getline(file, line)) {
		try {
			auto tokens = strf::split(line, ' ');
			if (counter == 2) {
				// reading number of jobs and machines
				numJobs = stoi(tokens.at(0));
				numMachines = stoi(tokens.at(1));
			}
			else if (counter == 4) {
				// reading machines capacity
				for (int i = 0; i < numMachines; i++) {
					Q.push_back(stoi(tokens.at(i)));
				}
			}
			else if (counter >= 6 && counter < 6 + numJobs) {
				// reading processing time of jobs for each machine
				vector<int> aux;
				for (int i = 0; i < numMachines; i++) {
					aux.push_back(stoi(tokens.at(i)));
				}
				p.push_back(aux);
			}
			else if (counter == 6 + numJobs + 2) {
				// reading size of jobs;
				for (auto i : tokens) {
					s.push_back(stoi(i));
				}
			}
			else if (counter == 6 + numJobs + 2 + 2) {
				// reading ready time for each job
				for (auto i : tokens) {
					r.push_back(stoi(i));
				}
			}
		}
		catch (exception e) {
			cout << e.what() << endl;
			exit(1);
		}
		counter++;
	}
}

void jobsScheduling::setupModel()
{
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		//env = new GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "jobsScheduling");

		varX(model);
		fo(model);

		c1(model);
		c2(model);
		c3(model);
		c4(model);
		c5(model);
		c6(model);

		model.write("teste.lp");

		model.getEnv().set(GRB_DoubleParam_TimeLimit, 600);

		model.optimize();

		getSolution(model);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void jobsScheduling::getSolution(GRBModel &model)
{
	int status = model.get(GRB_IntAttr_Status);

	if (status == GRB_UNBOUNDED)
	{
		cout << "O modelo nao pode ser resolvido porque e ilimitado" << endl;
	}
	if (status == GRB_OPTIMAL)
	{
		cout << "Solucao otima encontrada!" << endl;
		//Acessa e imprime o valor da funcao objetivo

		cout << "O valor da solucao otima e: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	}
	if (status == GRB_TIME_LIMIT)
	{
		cout << "Tempo limite!" << endl;
		//Acessa e imprime o valor da funcao objetivo

		cout << "O valor da melhot solucao ate o momento e: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	}
	if (status == GRB_INFEASIBLE)
	{
		cout << "O modelo nao pode ser resolvido porque e inviavel" << endl;

	}
	else {
		cout << "Status: " << status << endl;

	}
}

void jobsScheduling::printData()
{
	cout << numJobs << " " << numMachines << endl;
	cout << "Q: ";
	for (auto i : Q) {
		cout << i << " ";
	}
	cout << endl << "p: \n";
	for (auto i : p) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
	cout << "s: ";
	for (auto i : s) {
		cout << i << " ";
	}
	cout << endl << "r: ";
	for (auto i : r) {
		cout << i << " ";
	}
	cout << endl;
}

jobsScheduling::~jobsScheduling()
{
}

void jobsScheduling::varX(GRBModel &model)
{
	// resize x
	x.resize(numJobs);
	for (int i = 0; i < numJobs; i++) {
		x.at(i).resize(numJobs);
		for (int j = 0; j < numJobs; j++) {
			x.at(i).at(j).resize(numMachines);
		}
	}
	// add var x to model
	for (int i = 0; i < numJobs; i++) {
		for (int j = 0; j < numJobs; j++) {
			for (int k = 0; k < numMachines; k++) {
				x.at(i).at(j).at(k) = model.addVar(0, 1, 1, GRB_BINARY, "x(" + to_string(i) + "," + to_string(j) + "," + to_string(k) + ")");
			}
		}
	}
	model.update();
}

void jobsScheduling::varC(GRBModel &model)
{
	// resize c
	c.resize(numJobs);
	for (int i = 0; i < numJobs; i++) {
		c.at(i).resize(numMachines);
	}
	// add var c to model
	for (int i = 0; i < numJobs; i++) {
		for (int k = 0; k < numMachines; k++) {
			c.at(i).at(k) = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "c(" + to_string(i) + "," + to_string(k) + ")");
		}
	}
	model.update();
}

void jobsScheduling::varT(GRBModel &model)
{
	// resize t
	t.resize(numJobs);
	// add t to model
	for (int i = 0; i < numMachines; i++) {
		t.at(i) = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "t(" + to_string(i) + ")");
	}
	model.update();
}

void jobsScheduling::fo(GRBModel &model)
{
	GRBLinExpr fo{ 0 };
	for (int i = 0; i < numJobs; i++) {
		fo += t.at(i);
	}
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void jobsScheduling::c1(GRBModel &model)
{
	for (int j = 0; j < numJobs; j++) {
		GRBLinExpr c1{ 0 };
		for (int k = 0; k < numMachines; k++) {
			for (int i = 0; i < numJobs; i++) {
				c1 += x.at(i).at(j).at(k);
			}
		}
		model.addConstr(c1 == 1, "c(" + to_string(j) + ")");
	}
	model.update();
}

void jobsScheduling::c2(GRBModel &model)
{
	for (int k = 0; k < numMachines; k++) {
		GRBLinExpr c2{ 0 };
		for (int j = 0; j < numJobs; j++) {
			c2 += x.at(0).at(j).at(k);
		}
		model.addConstr(c2 <= 1, "c2(" + to_string(k) + ")");
	}
	model.update();
}

void jobsScheduling::c3(GRBModel &model)
{
	for (int h = 0; h < numJobs; h++) {		
		for (int k = 0; k < numMachines; k++) {
			GRBLinExpr c31{ 0 };
			GRBLinExpr c32{ 0 };
			for (int i = 0; i < numJobs; i++) {
				if (i != h) {
					c31 += x.at(i).at(h).at(k);
				}				
			}
			for (int j = 0; j < numJobs; j++) {
				if (j != h) {
					c32 += x.at(h).at(j).at(k);
				}				
			}
			model.addConstr(c31 - c32 == 0, "c3(" + to_string(h) + "," + to_string(k) + ")");
		}
	}
	model.update();
}

void jobsScheduling::c4(GRBModel &model)
{
	for (int k = 0; k < numMachines; k++) {
		model.addConstr(c.at(0).at(k) == 0, "c4(" + to_string(k) + ")");
	}
	model.update();
}

void jobsScheduling::c5(GRBModel &model)
{
	for (int i = 0; i < numJobs; i++) {
		for (int j = 1; j < numJobs; j++) {
			for (int k = 1; k < numMachines; k++) {
				GRBLinExpr c51{ 0 };
				GRBLinExpr c52{ 0 };
				c51 = c.at(j).at(k);
				c52 = c.at(i).at(k) - bigM + (bigM + p.at(j).at(k)) * x.at(i).at(j).at(k);
				model.addConstr(c51 >= c52, "c5(" + to_string(i) + "," + to_string(j) + "," + to_string(k) + ")");
			}
		}
	}
	model.update();
}

void jobsScheduling::c6(GRBModel &model)
{
	for (int i = 1; i < numJobs; i++) {
		for (int k = 1; k < numMachines; k++) {
			GRBLinExpr c61{ 0 };
			GRBLinExpr c62{ 0 };
			c61 = t.at(i);
			c62 = c.at(i).at(k) - r.at(i);
			model.addConstr(c61 >= c62, "c6(" + to_string(i) + "," + to_string(k) + ")");
		}
	}
	model.update();
}
