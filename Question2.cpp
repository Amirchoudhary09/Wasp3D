#include<iostream>
#include"pointers.h"
using namespace std;

//create the matrix
int** Matrix:: CreateMatrix(int rows, int cols) {
	int** matrix = new int* [rows];
	for (int i = 0; i < rows; i++) {
		matrix[i] = new int[cols];
	}
	return matrix;
}
//fill the matrix 
int** Matrix::FillMatrix(int** matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			matrix[i][j] = i + j;
		}
	}
	return matrix;
}
//print the matrix
void Matrix::PrintMatrix(int** matrix, int rows, int cols) {
	cout << "matrix:- " << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << matrix[i][i] << " ";
		}
		cout << endl;
	}
}
//destructor for matrix
Matrix::~Matrix() {
	cout << "matrix destructor called" << endl;
}
	

SharedPointer::SharedPointer(int value) {
	ptr = make_shared<int>(value);
}

int SharedPointer:: getValue() {
	return *ptr;
}
int  SharedPointer::SetCount(shared_ptr<int>&p) {
	return p.use_count();
}
