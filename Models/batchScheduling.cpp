#include "batchScheduling.h"

batchScheduling::batchScheduling(string fileName)
{
	this->directory = "";
	this->fileName = fileName;
	readInstance();
}

batchScheduling::batchScheduling(string directory, string fileName)
{
	this->directory = directory;
	this->fileName = fileName;
	readInstance();
}

void batchScheduling::setupModel()
{
	cout << "---------------Running " << this->fileName << endl << endl << endl;
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		//env = new GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "BatchScheduling_" + fileName);

		varX(model); // xjbk = 1 if job is designated to batch b on machine k
		varSB(model); // sbk = begining time of batch b on machine k
		varPB(model); // pbk = processing time of batch b on machine k
		varC(model); // makespan 
		fo(model); // minimize the makespan
		c1(model); // each job j must be on a unique batch
		c2(model); // the sum of the jobs in the batch cant be higher then the machine capacity
		c3(model); // a batch can only begging after all his jobs are available
		c4(model); // a batch batch should start after the previous batch ends
		c5(model); // the processing time of a batch is the longest of its tasks
		c6(model); // determines is the longest completion time on each machine


		model.write("teste.lp");

		model.getEnv().set(GRB_DoubleParam_TimeLimit, TMAX);//TMAX);

		model.optimize();

		utilities::processSolution(model);
		getSolution(model);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

void batchScheduling::getSolutionFull(GRBModel &model)
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
	output << model.get(GRB_DoubleAttr_ObjVal) << " " << model.get(GRB_DoubleAttr_MIPGap) * 100 << " " << model.get(GRB_DoubleAttr_Runtime) << endl;
	output << "# c: " << model.getVarByName("makespan").get(GRB_DoubleAttr_X) << endl;;
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
	output << "# sb " << sb.size() << " " << sb.at(0).size() << endl;
	for (int i = 0; i < sb.size(); i++) {
		for (int j = 0; j < sb.at(i).size(); j++) {
			auto temp = model.getVarByName("sb(" + to_string(i) + "," + to_string(j) + ")").get(GRB_DoubleAttr_X);
			if (temp == -0) { // i dont know why some values are beeing -0
				temp *= -1;
			}
			output << temp << " ";
		}
		output << endl;
	}
	output << "# pb " << pb.size() << " " << pb.at(0).size() << endl;
	for (int i = 0; i < pb.size(); i++) {
		for (int j = 0; j < pb.at(i).size(); j++) {
			auto temp = model.getVarByName("pb(" + to_string(i) + "," + to_string(j) + ")").get(GRB_DoubleAttr_X);
			if (temp == -0) { // i dont know why some values are beeing -0
				temp *= -1;
			}
			output << temp << " ";
		}
		output << endl;
	}
	output << endl;
	output.close();
}

void batchScheduling::getSolution(GRBModel & model)
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
	output << model.get(GRB_DoubleAttr_ObjVal) << " " << model.get(GRB_DoubleAttr_MIPGap) * 100 << " " << model.get(GRB_DoubleAttr_Runtime) << endl;
	// writing variables in a format for easy ploting
	for (int k = 0; k < numMachines; k++) { // for each machine k
		output << k << ": ";
		vector<int> batch;
		for (int b = 0; b < n; b++) { // for each batch slot			
			for (int j = 0; j < n; j++) { // get jobs
				auto temp = model.getVarByName("x(" + to_string(j) + "," + to_string(b) + "," + to_string(k) + ")").get(GRB_DoubleAttr_X);
				if (temp == 1) { // 
					output << j << " ";
					batch.push_back(j);
				}
			}
			output << "|";
		}
		output << endl;
	}
	output << endl;
	output.close();
}

void batchScheduling::printData()
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

batchScheduling::~batchScheduling()
{
}


void batchScheduling::readInstance()
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
	file.close();
	n = numJobs;
	m = numMachines;
}

void batchScheduling::varX(GRBModel &model)
{
	// resize x
	x.resize(n);
	for (int i = 0; i < n; i++) {
		x.at(i).resize(n);
		for (int j = 0; j < n; j++) {
			x.at(i).at(j).resize(m);
		}
	}
	// add var x to model
	for (int j = 0; j < n; j++) {
		for (int b = 0; b < n; b++) {
			for (int k = 0; k < m; k++) {
				x.at(j).at(b).at(k) = model.addVar(0, 1, 1, GRB_BINARY, "x(" + to_string(j) + "," + to_string(b) + "," + to_string(k) + ")");
			}
		}
	}
	model.update();
}

void batchScheduling::varSB(GRBModel & model)
{
	sb.resize(n);
	for (int b = 0; b < n; b++) {
		sb.at(b).resize(m);
	}
	for (int b = 0; b < n; b++) {
		for (int k = 0; k < m; k++) {
			try {
				sb.at(b).at(k) = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "sb(" + to_string(b) + "," + to_string(k) + ")");
			}
			catch (exception e) {
				cout << e.what() << endl;
				exit(1);
			}
		}
	}
	model.update();
}

void batchScheduling::varPB(GRBModel & model)
{
	pb.resize(n);
	for (int b = 0; b < n; b++) {
		pb.at(b).resize(m);
	}
	for (int b = 0; b < n; b++) {
		for (int k = 0; k < m; k++) {
			pb.at(b).at(k) = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "pb(" + to_string(b) + "," + to_string(k) + ")");
		}
	}
	model.update();
}

void batchScheduling::varC(GRBModel & model)
{
	c = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "makespan");
	model.update();
}

void batchScheduling::fo(GRBModel & model)
{
	GRBLinExpr fo{ 0 };
	fo = c;
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void batchScheduling::c1(GRBModel & model)
{
	for (int j = 0; j < n; j++) {
		GRBLinExpr c1{ 0 };
		for (int b = 0; b < n; b++) {
			for (int k = 0; k < m; k++) {
				c1 += x.at(j).at(b).at(k);
			}
		}
		model.addConstr(c1 == 1, "c1(" + to_string(j) + ")");
	}
	model.update();
}

void batchScheduling::c2(GRBModel & model)
{
	for (int b = 0; b < n; b++) {		
		for (int k = 0; k < m; k++) {
			GRBLinExpr c2{ 0 };
			for (int j = 0; j < n; j++) {
				c2 += s.at(j) * x.at(j).at(b).at(k);
			}
			model.addConstr(c2 <= Q.at(k), "c2(" + to_string(b) + "," + to_string(k) + ")");
		}
	}
	model.update();
}

void batchScheduling::c3(GRBModel & model)
{
	for (int j = 0; j < n; j++) {
		for (int b = 0; b < n; b++) {
			for (int k = 0; k < m; k++) {
				GRBLinExpr c31{ 0 };
				GRBLinExpr c32{ 0 };
				c31 += sb.at(b).at(k);
				c32 += r.at(j) * x.at(j).at(b).at(k);
				model.addConstr(c31 >= c32, "c3(" + to_string(j) + "," + to_string(b) + "," + to_string(k) + ")");
			}
		}
	}
	model.update();
}

void batchScheduling::c4(GRBModel & model)
{
	for (int b = 0; b < n-1; b++) {
		for (int k = 0; k < m; k++) {
			GRBLinExpr c41{ 0 };
			GRBLinExpr c42{ 0 };
			c41 += sb.at(b+1).at(k);
			c42 += sb.at(b).at(k) + pb.at(b).at(k);
			model.addConstr(c41 >= c42, "c4(" + to_string(b) + "," + to_string(k) + ")");
		}
	}
	model.update();
}

void batchScheduling::c5(GRBModel & model)
{
	for (int j = 0; j < n; j++) {
		for (int b = 0; b < n; b++) {
			for (int k = 0; k < m; k++) {
				GRBLinExpr c51{ 0 };
				GRBLinExpr c52{ 0 };
				c51 += pb.at(b).at(k);
				c52 += p.at(j).at(k) * x.at(j).at(b).at(k);
				model.addConstr(c51 >= c52, "c5(" + to_string(j) + "," + to_string(b) + "," + to_string(k) + ")");
			}
		}
	}
	model.update();
}

void batchScheduling::c6(GRBModel & model)
{
	for (int b = 0; b < n; b++) {
		for (int k = 0; k < m; k++) {
			GRBLinExpr c61{ 0 };
			GRBLinExpr c62{ 0 };
			c61 += c;
			c62 += sb.at(b).at(k) + pb.at(b).at(k);
			model.addConstr(c61 >= c62, "c6(" + to_string(b) + "," + to_string(k) + ")");
		}
	}
	model.update();
}

