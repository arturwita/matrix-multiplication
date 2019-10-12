# matrix-multiplication

&nbsp; &nbsp; The aim of this project is to show how much faster operations are if an assembly language is used instead of a high level one. To show this I decided to compare time used to multiply 2 matrixes in C++ and Assembly.

**Main assumptions:**

+ usage of vectors
+ usage of threads
+ user can decide how many CPU cores are going to be used  

## Usage

The program takes 3 arguments: 

1. input file containing first matrix
2. input file containing second matrix
3. number of CPU cores which are going to be used for continous calculations

+ ### Input file:

&nbsp; &nbsp; Contains floating point numbers separated by `\t` or `space`:
 
+ ### Example input file:

```
-2.0	-3.0	1.0
-1.0	4.0	0
```
+ ### CPU cores number:

&nbsp; &nbsp; Accepted values are in the range of 1 to 4. If another value is passed, the program loops untill a proper value is passed.

```
Invalid CPU cores number. Please pass a value in range of 1 to 4: 10

Invalid CPU cores number. Please pass a value in range of 1 to 4: -10

Invalid CPU cores number. Please pass a value in range of 1 to 4: 0
```

## Results

&nbsp; &nbsp; As a result the program creates a text file which contains a resutant matrix.
Times of multiplying in assembly and C++ are showed in console. 

```
Assembly procedure: 1280ms. 
C++ procedure: 1440ms.
```