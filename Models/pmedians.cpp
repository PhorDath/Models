#include "pmedians.h"

pmedians::pmedians(string fileName)
{
	this->fileName = fileName;
	
	fstream file;
	file.open(dir + fileName, ios::in);
	if (file.is_open() == false) {
		cout << "Error reading file " + fileName << endl;
		cout << "On the directory " + dir << endl;
	}
	
	string line;
	vector<string> tokens;
	while (getline(file, line)) {
		cout << line << endl;
	}

}

void pmedians::printData()
{
	cout << fileName << endl;
	cout << numMedians << " " << numClients << " " << numPlaces << endl;
	for (auto i : demand) {
		cout << i << " ";
	}
	cout << endl;
	for (auto i : coordsClients) {
		cout << i.x << " " << i.y << endl;
	}
	for (auto i : coordsPlaces) {
		cout << i.x << " " << i.y << endl;
	}
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
