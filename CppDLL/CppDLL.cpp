// CppDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#define LIBRARY_EXPORTS


float ** multiplyMatrixesCpp(float ** & firstMatrix, const int & firstsRows, const int & firstsCols,
	float ** & secondMatrix, const int & secondsRows, const int & secondsCols, const int & coresNumber) {

	float** result = new float*[firstsRows];

	for (int i = 0; i < firstsRows; i++) {
		result[i] = new float[secondsCols];
	}

	for (int i = 0; i < firstsRows; i++) {
		for (int j = 0; j < secondsCols; j++) {
			result[i][j] = 0;

			for (int k = 0; k < firstsCols; k++) {
				result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}

	return result;
}