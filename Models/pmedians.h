#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
//#include "split.h"
#include "useful.h"

using namespace std;

class pmedians
{
private:
	string dir = "D:/Victor/Pos-Graduacao/UFV/2 periodo/INF682/Trabalhos/01/Instances/pmedianas/";
	string fileName;
	int numMedians;
	int numClients;
	int numPlaces;
	int placesCapacity;
	vector<int> demands;
	vector<coord> coordsClients;
	vector<coord> coordsPlaces;
	vector<vector<float>> distanceMatrix;

public:
	pmedians(string);
	void printData();
	~pmedians();
};

