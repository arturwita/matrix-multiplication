// CppDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CppDLL.h"

void multiplyCpp(Matrix *leftMatrix, Matrix *rightMatrix, Matrix * resultantMatrix, const int lineOffset, const int rows) {
	int l, m, n;
	l = m = n = 0;

	const int rightOffset = lineOffset * rightMatrix->height;
	const int leftOffset = lineOffset * leftMatrix->width;

	for (int k = 0; k < rows; k++) {
		for (int i = 0; i < rightMatrix->height; i++) {
			for (int j = 0; j < rightMatrix->width; j++) {
				resultantMatrix->data[rightOffset + l + i] +=
					leftMatrix->data[leftOffset + m + j] * rightMatrix->data[n + j];
			}
			n += rightMatrix->width;
		}
		n = 0;
		l += rightMatrix->height;
		m += leftMatrix->width;
	}
}
