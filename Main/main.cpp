// matrix-multiplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils.h"
#include "CppDLL.h"

extern "C" float multiplyAsm(Matrix*, Matrix*, Matrix*, const int, const int);

int main(int argc, char** argv) {
	if(!isArgumentsNumberProper(argc)) {							// argc should be >=3
		cout << "Invalid command line aguments number. Pass at least two text file names. ";
		system("pause");
		return 0;
	}

	const char* firstFilePath = argv[1];
	const char* secondFilePath = argv[2];

	Matrix *leftMatrix, *rightMatrix;
	leftMatrix = loadMatrix(firstFilePath);						// loading data from input files
	rightMatrix = loadMatrix(secondFilePath);

	if (!leftMatrix || !rightMatrix) {								// if at least one matrix is empty
		cout << "A file error occured while loading data to matrixes. Check if file names are correct. ";
		system("pause");
		return 0;
	}

	if (leftMatrix->width != rightMatrix->height) {		// if the necessary condition to multiply two matrixes is not met
		cout << "These matrixes can't be multiplied. The width of left matrix must equal right's height. ";
		system("pause");
		return 0;
	}

	transposeMatrix(rightMatrix);	// right matrix needs to be transposed for easier access to its data

	Matrix* resultantMatrixCpp = initializeResultantMatrix(leftMatrix->height, rightMatrix->height);
	Matrix* resultantMatrixAsm = initializeResultantMatrix(leftMatrix->height, rightMatrix->height);

	alignMatrix(leftMatrix);		// both matrixes need to be aligned to the closest multiplicity of 8
	alignMatrix(rightMatrix);		// so AVX instructions could be used in assembly DLL

	int threadsNumber;

	if (argv[3]) {
		threadsNumber = atoi(argv[3]);

		if (!isThreadsNumberProper(threadsNumber)) {
			do {
				cout << "\nInvalid threads number. Please pass a value in range of 1 to 64: ";
				cin >> threadsNumber;
			} while (!isThreadsNumberProper(threadsNumber));
		}
	}
	else {
		threadsNumber = thread::hardware_concurrency();
	}

	vector<thread> threads;
	
	if (threadsNumber <= leftMatrix->height) {
		int lineOffset = 0;
		int remainingLines = leftMatrix->height % threadsNumber;

		int *rowsPerThread = new int[threadsNumber];
		fill(rowsPerThread, rowsPerThread + threadsNumber, floor((float)leftMatrix->height / (float)threadsNumber));
		for (int i = 0; i < remainingLines; i++) { rowsPerThread[i]++; }	
		
		auto startCpp = high_resolution_clock::now();	// start of measuring time necessary for cpp to multiply matrixes
		for (int i = 0; i < threadsNumber; i++) {
			threads.push_back(thread(multiplyCpp, leftMatrix, rightMatrix, resultantMatrixCpp, lineOffset, rowsPerThread[i]));
			lineOffset += rowsPerThread[i];
		}
		for (int i = 0; i < threadsNumber; i++) {
			threads[i].join();
		}
		auto stopCpp = high_resolution_clock::now();
		saveMatrixToFile(resultantMatrixCpp, "Cpp");

		lineOffset = 0;
		threads.clear();

		auto startAsm = high_resolution_clock::now();	// start of measuring time necessary for assembly to multiply matrixes
		for (int i = 0; i < threadsNumber; i++) {
			threads.push_back(thread(multiplyAsm, leftMatrix, rightMatrix, resultantMatrixAsm, lineOffset, rowsPerThread[i]));
			lineOffset += rowsPerThread[i];
		}	
		for (int i = 0; i < threadsNumber; i++) {
			threads[i].join();
		}

		auto stopAsm = high_resolution_clock::now();
		saveMatrixToFile(resultantMatrixAsm, "Asm");

		delete[] rowsPerThread;

		cout << "Assembly procedure: " << duration_cast<milliseconds>(stopAsm - startAsm).count() << "ms.\n";
		cout << "C++ procedure: " << duration_cast<milliseconds>(stopCpp - startCpp).count() << "ms.\n";
	}
	else {
		auto startCpp = high_resolution_clock::now();	// start of measuring time necessary for cpp to multiply matrixes
		multiplyCpp(leftMatrix, rightMatrix, resultantMatrixCpp, 0, leftMatrix->height);
		auto stopCpp = high_resolution_clock::now();
		saveMatrixToFile(resultantMatrixCpp, "Cpp");

		auto startAsm = high_resolution_clock::now();	// start of measuring time necessary for assembly to multiply matrixes
		multiplyAsm(leftMatrix, rightMatrix, resultantMatrixAsm, 0, leftMatrix->height);
		auto stopAsm = high_resolution_clock::now();
		saveMatrixToFile(resultantMatrixAsm, "Asm");

		cout << "Assembly procedure: " << duration_cast<milliseconds>(stopAsm - startAsm).count() << "ms.\n";
		cout << "C++ procedure: " << duration_cast<milliseconds>(stopCpp - startCpp).count() << "ms.\n";
	}
	
	deallocateMatrix(leftMatrix);						// free allocated memory
	deallocateMatrix(rightMatrix);
	deallocateMatrix(resultantMatrixAsm);
	deallocateMatrix(resultantMatrixCpp);
	
	system("pause");
	return 0;
}
