#ifndef POINTERS_H
#define POINTERS_H
#include <iostream>
using namespace std;
template<typename T>
// function template 
void PrintArray(T*arr ,int size) {
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}
// double pointer 
class Matrix {
public:
    int** CreateMatrix(int rows, int cols);
     int** FillMatrix(int** matrix, int rows, int cols);
     void PrintMatrix(int** matrix, int rows, int cols);
	 ~Matrix();
};

//shared pointer
class SharedPointer {
public:
	shared_ptr<int> ptr;
	SharedPointer(int value);
	int getValue();
	int SetCount(shared_ptr<int>&p);
};

#endif 