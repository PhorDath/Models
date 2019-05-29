#ifndef SPLIT_H
#define SPLIT_H

#include <string>
#include <vector>

using namespace std;

vector<string> split(string line, char token);
vector<string> split2(string line, char token1, char token2);
vector<string> split3(string line, char token1, char token2, char token3);

#endif