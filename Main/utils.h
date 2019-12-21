#pragma once
#include <string>
#include <fstream>
#include "CppDLL.h"

using namespace std;
using namespace chrono;

bool isArgumentsNumberProper(const int);

bool isThreadsNumberProper(const int&);

Matrix* loadMatrix(const char*);

Matrix* transposeMatrix(Matrix*&);

void alignMatrix(Matrix *&);

void saveMatrixToFile(Matrix*&, const string&);

void printMatrix(Matrix*&);
void printAlignedMatrix(Matrix*&);

bool compareMatrixes(Matrix*&, Matrix*&);

Matrix* initializeResultantMatrix(const int, const int);

void deallocateMatrix(Matrix*&);
