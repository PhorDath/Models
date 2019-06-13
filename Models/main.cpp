#include <iostream>
#include "pmedians.h"
#include "designation.h"
#include "tsp.h"
#include "vrp.h"
#include "flowshop.h"
#include "jobsscheduling.h"
#include "batchScheduling.h"

using namespace std;

const string DIR = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/01/Implementation/Models/Instances/";
const string MODEL1 = "pmedianas/"; // pmedians
const string MODEL2 = "DesignacaoGeneralizada/"; // designation
const string MODEL3 = "CaixeiroViajante/"; // tsp
const string MODEL4 = "RoteamentoV/"; // vrp
const string MODEL5 = "flowshop/"; // flowshop
const string MODEL6 = "LotesMaquinasParalelas/"; // jobsscheduling
const string INST1 = "Probs_5_med_70_50.txt";
const string INST2 = "PDG_5_100.txt";
const string INST3 = "1_TSP_50.txt";
const string INST4 = "1_PRV_30.txt";
const string INST5 = "1.txt";
const string INST6 = "1_1_n20_m2_S15_r0.3.txt";

void m1_all() {
	string dir = DIR + MODEL1;
	fstream file(dir + "all.txt", ios::in);

	string line;
	while (getline(file, line)) {
		cout << "Executing model with file " << line << endl;
		pmedians p(DIR + MODEL1, line);
		p.setupModel();
	}
}

void m2_all() {
	string dir = DIR + MODEL2;
	fstream file(dir + "all.txt", ios::in);

	string line;
	while (getline(file, line)) {
			designation d(DIR + MODEL2, line);
			d.setupModel();
	}
}

void m3_all() {
	string dir = DIR + MODEL3;
	fstream file(dir + "all.txt", ios::in);

	string line;
	while (getline(file, line)) {
		tsp t(DIR + MODEL3, line);
		t.setupModel();
	}
}

void m4_all() {
	string dir = DIR + MODEL4;
	fstream file(dir + "all.txt", ios::in);

	string line;
	while (getline(file, line)) {
		vrp v(DIR + MODEL4, line);
		v.setupModel();
	}
}

void m5_all() {
	string dir = DIR + MODEL5;
	fstream file(dir + "all.txt", ios::in);

	string line;
	while (getline(file, line)) {
		flowshop f(DIR + MODEL5, line);
		f.setupModel();
	}
}

void m6_all() {
	string dir = DIR + MODEL6;
	fstream file(dir + "all.txt", ios::in);

	string line;
	while (getline(file, line)) {
		jobsScheduling j(DIR + MODEL6, line);
		j.setupModel();
	}
}

void m7_all() {
	string dir = DIR + MODEL6;
	fstream file(dir + "all.txt", ios::in);

	string line;
	while (getline(file, line)) {
		batchScheduling b(DIR + MODEL6, line);
		b.setupModel();
	}
}

int main() {
	int op{ 8 };
	//cin >> op;

	if (op == 0) {
		batchScheduling m(DIR + MODEL6, "1_1_n20_m2_S15_r0.05.txt");
		m.printData();
		m.setupModel();
		
	}
	// pmedians
	else if (op == 1) {
		m1_all();
	}
	// generalized assigment
	else if (op == 2) {
		m2_all();
	}
	// tsp
	else if (op == 3) {
		m3_all();
	}
	// vehicle routing
	else if (op == 4) {
		m4_all();
	}
	// flowshop
	else if (op == 5) {
		m5_all();
	}
	// jobs scheduling
	else if (op == 6) {
		m6_all();
	}
	// batch scheduling
	else if (op == 7) {
		m7_all();
	}
	else if (op == 8) {
		//m1_all(); // done
		//m2_all(); // done
		//m3_all(); // done
		//m4_all(); // done
		//m5_all(); // done
		//m6_all(); // done
		m7_all(); //

	}

	return 0;
}