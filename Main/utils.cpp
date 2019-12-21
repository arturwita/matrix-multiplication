#include "stdafx.h"
#include "utils.h"

bool isArgumentsNumberProper(const int argc) {
	return argc >= 3;
}

bool isThreadsNumberProper(const int& coresNumber) {
	return (coresNumber <= 64) && (coresNumber >= 1);
}

Matrix* loadMatrix(const char* filePath) {
	ifstream file(filePath);
	if (file.bad()) { return nullptr; }
	
	Matrix * matrix = new Matrix;
	
	file >> matrix->height;
	file >> matrix->width;
	matrix->_align = 8 - (matrix->width % 8);

	int size = matrix->height * matrix->width;
	matrix->data = new float[size];

	for (int i = 0; i < size; i++) {
		file >> matrix->data[i];
	}

	return matrix;
}

Matrix* transposeMatrix(Matrix*& matrix) {
	float* newData = new float[matrix->height * matrix->width];
	int l = 0, k = 0;

	for (int i = 0; i < matrix->width; i++) {
		for (int j = 0; j < matrix->height; j++) {
			newData[j + k] = matrix->data[i + l];
			l += matrix->width;
		}
		l = 0;
		k += matrix->height;
	}

	swap(matrix->height, matrix->width);
	matrix->_align = 8 - (matrix->width % 8);

	delete matrix->data;
	matrix->data = newData;
	return matrix;
}

Matrix* initializeResultantMatrix(const int height, const int width) {
	Matrix * matrix = new Matrix;

	matrix->height = height;
	matrix->width = width;
	matrix->data = new float[height * width];
	matrix->_align = 0;

	fill(matrix->data, matrix->data + (height*width), 0.0);

	return matrix;
}

void alignMatrix(Matrix *& matrix) {
	if (matrix->_align == 8) { return; }

	int kTimes = matrix->width + matrix->_align;
	int l = 0;
	int m = 0;
	float* newData = new float[matrix->height * kTimes];

	for (int i = 0; i < matrix->height; i++) {
		for (int j = 0; j < matrix->width; j++) {
			newData[m + j] = matrix->data[l + j];
		}
		for (int k = matrix->width; k < kTimes; k++) {
			newData[m + k] = 0.0;
		}
		l += matrix->width;
		m += kTimes;
	}

	matrix->width = kTimes;
	delete matrix->data;
	matrix->data = newData;
}

void saveMatrixToFile(Matrix*& matrix, const string &lang) {
	ofstream file("resultantMatrix" + lang + ".txt");
	int k = 0;

	file << "Matrix height: " << matrix->height << "\n";
	file << "Matrix width: " << matrix->width << "\n";
	int jSize = matrix->width - matrix->_align;

	for (int i = 0; i < matrix->height; i++) {
		for (int j = 0; j < jSize; j++) {
			file << matrix->data[j + k] << "\t";
		}
		file << "\n";
		k += jSize;
	}
	file.close();
}

bool compareMatrixes(Matrix*& asmMatrix, Matrix*& cppMatrix) {
	int iTimes = asmMatrix->height * asmMatrix->width;
	for (int i = 0; i < iTimes; i++) {
		if (asmMatrix->data[i] != cppMatrix->data[i]) {	
			return false;	
		}
	}
	return true;
}

void deallocateMatrix(Matrix*& matrix) {
	delete matrix->data;
	delete matrix;
	return;
}