#include "pmedians.h"

float distance(coord a, coord b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) * 1.0);
}

void pmedians::readInstance()
{
	fstream file;
	file.open(directory + fileName, ios::in);
	if (file.is_open() == false) {
		cout << "Error reading file " + fileName << endl;
		cout << "On the directory " + directory << endl;
	}

	string line;
	int cont = 1;
	while (getline(file, line)) {
		vector<string> tokens = strf::split(line, ' ');
		try {
			if (cont == 1) { // reading number of medians
				this->numMedians = stoi(tokens.at(0));
			}
			else if (cont == 2) { // reading number of clients
				this->numClients = stoi(tokens.at(0));
				cout << this->numClients << endl;
			}
			else if (cont >= 4 && cont < 4 + numClients) { // reading clients' coordinates
				// remove all non digit characters from string
				// this is needed due to the way i'm reading the file, with split (python style). 
				// this prevent the stoi to try to convert strings like "(12," into integers
				// for more reference on lamda expressions check the link http://www.drdobbs.com/cpp/lambdas-in-c11/240168241
				auto x = tokens.at(3);
				x.erase(std::remove_if(x.begin(), x.end(), [](char c) {return !isdigit(c); }), x.end()); // lambda expression FTW		
				auto y = tokens.at(4);
				y.erase(std::remove_if(y.begin(), y.end(), [](char c) {return !isdigit(c); }), y.end());  // more lamba

				// storing coordinates
				coord c;
				c.x = stoi(x);
				c.y = stoi(y);
				this->coordsClients.push_back(c);
			}
			else if (cont == 4 + numClients + 2) { // reading clients' demands
				for (auto tk : tokens) {
					tk.erase(remove_if(tk.begin(), tk.end(), [](char c) {return !isdigit(c); }), tk.end()); // removing any non digit character from the token

					// storing demands
					this->demands.push_back(stoi(tk));
				}
			}
			else if (cont == 4 + numClients + 2 + 2) { // reading number of places
				this->numPlaces = stoi(tokens.at(0));
			}
			else if (cont >= 4 + numClients + 2 + 2 + 1 && cont < 4 + numClients + 2 + 2 + 1 + numPlaces) { // reading places' coordinates
				// just like above, removing non digits chars from the token
				auto x = tokens.at(3);
				x.erase(std::remove_if(x.begin(), x.end(), [](char c) {return !isdigit(c); }), x.end()); // lambda expression FTW		
				auto y = tokens.at(4);
				y.erase(std::remove_if(y.begin(), y.end(), [](char c) {return !isdigit(c); }), y.end());  // more lamba

				// storing places' coordinates
				coord c;
				c.x = stoi(x);
				c.y = stoi(y);
				this->coordsPlaces.push_back(c);
			}
			else if (cont == 4 + numClients + 2 + 2 + 1 + numPlaces) { // reading facilities capacity
				this->placesCapacity = stoi(tokens.at(tokens.size() - 1));
			}
			// not reading the distance matrix
			//else if (cont >= 4 + numClients + 2 + 2 + 1 + numPlaces + 3) { // reading distance matrix
			//}
		}
		catch (exception& e) {
			cout << e.what() << endl;
			return;
		}
		cont++;
	}
	// distance matrix calculus
	// resize matrix
	distanceMatrix.resize(numClients);
	for (int i = 0; i < numClients; i++) {
		distanceMatrix.at(i).resize(numPlaces, 0);
	}
	// calculus
	for (int i = 0; i < numClients; i++) {
		for (int j = 0; j < numPlaces; j++) {
			distanceMatrix.at(i).at(j) = int(distance(coordsClients.at(i), coordsPlaces.at(j)));
		}
	}

	n = numClients;
	m = numPlaces;
}

void pmedians::varX(GRBModel &model)
{
	// resize x decision variable
	x.resize(n);
	for (int i = 0; i < n; i++) {
		x.at(i).resize(m);
	}

	// add var to model
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			x.at(i).at(j) = model.addVar(0, 1, 1, GRB_BINARY, "x(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

void pmedians::varY(GRBModel &model)
{
	// resize y
	y.resize(m);
	// add y to model
	for (int j = 0; j < m; j++) {
		y.at(j) = model.addVar(0, 1, 1, GRB_BINARY, "y(" + to_string(j) + ")");
	}
	model.update();
}

void pmedians::fo(GRBModel &model)
{
	GRBLinExpr fo = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			fo += distanceMatrix.at(i).at(j) * x.at(i).at(j);
		}
	}
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void pmedians::c1(GRBModel &model)
{
	for (int i = 0; i < n; i++) {
		GRBLinExpr c1 = 0;
		for (int j = 0; j < m; j++) {
			c1 += x.at(i).at(j);
		}
		model.addConstr(c1 == 1, "c1(" + to_string(i) + ")");
	}
	model.update();
}

void pmedians::c2(GRBModel &model)
{
	GRBLinExpr c2 = 0;
	for (int j = 0; j < m; j++) {
		c2 += y.at(j);
	}
	model.addConstr(c2 == numPlaces, "c2");
}

void pmedians::c3(GRBModel &model)
{
	for (int j = 0; j < m; j++) {
		GRBLinExpr c3 = 0;
		for (int i = 0; i < n; i++) {
			c3 += demands.at(i) * x.at(i).at(j);
		}
		model.addConstr(c3 <= placesCapacity, "c3(" + to_string(j) + ")");
	}
	model.update();
}

void pmedians::c4(GRBModel &model)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			GRBLinExpr c4 = 0;
			model.addConstr(x.at(i).at(j) <= y.at(j), "c4(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

pmedians::pmedians(string fileName)
{
	this->fileName = fileName;
	readInstance();
}

pmedians::pmedians(string directory, string fileName)
{
	this->fileName = fileName;
	this->directory = directory;
	readInstance();
}

void pmedians::setupModel()
{
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		//env = new GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "pmedians_" + fileName);

		varX(model);
		varY(model);
		fo(model);

		c1(model);
		c2(model);
		c3(model);
		c4(model);

		model.write("teste.lp");

		model.getEnv().set(GRB_DoubleParam_TimeLimit, 600);

		model.optimize();

		model.write("teste.sol");
		utilities::processSolution(model);
		getSolution(model);

		//getSolution(model);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void pmedians::getSolution(GRBModel &model)
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
	output << "x: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			auto temp = model.getVarByName("x(" + to_string(i) + "," + to_string(j) + ")").get(GRB_DoubleAttr_X);
			if (temp == -0) { // i dont know why some values are beeing -0
				temp *= -1;
			}
			output << temp << " ";
		}
		output << endl;
	}
	output << "y: " << endl;
	for (int j = 0; j < m; j++) {
		auto temp = model.getVarByName("y(" + to_string(j) + ")").get(GRB_DoubleAttr_X);
		if (temp == -0) { // i dont know why some values are beeing -0
			temp *= -1;
		}
		output << temp << " ";
	}
	output.close();
}

void pmedians::printData()
{
	cout << fileName << endl;
	cout << numMedians << " " << numClients << " " << numPlaces << " " << placesCapacity << endl;
	for (auto i : demands) {
		cout << i << " ";
	}
	cout << endl;
	cout << "client: " << endl;
	for (auto i : coordsClients) {
		cout << i.x << " " << i.y << endl;
	}
	cout << "places: " << endl;
	for (auto i : coordsPlaces) {
		cout << i.x << " " << i.y << endl;
	}
	cout << "matrix: " << endl;
	for (auto i : distanceMatrix) {
		for (auto j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
}

pmedians::~pmedians()
{

}
