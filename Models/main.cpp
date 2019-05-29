#include <iostream>
#include <gurobi_c++.h>
#include "pmedians.h"

using namespace std;

int main() {
	string dir = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/01/Implementation/Models/Instances/pmedianas/";
	fstream file(dir + "all.txt", ios::in);

	string line;
	while (getline(file, line)) {
		pmedians p(line);
		p.setupModel();
	}	
	return 0;
}