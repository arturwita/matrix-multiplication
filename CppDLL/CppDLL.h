#pragma once

struct Matrix {
	int width;
	int height;
	float* data;
	int _align;
};

void multiplyCpp(Matrix *, Matrix *, Matrix *, const int, const int);
