#include "designation.h"

// create and add var x to model
void designation::varX(GRBModel &model)
{
	x.resize(m);
	for (int i = 0; i < x.size(); i++) {
		x.at(i).resize(n);
	}
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < x.at(i).size(); j++) {
			x.at(i).at(j) = model.addVar(0, 1, 1, GRB_BINARY, "x(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

void designation::fo(GRBModel &model)
{
	GRBLinExpr FO = 0;
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < n; j++) {
			FO += x.at(i).at(j) * C.at(i).at(j);
		}
	}
	model.setObjective(FO, GRB_MINIMIZE); // minimize the total cost
	model.update();
}

void designation::c1(GRBModel &model)
{
	// capacity of each machine must be respected
	for (int i = 0; i < m; i++) {
		GRBLinExpr c1 = 0;
		for (int j = 0; j < n; j++) {
			c1 += A.at(i).at(j) * x.at(i).at(j);
		}
		model.addConstr(c1 <= B.at(i), "c1(" + to_string(i) + ")");
	}
}

void designation::c2(GRBModel &model)
{
	// each task must be executed by one machine
	for (int j = 0; j < n; j++) {
		GRBLinExpr c2 = 0;
		for (int i = 0; i < m; i++) {
			c2 += x.at(i).at(j);
		}
		model.addConstr(c2 == 1, "c2(" + to_string(j) + ")");
	}
}

designation::designation(string fName)
{
	
	this->fileName = fName;
	fstream file(fileName, ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		return;
	}
	cout << fileName << endl;

	int count = 1;
	int countC = 0; // count number of C rows read
	int countA = 0; // conut number of A rows read
	string line;
	while (getline(file, line)) {
		//cout << line << endl;
		vector<string> tokens = strf::split(line, ' ');
		if (count == 2) {
			// gettin number of machines and tasks
			this->m = stoi(tokens.at(0)); // machines
			this->n = stoi(tokens.at(1)); // tasks

			// resize cost matrix
			this->C.resize(m);
			for (int i = 0; i < C.size(); i++) {
				C.at(i).resize(n, 0);
			}
			// resize resource matrix
			this->A.resize(m);
			for (int i = 0; i < A.size(); i++) {
				A.at(i).resize(n, 0);
			}
			// resize capacity vector
			this->B.resize(m, 0);
		}
		else if (count >= 4 && count < 4 + this->m) {
			// reading matrix Cij
			for (int j = 0; j < tokens.size(); j++) {
				try {
					C.at(countC).at(j) = stoi(tokens.at(j));
				}
				catch (out_of_range &e) {
					cout << "Error: " << e.what() << endl;
					cout << "While reading line " << count << endl;
					cout << "Accessing C at position (" << countC << ", " << j << ")" << endl;
					cout << "C have " << C.size() << " X " << C.at(0).size() << " dimension\n";
					exit(1);
				}
				catch (exception &e) {
					cout << "Error: " << e.what() << endl;
					cout << "While reading line " << count << endl;
					cout << "Token: |" << tokens.at(j) << "|" << endl;
					cout << "Line: " << line << endl;
					exit(1);
				}
			}
			countC++;
		}
		else if (count >= 4 + this->m + 1 && count < 4 + this->m + 1 + this->m) {
			// reaing matrix Aij
			for (int j = 0; j < tokens.size(); j++) {
				try {
					A.at(countA).at(j) = stoi(tokens.at(j));
				}
				catch (const out_of_range &e) {
					cout << "Error: " << e.what() << endl;
					cout << "While reading line " << count << endl;
					cout << "Accessing A at position (" << countA << ", " << j << ")" << endl;
					cout << "A have " << A.size() << " X " << A.at(0).size() << " dimension\n";
					exit(1);

				}
				catch (exception &e) {
					cout << "Error: " << e.what() << endl;
					cout << "While reading line " << count << endl;
					cout << "Token: |" << tokens.at(j) << "|" << endl;
					cout << "Line: " << line << endl;
					exit(1);
				}
			}
			countA++;
		}
		else if (count == 4 + this->m + 1 + this->m + 1) {
			// reading machines capacity
			for (int j = 0; j < tokens.size(); j++) {
				try {
					B.at(j) = stoi(tokens.at(j));
				}
				catch (const out_of_range &e) {
					cout << "Error: " << e.what() << endl;
					cout << "While reading line " << count << endl;
					cout << "Accessing B at position (" << j << ")" << endl;
					cout << "B have " << B.size() << " dimension\n";
					exit(1);
				}
				catch (exception &e) {
					cout << "Error: " << e.what() << endl;
					cout << "While reading line " << count << endl;
					cout << "Token: " << tokens.at(j) << endl;
					cout << "Line: " << line << endl;
					exit(1);
				}
			}
		}
		count++;
	}
}

void designation::setupModel()
{
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		//env = new GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "designation");

		varX(model);
		fo(model);

		c1(model);
		c2(model);

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

void designation::getSolution(GRBModel &model)
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

void designation::printData()
{
	cout << fileName << endl;
	cout << m << " " << n << endl;
	// print C
	cout << "C: \n";
	for (auto i : C) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
	// print A
	cout << "A: \n";
	for (auto i : A) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
	cout << "B: \n";
	for (auto j : B) {
		cout << j << " ";
	}
	cout << endl;
}

designation::~designation()
{
}
