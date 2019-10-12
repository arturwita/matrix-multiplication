#pragma once
#include <string>
#include <fstream>

using namespace std;
using namespace chrono;

bool checkArgumentsNumber(const int);

bool isCoresNumberProper(const int);

bool loadDataFromFile(fstream &, const string &, vector <vector <float>> &);

float** createArray(const int &, const int &);

void fillMatrix(float ** &, const vector <vector <float>> &);

void deleteMatrix(float ** &, const int);

float ** initMatrixes(fstream &, const string &, vector <vector <float>> &, int &, int &);