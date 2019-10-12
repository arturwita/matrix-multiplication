#pragma once

#ifdef LIBRARY_EXPORTS
#    define LIBRARY_API __declspec(dllexport)
#else
#    define LIBRARY_API __declspec(dllimport)
#endif

LIBRARY_API float ** multiplyMatrixesCpp(float ** & firstMatrix, const int & firstsRows, const int & firstsCols,
	float ** & secondMatrix, const int & secondsRows, const int & secondsCols, const int & coresNumber);