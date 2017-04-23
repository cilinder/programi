#include <iostream>
#include "matrix.h"

using namespace std;


int encodeChar(char c) {

    int code = 0;

    if ( c >= 'A' && c <= 'Z' ) {
        code = (int)c-'A';
    }
    else {
        switch(c) {
            case '.':
                code = 26;
                break;
            case ',':
                code = 27;
                break;
            case '\n':
                code = 28;
                break;
            case '?':
                code = 29;
                break;
            case '\'':
                code = 30;
                break;
            case ' ':
                code = 31;
                break;
            default:
                cerr << "Invalid character" << endl;
                break;
        }
    }
    return code;
}

Matrix encodeSequence(vector<char> seq, Matrix G) {

    int n = seq.size();
    vector<int> data (n * 5);

    for (int i = 0; i < n; ++i) {
        int code = encodeChar(seq[i]);
        for (int j = 0; j < 5; ++j) {
            int next = code & 1;
            data[5*(i+1)-1-j] = next;
            code = code >> 1;
        }
    }
    // add one 0 to the end to make it 26 long
    data.push_back(0);
    Matrix s = Matrix::construct(1, data.size(), data);
    Matrix c = s * G;
    
    return c;
}

int run_encoding(int argc, char** argv) {

    vector<int> data = {1,1,1,0,1,0};
    Matrix H = Matrix::read("/home/jure/TKK/Naloga4/H.txt");
    H.print();

    Matrix X = H(":,0:25");

    X = X.transpose();
    Matrix I = Matrix::eye(31);
    Matrix Y = I | X;
    Y.print();

    Matrix G = Matrix::eye(31) | X;

    vector<char> seq { 'P', 'E', 'T', 'E', 'K' };
    Matrix c = encodeSequence(seq, H);
    c.print();

    return 0;
}

int run_test() {

    Matrix T = Matrix::read("/home/jure/TKK/Naloga4/data/test_matrix1.txt");

    vector<int> tmp {1,0,0,1};
    Matrix b1 = Matrix::construct(4,1,tmp);

    vector<int> data2 {1,1,0,1,1,0};
    Matrix B = Matrix::construct(3,2,data2);

    vector<int> data {1,1,0,0,1,1,1,1,1};
    Matrix A = Matrix::construct(3,3,data);

    vector<int> data3 {1,0,1};
    Matrix b = Matrix::construct(3,1,data3);

    A.print();
    b.print();

    Matrix x = A/b;
    x.print();

    return 0;
}


int main(int argc, char ** argv) {

    int result = 0;

    try  {
        run_test();
        //run_encoding(argc, argv);
    }
    catch(exception& e) {
        cout << e.what();
    }

    return 0;
}
