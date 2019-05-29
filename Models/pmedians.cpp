#include "pmedians.h"

vector<string> split(string line, char token) {
	vector<string> tokens;

	string aux = "";
	//cout << line.length() << endl;
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) != token) {
			aux += line.at(i);
		}
		else {
			tokens.push_back(aux);
			aux = "";
		}
	}
	tokens.push_back(aux);


	//printVectorString(tokens);

	return tokens;
}

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
	
	int cont = 1;
	while (getline(file, line)) {
		vector<string> tokens = split(line, ' ');
		cout << line << endl;
		try {
			if (cont == 1) { // reading number of medians
				this->numMedians = stoi(tokens.at(0));
			}
			else if (cont == 2) { // reading number of clients
				this->numClients = stoi(tokens.at(0));
				cout << this->numClients << endl;
			}
			else if (cont >= 4 && cont < 4 + numClients) { // reading clients' coordinates
				auto x = tokens.at(3);
				x.erase(std::remove_if(x.begin(), x.end(), (int(*)(int))std::isalnum), x.end());
				cout << x << endl;
				coord c;
				c.x = stoi(tokens.at(3));
				c.y = stoi(tokens.at(4));
				this->coordsClients.push_back(c);
			}
			else if (cont == 4 + numClients + 2) { // reading clients' demands
				for (auto i : tokens) {
					this->demands.push_back(stoi(i));
				}
			}
			else if (cont == 4 + numClients + 2 + 2) { // reading number of places
				this->numPlaces= stoi(tokens.at(0));
			}
			else if (cont >= 4 + numClients + 2 + 2 + 1 && cont < 4 + numClients + 2 + 2 + 1 + numPlaces) { // reading places' coordinates
				coord c;
				c.x = stoi(tokens.at(3));
				c.y = stoi(tokens.at(4));
				this->coordsPlaces.push_back(c);
			}
			else if (cont == 4 + numClients + 2 + 2 + 1 + numPlaces) { // reading facilities capacity
				this->placesCapacity = stoi(tokens.at(0));
			}
			//else if (cont >= 4 + numClients + 2 + 2 + 1 + numPlaces + 3) { // reading distance matrix
			//}
			}
		catch (exception& e) {
			cout << e.what() << endl;
			return;
		}

		cont++;
	}

}

void pmedians::printData()
{
	cout << fileName << endl;
	cout << numMedians << " " << numClients << " " << numPlaces << endl;
	for (auto i : demands) {
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
