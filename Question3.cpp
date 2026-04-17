#include<iostream>
#include"oops.h"
using namespace std;

	Shape::Shape(int Length, int Width) {
		this->Length = Length;
		this->Width = Width;
	}

	Shape::~Shape() {
		cout << "Shape destructor called" << endl;
	}

	Rectangle::Rectangle(int Length, int Width) : Shape(Length, Width) {
	}
	int Rectangle:: Area(){
		return Length * Width;
	}
	Rectangle::~Rectangle() {
		cout << "Rectangle destructor called" << endl;
	}


Square::Square(int Length) : Shape(Length, Length) {
	}
	int Square:: Area()  {
		return Length * Length;
	}
	Square::~Square() {
		cout << "Square destructor called" << endl;
	}