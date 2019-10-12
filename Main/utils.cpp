#include "stdafx.h"
#include "utils.h"

bool checkArgumentsNumber(const int argc) {
	return argc == 4;
}

bool isCoresNumberProper(const int coresNumber) {
	return (coresNumber <= 4) && (coresNumber > 0);
}

bool loadDataFromFile(fstream &file, const string &fileName, vector <vector <float>> & vec) {
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

	file.close();
	return true;
}

float** createArray (const int &row, const int &col) {
	float** arr = new float*[row];

	for (int i = 0; i < row; i++) {
		arr[i] = new float[col];
	}
	
	return arr;
}

void fillMatrix(float ** &arr, const vector <vector <float>> & vec) {
	const int row = vec.size();
	const int col = vec.at(0).size();

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			arr[i][j] = vec[i][j];
		}
	}
}

void deleteMatrix(float ** &arr, const int row) {
	for (int i = 0; i < row; i++) {
		delete arr[i];
	}

	delete arr;
}

float ** initMatrixes(fstream &file, const string &fileName, vector <vector <float>> & vec, int &row, int &col) {
	loadDataFromFile(file, fileName, vec);

	row = vec.size();
	col = vec.at(0).size();

	float** matrix = createArray(row, col);
	fillMatrix(matrix, vec);

	return matrix;
}
