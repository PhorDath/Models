#include "jobsScheduling.h"

jobsScheduling::jobsScheduling(string fileName)
{
	this->directory = "";
	this->fileName = fileName;
	readInstance();
}

jobsScheduling::jobsScheduling(string directory, string fileName)
{
	this->directory = directory;
	this->fileName = fileName;
	readInstance();
}

void jobsScheduling::setupModel()
{
	cout << "---------------Running " << this->fileName << endl << endl << endl;
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		//env = new GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "jobsScheduling_" + fileName);

		varX(model);
		varT(model);
		varC(model);
		fo(model);
		c1(model);
		c2(model);
		c3(model);
		c4(model);
		c5(model);
		c6(model);

		model.write("teste.lp");

		model.getEnv().set(GRB_DoubleParam_TimeLimit, TMAX);//TMAX);

		model.optimize();

		getSolution(model);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void jobsScheduling::getSolutionFull(GRBModel &model)
{
	string dir = directory + "output/";
	string fn = model.get(GRB_StringAttr_ModelName);
	//model.write(directory + fn + ".sol");
	fstream output(dir + fn, ios::out | ios::trunc);
	if (output.is_open() == false) {
		cout << "Error opening output file " << fn << endl;
		cout << "On directory " << dir << endl;
		exit(1);
	}
	// writing variables in a format for easy ploting
	output << "# x " << x.size() << " " << x.at(0).size() << " " << x.at(0).at(0).size() << endl;
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < x.at(i).size(); j++) {
			for (int k = 0; k < x.at(i).at(j).size(); k++) {
				auto temp = model.getVarByName("x(" + to_string(i) + "," + to_string(j) + "," + to_string(k) + ")").get(GRB_DoubleAttr_X);
				if (temp == -0) { // i dont know why some values are beeing -0
					temp *= -1;
				}
				output << temp << " ";
			}
			output << endl;
		}
		output << endl;
	}
	output << "# c " << c.size() << " " << c.at(0).size() << endl;
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < c.at(i).size(); j++) {
			auto temp = model.getVarByName("c(" + to_string(i) + "," + to_string(j) + ")").get(GRB_DoubleAttr_X);
			if (temp == -0) { // i dont know why some values are beeing -0
				temp *= -1;
			}
			output << temp << " ";
		}
		output << endl;
	}
	output << "# t " << t.size() << endl;
	for (int i = 0; i < t.size(); i++) {
		auto temp = model.getVarByName("t(" + to_string(i) + ")").get(GRB_DoubleAttr_X);
		if (temp == -0) { // i dont know why some values are beeing -0
			temp *= -1;
		}
		output << temp << " ";
	}
	output << endl;
	output.close();
}

void jobsScheduling::getSolution(GRBModel & model)
{
	string dir = directory + "output/";
	string fn = model.get(GRB_StringAttr_ModelName);
	//model.write(directory + fn + ".sol");
	fstream output(dir + fn, ios::out | ios::trunc);
	if (output.is_open() == false) {
		cout << "Error opening output file " << fn << endl;
		cout << "On directory " << dir << endl;
		exit(1);
	}
	// write fo, gap and execution time
	output << model.get(GRB_DoubleAttr_ObjVal) << " " << model.get(GRB_DoubleAttr_MIPGap) << " " << model.get(GRB_DoubleAttr_Runtime) << endl;
	// writing variables in a format for easy ploting
	for (int k = 0; k < numMachines; k++) { // for each machine k
		output << k << ": ";
		for (int i = 0; i < x.size(); i++) {
			for (int j = 0; j < x.at(i).size(); j++) {
				auto temp = model.getVarByName("x(" + to_string(i) + "," + to_string(j) + "," + to_string(k) + ")").get(GRB_DoubleAttr_X);
				if (temp == 1) { // i dont know why some values are beeing -0
					output << i << " ";
				}
			}
		}
		output << endl;
	}
	output << endl;
	output.close();
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

void jobsScheduling::readInstance()
{
	fstream file(directory + fileName, ios::in);
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
	// bigM calculus
	// the big M is being defined as the sum of pij, i.e, all the completitions times for every task and machine
	int aux = 0;
	for (int i = 0; i < p.size(); i++) {
		for (int j = 0; j < p.at(i).size(); j++) {
			aux += p.at(i).at(j);
		}
	}
	bigM = aux;
	file.close();
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
	for (int i = 0; i < numJobs; i++) {
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
	for (int j = 1; j < numJobs; j++) { // for (int j = 0; j < numJobs; j++) {
		GRBLinExpr c1{ 0 };
		for (int k = 0; k < numMachines; k++) { // for each machine
			for (int i = 0; i < numJobs; i++) { // for each job
				c1 += x.at(i).at(j).at(k);
			}
		}
		model.addConstr(c1 == 1, "c(" + to_string(j) + ")");
	}
	model.update();
}

void jobsScheduling::c2(GRBModel &model)
{
	for (int k = 0; k < numMachines; k++) { // for each machine
		GRBLinExpr c2{ 0 };
		for (int j = 1; j < numJobs; j++) { // for each job // for (int j = 0; j < numJobs; j++) {
			c2 += x.at(0).at(j).at(k);
		}
		model.addConstr(c2 <= 1, "c2(" + to_string(k) + ")");
	}
	model.update();
}

void jobsScheduling::c3(GRBModel &model)
{
	for (int h = 1; h < numJobs; h++) {	// for (int h = 0; h < numJobs; h++) {	
		for (int k = 0; k < numMachines; k++) { // for each machine
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
	for (int k = 0; k < numMachines; k++) { // for each machine
		model.addConstr(c.at(0).at(k) == 0, "c4(" + to_string(k) + ")"); // the fake activite ends on time zero
	}
	model.update();
}

void jobsScheduling::c5(GRBModel &model)
{
	for (int i = 0; i < numJobs; i++) { // for each job
		for (int j = 1; j < numJobs; j++) { // for each job but the zero
			for (int k = 0; k < numMachines; k++) { // for (int k = 1; k < numMachines; k++) {
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
	for (int i = 1; i < numJobs; i++) { // for each job but the fake initial one
		for (int k = 0; k < numMachines; k++) { // for (int k = 1; k < numMachines; k++) {
			GRBLinExpr c61{ 0 };
			GRBLinExpr c62{ 0 };
			c61 = t.at(i);
			c62 = c.at(i).at(k) - r.at(i);
			model.addConstr(c61 >= c62, "c6(" + to_string(i) + "," + to_string(k) + ")");
		}
	}
	model.update();
}
