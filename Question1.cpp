#include<iostream>
#include"oops.h"
using namespace std;


Wasp3D::Wasp3D(int Data,int Value) {
    this->Data = new int(Data);
	this->Value = Value;

}
Wasp3D::Wasp3D(Wasp3D& obj) {
    this->Data = new int(*obj.Data);
	this->Value = obj.Value;
}
Wasp3D* Wasp3D::operator+(const Wasp3D& obj) {
    Wasp3D* temp = new Wasp3D(0, 0);
    temp->Value = this->Value + obj.Value;
    *temp->Data = *this->Data + *obj.Data;
    return temp;
}
Wasp3D::~Wasp3D() {
    cout << "Data value free  from Wasp3D obj" << endl;
    delete Data;
}