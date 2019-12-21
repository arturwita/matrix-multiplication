# matrix-multiplication

&nbsp; &nbsp; The aim of this project is to show how much faster operations are if an assembly language is used instead of a high level one. To show this I decided to compare time used to multiply two matrixes in C++ and Assembly.

**Main assumptions:**

+ usage of SIMD intructions
+ usage of threads
+ user can decide how many threads are going to be used.

## Usage

The program takes at least 2 arguments: 

1. input file containing data for left matrix
2. input file containing data for right matrix
3. number of threads which are going to be used for continous calculations*

The third argument is optional - if it is not passed the program uses the most efficient number of threads equal to the number of CPU cores.

+ ### Input file:

&nbsp; &nbsp; Contains following data separated by white signs (`\t`, `\n` or `space`):
1. matrix height
2. matrix width 
3. floating point numbers a matrix is going to be initialized with:
 
+ ### Example input files:

```
2
3
-2.0	-3.0	1.0
-1.0	4.0	0
```

```
2 3 -2.0 -3.0 1.0 -1.0 4.0 0
```

+ ### Threads number:

&nbsp; &nbsp; Accepted values are in the range of 1 to 64. If another value is passed, the program loops untill a proper value is passed.

```
Invalid threads number. Please pass a value in the range of 1 to 64: 100

Invalid threads number. Please pass a value in the range of 1 to 64: -10

Invalid threads number. Please pass a value in the range of 1 to 64: 4
```

## Results

&nbsp; &nbsp; As a result the program creates a text file which contains a resultant matrix.
Times of multiplying in assembly and C++ are showed in console. 

```
Assembly procedure: 12ms. 
C++ procedure: 40ms.
```
