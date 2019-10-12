// page-rank-calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils.h"
#include "vld.h"

typedef int(_fastcall *MultiplyMatrixesAsm)(int, int);

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
	int row, col;

	loadDataFromFile(firstFile, firstFileName, inputData, row, col);

	//float ** matrixOne;
	//createArray(matrixOne, row, col);

	int coresNumber = atoi(argv[3]);

	if (!isCoresNumberProper(coresNumber)) {
		do {
			cout << "\nInvalid CPU cores number. Please pass a value in range of 1 to 4: ";
			cin >> coresNumber;
		} while (!isCoresNumberProper(coresNumber));
	}

	// Mnożenie macierzy jest możliwe jeśli:
	// liczba kolumn 1 macierzy jest równa liczbie wierszy 2 macierzy.
	

	/*
	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"AsmDLL.dll");

	MyProc1 procedura = (MyProc1)GetProcAddress(dllHandle, "MultiplyMatrixesAsm");
	
	int x = 5, y = 7;
	int retVal = procedura(x, y);

	FreeLibrary(dllHandle);
	*/
	return 0;
}

