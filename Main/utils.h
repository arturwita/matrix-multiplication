#pragma once
#include <string>
#include <fstream>

using namespace std;

bool checkArgumentsNumber(const int);

bool isCoresNumberProper(const int);

bool loadDataFromFile(fstream &, const string &, vector <vector <float>> &, int &, int &);

float** createArray(float ** &, int, int);

void deleteArray(float ** &, int);
