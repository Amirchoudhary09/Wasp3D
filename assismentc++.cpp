#include "oops.h"
#include <iostream>
#include <vector>
#include"pointers.h"
using namespace std;
int main()
{    // Question 1: Deep copy vs Shallow copy and operator overloading for assignment operator
	 //node:- fro deep copy and shallow  we create  different different object so easily 
	 // we can identify that which one is deep copy and which one is shallow copy 
	 // but for operator overloading for assignment operator we create 3 object 


	// deep copy
	cout<<"Deep copy program execution :- " << endl;
	Wasp3D* wasp3d = new Wasp3D(10, 20);
	Wasp3D* wasp3d2 = new Wasp3D(*wasp3d);
	cout << *wasp3d->Data << endl;
	cout << *wasp3d2->Data << endl;
	*wasp3d2->Data = 50;
	cout << *wasp3d->Data << endl;
	cout << *wasp3d2->Data << endl;
	if (wasp3d->Data == wasp3d2->Data) {
		cout << " it is a Shallow copy" << endl;
	}
	else {
		cout << " it is a Deep copy" << endl;
	}
	if (wasp3d) {
		delete wasp3d;
	}
	wasp3d = nullptr;
	if (wasp3d2) {
		delete wasp3d2;
	}
	wasp3d2 = nullptr;


	// shallow copy
	cout << "shallow copy program execution:- " << endl;
	Wasp3D* wasp3d3 = new Wasp3D(20, 10);
	Wasp3D* wasp3d4 = wasp3d3;
	cout << *wasp3d3->Data << endl;
	cout << *wasp3d4->Data << endl;
	*wasp3d4->Data = 100;
	cout << *wasp3d3->Data << endl;
	cout << *wasp3d4->Data << endl;
	if (wasp3d3->Data == wasp3d4->Data) {
		cout << "it is a Shallow copy" << endl;
	}
	else {
		cout << " it is a Deep copy" << endl;
	}
	if (wasp3d3) {
		delete wasp3d3;
	}
	wasp3d3 = nullptr;

	//operator overloading for assignment operator
	cout << "operator overloading for assignment operator:- " << endl;
	Wasp3D* wasp3d5 = new Wasp3D(30, 40);
	Wasp3D* wasp3d6 = new Wasp3D(20, 90);
	Wasp3D* wasp3d7 = *wasp3d5 + *wasp3d6;
	cout << *wasp3d7->Data << endl;
	cout << wasp3d7->Value << endl;
	if (wasp3d5) {
		delete wasp3d5;
	}
	wasp3d5 = nullptr;
	if (wasp3d6) {
		delete wasp3d6;
	}
	wasp3d6 = nullptr;

	//Question no2:-template pointer
	cout << "template  function:- " << endl;

	int* arr = new int[5] { 1, 2, 3, 4, 5 };
	PrintArray<int>(arr, 5);
	if (arr)delete arr;
	arr = nullptr;
	cout << "Matrix using double pointer:- " << endl;
	Matrix* matrix = new Matrix();
	int** matrix1 = matrix->CreateMatrix(3, 3);
	matrix1 = matrix->FillMatrix(matrix1, 3, 3);
	matrix->PrintMatrix(matrix1, 3, 3);

	 //matrix1 memory free
	for (int i = 0; i < 3; i++) {
		delete[] matrix1[i];
	}
	delete[] matrix1;
	matrix1 = nullptr;

	//delete matrix object
	if (matrix)delete matrix;
	matrix = nullptr;

	//shared pointer
	cout << "Shared pointer:- " << endl;
	SharedPointer* p1 = new SharedPointer(10);
	cout << p1->getValue() << endl;
	cout << p1->SetCount(p1->ptr) << endl;
	shared_ptr<int> p2 = p1->ptr;
	cout << p1->SetCount(p2) << endl; // ownership share 

	//QUESTION NO3:-abstract class and  virtual function with stl container
	cout << "shape classes:- " << endl;
	vector<Shape*>shapes; // we choise vector because it manange memory dynamic 
	shapes.push_back(new Rectangle(10, 20));
	shapes.push_back(new Square(89));
	shapes.push_back(new Rectangle(35, 20));
	shapes.push_back(new Square(18));
	for (auto x : shapes) {
		cout << "Value of area:- "<< x->Area() << endl;
	 }
	for (auto x : shapes) {
		delete x;
	}
	shapes.clear();
	return 0;

	//notes:- 
	//shared_ptr:- which auto matic delete after execution
	// row pointer:- that is  store  address of  another variable  and delete manualy
	// double pointer :- which is  point another pointer 
	// virtual :-which easily derive  another class 
	
}