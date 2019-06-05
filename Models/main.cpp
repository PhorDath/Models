#include <iostream>
#include <gurobi_c++.h>
#include "pmedians.h"
#include "designation.h"
#include "tsp.h"
#include "vrp.h"

using namespace std;

const string DIR = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/01/Implementation/Models/Instances/";
const string MODEL1 = "pmedianas/";
const string MODEL2 = "DesignacaoGeneralizada/";
const string MODEL3 = "CaixeiroViajante/";
const string MODEL4 = "RoteamentoV/";
const string MODEL5 = "flowshop/";
const string MODEL6 = "LotesMaquinasParalelas/";

int main() {
	int op{ 0 };
	//cin >> op;

	if (op == 0) {
		designation m(DIR + MODEL2 + "PDG_5_100.txt");
		m.printData();
		m.setupModel();
	}
	// pmedians
	else if (op == 1) {
		string dir = DIR + MODEL1;
		fstream file(dir + "all.txt", ios::in);

		string line;
		while (getline(file, line)) {
			pmedians p(dir + "pmedianas/" + line);
			p.setupModel();
		}
	}
	// generalized assigment
	else if (op == 2) {
		string dir = DIR + MODEL2;
		fstream file(dir + "all.txt", ios::in);

		string line;
		while (getline(file, line)) {
			pmedians d(dir + line);
			d.setupModel();
		}
	}
	// tsp
	else if (op == 3) {

	}
	// vehicle routing
	else if (op == 4) {
		string dir = DIR + MODEL4;
		fstream file(dir + "all.txt", ios::in);

		string line;
		while (getline(file, line)) {
			pmedians d(dir + line);
			d.setupModel();
		}
	}

		
	return 0;
}