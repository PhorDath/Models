#include <iostream>
#include <gurobi_c++.h>
#include "pmedians.h"
#include "designation.h"

using namespace std;
const string DIR = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/01/Implementation/Models/Instances/";

int main() {
	int op;
	cin >> op;

	if (op == 0) {
		designation d(DIR + "DesignacaoGeneralizada/" + "PDG_5_100.txt");
		d.printData();
	}
	// pmedians
	else if (op == 1) {
		string dir = DIR + "pmedianas/";
		fstream file(dir + "all.txt", ios::in);

		string line;
		while (getline(file, line)) {
			pmedians p(dir + "pmedianas/" + line);
			p.setupModel();
		}
	}
	// generalized assigment
	else if (op == 2) {
		string dir = DIR + "DesignacaoGeneralizada/";
		fstream file(dir + "all.txt", ios::in);

		string line;
		while (getline(file, line)) {
			pmedians d(dir + line);
			d.setupModel();
		}
	}

		
	return 0;
}