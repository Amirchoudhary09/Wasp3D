#ifndef OOPS_H
#define OOPS_H
class Wasp3D {
public:
    int* Data;
    int Value;
	//constructor
    Wasp3D(int Data,int Value);
	//copy constructor if we want a deep copy
    Wasp3D(Wasp3D& obj);
	//destructor
    ~Wasp3D();
	//oprator overloading for assignment operator
    Wasp3D* operator+(const Wasp3D& obj);
};
// base class
class Shape {
public:
	int Length;
	int Width;
	Shape(int Length, int Width);
	virtual int Area()=0;
	virtual ~Shape();
};
// derived class
class Rectangle : public Shape {
public:
	Rectangle(int Length, int Width);
	int Area();
	~Rectangle();
};
// derived class
class Square : public Shape {
public:
	Square(int Length);
	int Area();
	~Square();
};
#endif
