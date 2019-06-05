#include "strf.h"

strf::strf(){}

vector<string> strf::split(string line, char token) {
	vector<string> tokens;
	string aux = "";
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) != token) {
			aux += line.at(i);
		}
		else if (aux != ""){			
			tokens.push_back(aux);
			aux = "";			
		}
		else {
			aux = "";
		}
	}
	if (aux != " " || aux != "") {
		tokens.push_back(aux);
	}
		
	return tokens;
}

vector<string> strf::split2(string line, char token1, char token2) {
	vector<string> tokens;
	string aux = "";
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) != token1 && line.at(i) != token2) {
			aux += line.at(i);
		}
		else if (aux != to_string(token1) && aux != to_string(token2) && aux != "") {
			tokens.push_back(aux);
			aux = "";
		}
	}
	tokens.push_back(aux);
	return tokens;
}

vector<string> strf::split3(string line, char token1, char token2, char token3) {
	vector<string> tokens;
	string aux = "";
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) != token1 && line.at(i) != token2 && line.at(i) != token3) {
			aux += line.at(i);
		}
		else if (aux != to_string(token1) && aux != to_string(token2) && aux != to_string(token3) && aux != "") {
			tokens.push_back(aux);
			aux = "";
		}
	}
	tokens.push_back(aux);
	return tokens;
}

// remove any non digit character from a given string
string strf::rnd(string x)
{
	x.erase(std::remove_if(x.begin(), x.end(), [](char c) {return !isdigit(c); }), x.end());
	return x;
}

// remove any non digit character from a given string and covert to float
float strf::rndac(string x)
{
	try {
		string aux = rnd(x);
		if (aux != "") {
			return stoi(aux);
		}		
	}
	catch (exception e) {}
}

strf::~strf(){}