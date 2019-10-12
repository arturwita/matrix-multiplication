// matrix-multiplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils.h"
#include "vld.h"

typedef int(_fastcall *multiplyMatrixesAsm)(int, int);
typedef float** (__stdcall *multiplyMatrixesCpp)(float ** &, const int &, const int &,
	float ** &, const int &, const int &, const int &);

////////////////////////////////////////////////////////////////
void printMatrix(float ** &arr, int row, int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout << arr[i][j] << "\t";
		}
		cout << "\n";
	}
}
////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
	
	if(!checkArgumentsNumber(argc)) {
		cout << "Invalid command line aguments number. Pass file_name and CPU_cores. ";
		system("pause");
		return 0;
	}

	string firstFileName = argv[1];
	string secondFileName = argv[2];
	fstream firstFile, secondFile;

	vector <vector <float>> inputData;
	int firstMatrixRows, firstMatrixCols, secondMatrixRows, secondMatrixCols;

	float ** firstMatrix = initMatrixes(firstFile, firstFileName, inputData, firstMatrixRows, firstMatrixCols);
	float ** secondMatrix = initMatrixes(secondFile, secondFileName, inputData, secondMatrixRows, secondMatrixCols);
	
	if (firstMatrixCols != secondMatrixRows) {
		cout << "These matrixes can't be multiplied. The number of first's matrix cols must equal second's cols.\n";
		system("pause");
		return 0;
	}

	int coresNumber = atoi(argv[3]);

	if (!isCoresNumberProper(coresNumber)) {
		do {
			cout << "\nInvalid CPU cores number. Please pass a value in range of 1 to 4: ";
			cin >> coresNumber;
		} while (!isCoresNumberProper(coresNumber));
	}
	
	// HINSTANCE asmDllHandle = NULL;
	HINSTANCE cppDllHandle = NULL;
	// asmDllHandle = LoadLibrary(L"AsmDLL.dll");
	cppDllHandle = LoadLibrary(L"CppDLL.dll");

	// multiplyMatrixesAsm multiplyAsm = (multiplyMatrixesAsm)GetProcAddress(asmDllHandle, "multiplyMatrixesAsm");
	multiplyMatrixesCpp multiplyCpp = (multiplyMatrixesCpp)GetProcAddress(cppDllHandle, "multiplyMatrixesCpp");

	// auto startAsm = high_resolution_clock::now();
	// multiplyAsm(firstMatrix, secondMatrix);
	// auto stopAsm = high_resolution_clock::now();

	auto startCpp = std::chrono::high_resolution_clock::now();

	float ** resultMatrix = multiplyCpp(firstMatrix, firstMatrixRows, firstMatrixCols,
		secondMatrix, secondMatrixRows, secondMatrixCols, coresNumber);

	auto stopCpp = std::chrono::high_resolution_clock::now();

	// auto timeAsm = duration_cast<nanoseconds>(stopAsm - startAsm).count();
	auto timeCpp = duration_cast<nanoseconds>(stopCpp - startCpp).count();

	printMatrix(resultMatrix, firstMatrixRows, secondMatrixCols);

	deleteMatrix(firstMatrix, firstMatrixRows);
	deleteMatrix(secondMatrix, secondMatrixRows);
	deleteMatrix(resultMatrix, firstMatrixRows);

	// FreeLibrary(asmDllHandle);
	FreeLibrary(cppDllHandle);
	
	// cout << "Assembly procedure: " << timeAsm << "ns.\n"; 
	cout << "C++ procedure: " << timeCpp << "ns.\n"; 
	
	return 0;
}
