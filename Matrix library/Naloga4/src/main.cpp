#include <iostream>
#include <string>
#include "matrix.h"

using namespace std;

int main(int argc, char ** argv) {

    cout << "hello" << endl;
    int n = 4;
    int m = 4;

    int data[] = {1,0,1,1,1,1,0,0,1,0,0,1,0,1,1,0};
    int data2[] = {1,0,1,1,1,0,0,0,0,1,1,0,1,0,0,1};
    int data3[] = {1,0,0,1,1,0,0,0,1};
    int data4[] = {1,1,1,1,1,0,0,1,0};
    int data5[] = {1,0,0,1};
    int data6[] = {1,0,1,1,0,1};
    int data7[] = {0,1,1};

    Matrix A = Matrix::construct(3,2,data6);
    Matrix b = Matrix::construct(3,1,data7);

    string fileName = "/home/jure/TKK/Naloga4/matrika1.txt";

    A = Matrix::eye(4);
    Matrix B = Matrix::eye(4);

    Matrix D = A|B;

    Matrix C = (A,B);

    cout << "D = " << endl;
    D.print();

    C.print();
    return 0;

    cout << "A = " << endl;
    A.print();

    cout << "b = " << endl;
    b.print();

    cout << endl << "------------" << endl;

    Matrix x = A.solve(b);

    cout << "x = " << endl;
    x.print();

    cout << "A*x = " << endl;
    (A*x).print();


}
