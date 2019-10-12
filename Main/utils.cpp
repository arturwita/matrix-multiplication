#include "stdafx.h"
#include "utils.h"

bool checkArgumentsNumber(const int argc) {
	return argc == 4;
}

bool isCoresNumberProper(const int coresNumber) {
	return (coresNumber <= 4) && (coresNumber > 0);
}

bool loadDataFromFile(fstream &file, const string &fileName, vector <vector <float>> & vec, int & row, int & col) {
	file.open(fileName, ios_base::in);

	if (!file.good()) {
		cout << "An error occured while opening first file. Check whether it's name is proper." << endl;
		cout << "Current file name: " << fileName << endl;
		return false;
	}

	vec.clear();
	string line, tmp;

	while (getline(file, line)) {
		vector <float> tmpVec;
		istringstream iss(line);

		while (iss >> tmp) {
			tmpVec.push_back(atof(tmp.c_str()));
		}

		vec.push_back(tmpVec);
	}
	row = vec.size();
	col = vec.at(0).size();

	file.close();
	return true;
}

float** createArray (float ** &arr, int row, int col) {
	arr = new float*[row];

	for (int i = 0; i < row; i++) {
		arr[i] = new float[col];
	}
	
	return arr;
}

void deleteArray (float ** &arr, int row) {
	for (int i = 0; i < row; i++) {
		delete arr[i];
	}
	delete arr;
}

void fillMatrix(float ** &arr, int col) {


}