#include "flowshop.h"



flowshop::flowshop(string fName)
{
	fileName = fName;
	fstream file(fileName, ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		exit(1);
	}
	int counter{ 1 };
	string line;
	while (getline(file, line)) {
		auto tokens = strf::split(line, ' ');
		if (counter == 1) {
			// read number of tasks
			numTasks = stoi(tokens.at(tokens.size() - 1));
		}
		else if (counter == 2) {
			// read number of machines
			numMachines = stoi(tokens.at(tokens.size() - 1));
		}
		else if (counter >= 4) {
			// reading Cij, the time of task i in the machine j
			vector<int> aux;
			for (auto tk : tokens) {
				aux.push_back(stoi(tk));
			}
			p.push_back(aux);
		}
		counter++;
	}
}

void flowshop::setupModel()
{
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		//env = new GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "flowshop");

		varX(model);
		varS(model);
		varC(model);
		fo(model);

		c1(model);
		c2(model);
		c3(model);
		c4(model);
		c5(model);
		c6(model);
		c7(model);

		model.write("teste.lp");

		model.getEnv().set(GRB_DoubleParam_TimeLimit, 600);

		model.optimize();

		getSolution(model);
		model.write("teste.sol");
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void flowshop::getSolution(GRBModel &model)
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

void flowshop::printData()
{
	cout << numTasks << " " << numMachines << endl;
	cout << "p: \n";
	for (auto i : p) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
}


flowshop::~flowshop()
{
}

void flowshop::varX(GRBModel &model)
{
	// resize x
	x.resize(numTasks);
	for (int i = 0; i < numTasks; i++) {
		x.at(i).resize(numTasks);
	}
	// add x to model
	for (int i = 0; i < numTasks; i++) {
		for (int j = 0; j < numTasks; j++) {
			x.at(i).at(j) = model.addVar(0, 1, 1, GRB_BINARY, "x(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
}

void flowshop::varS(GRBModel &model)
{
	// resize s
	s.resize(numMachines);
	for (int i = 0; i < numMachines; i++) {
		s.at(i).resize(numTasks);
	}
	// add x to model
	for (int i = 0; i < numMachines; i++) {
		for (int j = 0; j < numTasks; j++) {
			s.at(i).at(j) = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "s(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
}

void flowshop::varC(GRBModel &model)
{
	c = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "c");
}

void flowshop::fo(GRBModel &model)
{
	GRBLinExpr fo = 0;
	fo += s.at(numMachines - 1).at(numTasks - 1);
	for (int i = 0; i < numTasks; i++) {
		fo += p.at(i).at(numMachines - 1) * x.at(i).at(numTasks - 1);
	}
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void flowshop::c1(GRBModel &model)
{
	for (int i = 0; i < numTasks; i++) {
		GRBLinExpr c1 = 0;
		for (int j = 0; j < numTasks; j++) {
			c1 += x.at(i).at(j);
		}
		model.addConstr(c1 == 1, "c(" + to_string(i) + ")");
	}
	model.update();
}

void flowshop::c2(GRBModel &model)
{
	for (int j = 0; j < numTasks; j++) {
		GRBLinExpr c2 = 0;
		for (int i = 0; i < numTasks; i++) {
			c2 += x.at(i).at(j);
		}
		model.addConstr(c2 == 1, "c(" + to_string(j) + ")");
	}
	model.update();
}

void flowshop::c3(GRBModel &model)
{
	model.addConstr(s.at(0).at(0) == 0, "c3");
}

void flowshop::c4(GRBModel &model)
{
	for (int j = 0; j < numTasks - 1; j++) {
		GRBLinExpr c4 = 0;
		c4 += s.at(0).at(j);
		for (int i = 0; i < numTasks; i++) {
			c4 += p.at(i).at(0) * x.at(i).at(j);
		}
		model.addConstr(c4 == s.at(0).at(j + 1), "c(" + to_string(j) + ")");
	}
	model.update();
}

void flowshop::c5(GRBModel &model)
{
	for (int k = 0; k < numMachines - 1; k++) {
		GRBLinExpr c5 = 0;
		c5 += s.at(k).at(1);
		for (int i = 0; i < numTasks; i++) {
			c5 += p.at(i).at(k) * x.at(i).at(0);
		}
		model.addConstr(c5 == s.at(k + 1).at(1), "c5(" + to_string(k) + ")");
	}
	model.update();
}

void flowshop::c6(GRBModel &model)
{
	for (int j = 1; j < numTasks; j++) {
		for (int k = 0; k < numMachines - 1; k++) {
			GRBLinExpr c6 = 0;
			c6 += s.at(k).at(j);
			for (int i = 0; i < numTasks; i++) {
				c6 += p.at(i).at(k) * x.at(i).at(j);
			}
			model.addConstr(c6 <= s.at(k+1).at(j), "c6(" + to_string(j) + "," + to_string(k) + ")");
		}
	}
	model.update();
}

void flowshop::c7(GRBModel &model)
{
	for (int j = 0; j < numTasks - 1; j++) {
		for (int k = 1; k < numMachines; k++) {
			GRBLinExpr c7 = 0;
			c7 += s.at(k).at(j);
			for (int i = 0; i < numTasks; i++) {
				c7 += p.at(i).at(k) * x.at(i).at(j);
			}
			model.addConstr(c7 <= s.at(k).at(j + 1), "c7(" + to_string(j) + "," + to_string(k) + ")");
		}
	}
	model.update();
}
