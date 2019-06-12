#include "tsp.h"



void tsp::readInstance()
{
	fstream file;
	file.open(directory + fileName, ios::in);
	if (file.is_open() == false) {
		cout << "Error reading file " + fileName << endl;
		cout << "On the directory " + directory << endl;
	}

	string line;
	int cont{ 1 };
	while (getline(file, line)) {
		vector<string> tokens = strf::split(line, ' ');
		try {
			if (cont == 3) { // reading number of nodes
				this->numNodes = stoi(tokens.at(tokens.size() - 1));
			}
			else if (cont >= 6 && cont < 6 + numNodes) { // reading number of clients
				utilities::coord c;
				c.x = strf::rndac(tokens.at(0));
				c.y = strf::rndac(tokens.at(1));
				coordinates.push_back(c);
			}
		}
		catch (exception& e) {
			cout << "Error reading line " << cont << endl;
			cout << e.what() << endl;
			return;
		}
		cont++;
	}
	if (coordinates.size() != numNodes) {
		cout << "Error: ";
		cout << "coordinates vector size (" << coordinates.size() << ") doesnt match with number of nodes (" << numNodes << ")";
		exit(1);
	}
	// distance matrix calculus
	// resize matrix
	dMatrix.resize(numNodes);
	for (int i = 0; i < numNodes; i++) {
		dMatrix.at(i).resize(numNodes, 0);
	}
	// calculus
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			dMatrix.at(i).at(j) = int(utilities::distance(coordinates.at(i), coordinates.at(j)));
		}
	}
	file.close();
}

void tsp::varX(GRBModel & model)
{
	// resize x
	x.resize(numNodes);
	for (int i = 0; i < numNodes; i++) {
		x.at(i).resize(numNodes);
	}
	// add x to model
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			x.at(i).at(j) = model.addVar(0, 1, 1, GRB_BINARY, "x(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
}

void tsp::varF(GRBModel & model)
{
	// resize f
	f.resize(numNodes);
	for (int i = 0; i < numNodes; i++) {
		f.at(i).resize(numNodes);
	}
	// add f to model
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			f.at(i).at(j) = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "f(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
}

void tsp::fo(GRBModel & model)
{
	GRBLinExpr fo{ 0 };
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			fo += dMatrix.at(i).at(j) * x.at(i).at(j);
		}
	}
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

// from each city arrives only one arc
void tsp::c1(GRBModel & model)
{
	for (int k = 0; k < numNodes; k++) {
		GRBLinExpr c1{ 0 };
		for (int i = 0; i < numNodes; i++) {
			c1 += x.at(i).at(k);
		}
		model.addConstr(c1 == 1, "c1(" + to_string(k) + ")");
	}
	model.update();
}

// from each city leaves only one arc
void tsp::c2(GRBModel & model)
{
	for (int k = 0; k < numNodes; k++) {
		GRBLinExpr c2{ 0 };
		for (int j = 0; j < numNodes; j++) {
			c2 += x.at(k).at(j);
		}
		model.addConstr(c2 == 1, "c2(" + to_string(k) + ")");
	}
	model.update();
}

// flow conservation
void tsp::c3(GRBModel & model)
{
	for (int k = 1; k < numNodes; k++) {
		GRBLinExpr c31{ 0 };
		GRBLinExpr c32 = 0;
		for (int i = 0; i < numNodes; i++) {
			c31 += f.at(i).at(k);
		}
		for (int j = 0; j < numNodes; j++) {
			c32 += f.at(k).at(j);
		}
		model.addConstr(c31 - c32 == 1, "c3(" + to_string(k) + ")");
	}
	model.update();
}

void tsp::c4(GRBModel & model)
{
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			if (i != j) {
				model.addConstr(f.at(i).at(j) <= (numNodes - 1)*x.at(i).at(j), "c4(" + to_string(i) + "," + to_string(j) + ")");
			}
		}
	}
	model.update();
}

tsp::tsp(string fileName)
{
	this->directory = "";
	this->fileName = fileName;
	readInstance();
}

tsp::tsp(string directory, string fileName)
{
	this->directory = directory;
	this->fileName = fileName;
	readInstance();
}

void tsp::setupModel()
{
	cout << "---------------Running " << this->fileName << endl << endl << endl;
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		//env = new GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "tsp_" + fileName);

		pimModel(model);

		model.write("teste.lp");

		model.getEnv().set(GRB_DoubleParam_TimeLimit, TMAX);

		model.optimize();

		model.write("teste.sol");
		utilities::processSolution(model);
		getSolution(model);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void tsp::pimModel(GRBModel & model)
{
	varX(model);
	varF(model);
	fo(model);
	c1(model);
	c2(model);
	c3(model);
	c4(model);
}

void tsp::checkPermutation()
{
	auto temp = permutation;
	sort(temp.begin(), temp.end());
}

void tsp::getSolutionFull(GRBModel & model)
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
	output << "# x " << numNodes << " " << numNodes << endl;
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			auto temp = model.getVarByName("x(" + to_string(i) + "," + to_string(j) + ")").get(GRB_DoubleAttr_X);
			if (temp == -0) { // i dont know why some values are beeing -0
				temp *= -1;
			}
			output << temp << " ";
		}
		output << endl;
	}
	output << "# f " << numNodes << " " << numNodes << endl;
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			auto temp = model.getVarByName("f(" + to_string(i) + "," + to_string(j) + ")").get(GRB_DoubleAttr_X);
			if (temp == -0) { // i dont know why some values are beeing -0
				temp *= -1;
			}
			output << temp << " ";
		}
		output << endl;
	}
	output.close();
}

void tsp::getSolution(GRBModel & model)
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
	vector<int> aux;
	aux.resize(numNodes, 0);
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			auto temp = model.getVarByName("x(" + to_string(i) + "," + to_string(j) + ")").get(GRB_DoubleAttr_X);
			if (temp == 1) { 
				output << i << " " << j << " " << dMatrix.at(i).at(j) << endl;
				aux.at(i) = j;
			}
		}
	}
	// write input data
	output << "# data" << endl;
	output << numNodes << endl;
	for (int i = 0; i < coordinates.size(); i++) {
		output << coordinates.at(i).x << " " << coordinates.at(i).y << endl;
	}
	output.close();
	//
	//permutation.resize(numNodes);
	permutation.push_back(0);
	//cout << 0 << " ";
	int i = 0;
	int next = 0;
	while(i < numNodes - 1){		
		//cout << aux.at(next) << " ";
		permutation.push_back(aux.at(next));
		next = aux.at(next);
		i++;
	}
}

void tsp::printData()
{
	cout << numNodes << endl;
	for (auto i : coordinates) {
		cout << i.x << " " << i.y << endl;
	}
	for (auto i : dMatrix) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
}


tsp::~tsp()
{
}
