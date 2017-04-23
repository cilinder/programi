#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <tuple>
#include "matrix.h"


#define MATRIX_INDEX_OUT_OF_BOUNDS_ERROR 1
#define MATRIX_CONSTRUCTION_ERROR 2
#define MATRIX_ADDITION_ERROR 3
#define MATRIX_GET_ELEMENT_ERROR 4
#define MATRIX_MULTIPLICATION_ERROR 5
#define MATRIX_SOLVE_SYSTEM_ERROR 6
#define MATRIX_SUBSCRIPT_ERROR 7
#define MATRIX_FILE_OPENING_ERROR 8
#define MATRIX_DIMENSION_ERROR 9

#define DEBUG() printf("line: %d\n", __LINE__);

using namespace std;


Matrix::Matrix(int n, int m, int entries[]) {

    this->height = n;
    this->width = m;

    // allocate the memory for the data
    this->data = (int**) malloc(n*sizeof(int*));
    for (int i = 0; i < n; ++i) {
        data[i] = (int*)malloc(m*sizeof(int));
        memcpy(data[i], entries+(m*i), m*sizeof(int));
    }

    this->data_tmp = vector<int>(entries, entries+n*m);

}

Matrix::Matrix(int n, int m, int** entries) {

    this->height = n;
    this->width = m;
    
    // allocate memory for 2d array
    this->data = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        data[i] = (int*)malloc(m * sizeof(int));

        // copy the memory from the entries array
        memcpy(data[i], entries[i], sizeof(int)*m);
    }

    this->data_tmp = vector<int>(n*m);
    vector<int>::iterator it = this->data_tmp.begin();
    
    for (int i = 0; i < n; ++i) {
        data_tmp.insert(it, entries[i], entries[i]+m);
    }

}

Matrix::Matrix(int n, int m, vector<int> entries) {

    this->height = n;
    this->width = m;

    this->data = new int*[n];
        
    int* a = entries.data();
    for (int i = 0; i < n; ++i) {
        this->data[i] = new int[m];

        copy(a+(m*i), a+(m*(i+1)), this->data[i]);
    }

    this->data_tmp = entries;
}

Matrix::Matrix(int n, int m, string mode) {

    this->height = n;
    this->width = m;

    // IDENTITY
    if ( mode == "Identity" ) {
        this->data_tmp = vector<int>(n*m, 0);
        // Set the diagonal elements to 1
        for (int i = 0; i < n; ++i) {
            this->data_tmp[i*m+i] = 1;
        }
    }
    else if ( mode == "Ones" ) {
        this->data_tmp = vector<int>(n*m, 1);
    }
    else if ( mode == "Zeros") {
        this->data_tmp = vector<int>(n*m, 0);
    }
    else {
        throw logic_error("Incorrect mode for constructing matrix");
    }
}

// Destructor
Matrix::~Matrix() {

    // free the memory allocated for the data array
    //cout << "Object is being deleted" << endl;
    //free(this->data);

}

Matrix Matrix::construct(int n, int m, int entries[]) {

    Matrix mat = Matrix(n, m, entries);
    return mat;

}


Matrix Matrix::construct(int n, int m, int** entries) {

    Matrix mat = Matrix(n, m, entries);
    return mat;

}

Matrix Matrix::construct(int n, int m, vector<int> entries) {

    Matrix mat (n, m, entries);
    return mat;

}

Matrix Matrix::eye(int n) {

    try {
        Matrix mat (n, n, "Identity");
        return mat;
    }
    catch(const logic_error& e) {
        cout << e.what() << endl;
    }

}

Matrix Matrix::zeros(int n, int m) {

    try {
        Matrix mat (n, m, "Zeros");
        return mat;
    }
    catch (const logic_error& e) {
        cout << e.what() << endl;
    }

}

Matrix Matrix::ones(int n, int m) {

    try {
        Matrix mat (n, m, "Ones");
        return mat;
    }
    catch (const logic_error& e) {
        cout << e.what() << endl;
    }

}

Matrix Matrix::read(string fileName) {

    ifstream file;
    file.open(fileName);
    string line;
    int num_lines = 0;
    vector<int> data;
    int width = 0;
    int lastWidth = 0;

    if ( file.is_open() ) {

        while ( getline(file, line) ) {
            lastWidth = 0;
            for (int i = 0; i < line.length(); ++i) {
                if ( line[i] == ' ' || line[i] == '\t' ) {
                    continue;
                }
                lastWidth++;
                int element = (int) line[i] - '0';
                if ( element != 0 && element != 1 ) {
                    cerr << "Element must be 1 or 0" << endl;
                    throw MATRIX_FILE_OPENING_ERROR;
                }
                data.push_back(element);
            }
            if ( num_lines == 0 ) {
                width = lastWidth;
            }
            if ( num_lines > 0 &&  lastWidth != width ) {
                cerr << "Input matrix must be rectangular" << endl;
                throw MATRIX_DIMENSION_ERROR;
            }
            num_lines++;
        }

        Matrix mat (num_lines, width, data);

        file.close();
        return mat;
    }
    else {
        cerr << "Could not open file" << endl;
        throw runtime_error("Could not open file");
    }
}

void Matrix::write(string fileName, bool overrideFile) {

    bool write = false;
    string message;

    // file already exsists
    if (FILE *file = fopen(fileName.c_str(), "r")) {
        fclose(file);
        if ( overrideFile ) {
            write = true;
        }
        else {
            message += "File already exsists, set override flag to true if you want to replace it" ;
        }
    }
    else {
        write = true;
    }

    if ( write ) {
        ofstream file;
        file.open(fileName, ios::out | ios::trunc);

        if ( file.is_open() ) {

            int n = this->getHeight();
            int m = this->getWidth();

            for (int i = 0; i < n; ++i) {
                string line = "";
                for (int j = 0; j < m; ++j) {
                    line += to_string(this->data[i][j]);
                    if ( j < (m-1) ) {
                        line += " ";
                    }
                }
                file << line;
                file << endl;
            }
            file.close();
        }
        else {
            cerr << "Unable to open file for writing" << endl;
            throw runtime_error("Unable to open file for writing");
        }
    }
    else {
        throw runtime_error(message);
    }
}

void Matrix::write(string fileName) {

    write(fileName, false);

}


inline int Matrix::get(int i, int j) {
    if ( this->height < i || i < 0 || this->width < j || j < 0) {
        throw logic_error("Unable to get element at indexes: " + to_string(i) + ", " + to_string(j));
    }

    return this->data_tmp[i*this->width+j];
}

int Matrix::get(int i) {

    int n = this->getHeight();
    int m = this->getWidth();

    if ( n == 1 || m == 1) {
        return this->data_tmp[i];
    }
    else {
        cerr << "get(i) function expects matrix to be a vector (either nx1 or 1xm)" << endl;
        throw MATRIX_SUBSCRIPT_ERROR;
    }
}

void Matrix::set(int i, int j, int value) {

    if (value != 0 && value != 1) {
        throw logic_error("Matrix values can only be 0 or 1");
    }
    if ( i < 0 || j < 0 || i > this->getHeight() || j > this->getWidth() ) {
        throw logic_error("Index out of bounds of matrix");
    }
    this->data_tmp[i*this->width+j] = value;
}

Matrix Matrix::sub_row(int i, int j) {

    int n = this->getHeight();
    int m = this->getWidth();

    if ( i > n || j > n || i < 0 || j < 0 ) {
        throw logic_error("Matrix index out of bounds");
    }

    vector<int>::iterator start_i = this->data_tmp.begin()+(i*m);
    vector<int>::iterator end_i = this->data_tmp.begin()+((i+1)*m);
    vector<int>::iterator start_j = this->data_tmp.begin()+(j*m);
    vector<int>::iterator end_j = this->data_tmp.begin()+((j+1)*m);

    // swap the i-th sector (row) and the j-th sector
    vector<int> tmp (start_i, end_i);
    copy(start_j, end_j, start_i);
    copy(tmp.begin(), tmp.end(), start_j);

    // create the new matrix
    Matrix mat (n, m, this->data_tmp);

    // swap the sectors back
    copy(start_i, end_i, start_j);
    copy(tmp.begin(), tmp.end(), start_i);
    
    return mat;

}

Matrix Matrix::sub_col(int i, int j)  {

    if ( i > this->getWidth() || j > this->getWidth() || i < 0 || j < 0 ) {
        throw logic_error("Matrix index out of bounds");
    }

    int n = this->getHeight();
    int m = this->getWidth();

    // swap the i-th and j-th column
    for (int k = 0; k < n; ++k) {
        int ind_i = k*m+i;
        int ind_j = k*m+j;

        int tmp = this->data_tmp[ind_i];
        this->data_tmp[ind_i] = this->data_tmp[ind_j];
        this->data_tmp[ind_j] = tmp;
    }
    
    Matrix newMat (n,m,this->data_tmp);

    // swap them back
    for (int k = 0; k < n; ++k) {
        int ind_i = k*m+i;
        int ind_j = k*m+j;

        int tmp = this->data_tmp[ind_i];
        this->data_tmp[ind_i] = this->data_tmp[ind_j];
        this->data_tmp[ind_j] = tmp;
    }

    return newMat;
}

void Matrix::setSubMatrix(int row, int col, Matrix subMatrix) {

    int n = this->getHeight();
    int m = this->getWidth();

    if ( row < 0 || row > n || col < 0 || col > m ) {
        throw logic_error("Matrix index out of bounds");
    }
    
    int n1 = subMatrix.getHeight();
    int m1 = subMatrix.getWidth();

    if ( row + n1 > n || col + m1 > m ) {
        throw logic_error("Matrix index out of bounds");
    }

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            this->set(i + row, j + col, subMatrix.get(i,j));
        }
    }
}

Matrix Matrix::concat(Matrix mat, char orientation) {

    int n1 = this->getHeight();
    int m1 = this->getWidth();
    int n2 = mat.getHeight();
    int m2 = mat.getWidth();

    try {
        
        if ( orientation == 'v' ) {
            if ( m1 != m2 ) {
                throw invalid_argument("Invalid matrix dimensions for horizontal concatenation\n");
            }
            int n = n1 + n2;
            int m = m1;

            this->height = n;
            this->width = m;

            vector<int>::iterator mat1_start = this->data_tmp.begin(); 
            vector<int>::iterator mat1_break = this->data_tmp.end();
            vector<int>::iterator mat2_start = mat.data_tmp.begin();
            vector<int>::iterator mat2_break = mat.data_tmp.end();

            vector<int> data (n*m);
            vector<int>::iterator newData_start = data.begin();

            newData_start = copy(mat1_start, mat1_break, newData_start);
            copy(mat2_start, mat2_break, newData_start);

            Matrix newMat (n,m,data);
            return newMat;

        }
        else if ( orientation == 'h' ) {
            if ( n1 != n2 ) {
                throw invalid_argument("Invalid matrix dimensions for horizontal concatenation\n");
            }
            int n = n1;
            int m = m1 + m2;

            this->height = n;
            this->width = m;

            vector<int>::iterator mat1_start = this->data_tmp.begin(); 
            vector<int>::iterator mat1_break = this->data_tmp.begin() + m1;
            vector<int>::iterator mat2_start = mat.data_tmp.begin();
            vector<int>::iterator mat2_break = mat.data_tmp.begin() + m2;

            vector<int> data = vector<int>(n*m);
            vector<int>::iterator newData_start = data.begin();

            for (int i = 0; i  < n; ++i) {

                newData_start = copy(mat1_start, mat1_break, newData_start);
                mat1_start = mat1_break;
                mat1_break += m1;

                newData_start = copy(mat2_start, mat2_break, newData_start);
                mat2_start = mat2_break;
                mat2_break += m2;
            }
            Matrix newMat (n,m, data);
            return newMat;
        }
    }
    catch(const exception& e) {
        cout << e.what();
    }

}

Matrix Matrix::transpose() {

    int n = this->getHeight();
    int m = this->getWidth();

    vector<int> newData (m*n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            newData[n*j+i] = this->get(i,j);
        }
    }

    Matrix mat (m,n,newData);
    return mat;
}

vector<int> Matrix::dim() {
    vector<int> dim { this->getHeight(), this->getWidth() };
    return dim;
}

void Matrix::print() {

    int times = min(this->width, min(2, this->width));
    int n = this->height;
    int m = this->width;

    for (int i = 0; i < times; ++i) {
        cout << "_";
    }
    for (int i = 2; i < (2*width-1); ++i) {
        cout << " ";
    }
    if ( times == 1) {
        cout << " ";
    }
    for (int i = 0; i < times; ++i) {
        cout << "_";
    }
    cout << endl;

    for (int i = 0; i < n; ++i) {
        cout << "|";
        for (int j = 0; j < m; ++j) {
            cout << get(i,j);
            if ( j == width-1) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < times; ++i) {
        cout << "-";
    }
    for (int i = 2; i < (2*width-1); ++i) {
        cout << " ";
    }
    if ( times == 1) {
        cout << " ";
    }
    for (int i = 0; i < times; ++i) {
        cout << "-";
    }
    cout << endl;
}

Matrix Matrix::add(Matrix mat) {
    if ( this->height != mat.getHeight() || this->width != mat.getWidth() ) {
        throw MATRIX_ADDITION_ERROR;
    }

    int n = this->height;
    int m = this->width;

    vector<int> newData (n*m);
    for (int i = 0; i < n*m; ++i) {
        newData[i] = this->data_tmp[i] ^ mat.data_tmp[i];
    }
    Matrix newMat (n,m,newData);
    return newMat;

}

Matrix Matrix::sub(Matrix mat) {

    Matrix newMat = this->add(mat);
    return newMat;

}

Matrix Matrix::subMatrix(int startRow, int endRow, int startCol, int endCol) {

    if ( startRow > endRow || startCol > endCol) {
        throw logic_error("Invalid indexes for submatrix, conditions are:\n\n\t0<=startRow<=endRow<=n\n\t0<=startCol<=endCol<=m\n\n");
    }

    int n = this->getHeight();
    int m = this->getWidth();

    if ( startRow < 0 || endRow >= n ) {
        throw logic_error("Invalid indexes for submatrix, conditions are:\n\n\t0<=startRow<=endRow<=n\n\t0<=startCol<=endCol<=m\n\n");
    }
    if ( startCol < 0 || endCol >= m ) {
        throw logic_error("Invalid indexes for submatrix, conditions are:\n\n\t0<=startRow<=endRow<=n\n\t0<=startCol<=endCol<=m\n\n");
    }

    int newN = endRow - startRow + 1;
    int newM = endCol - startCol + 1;

    vector<int> newData (newN*newM);
    for (int i = 0; i < newN; ++i) {
        for (int j = 0; j < newM; ++j) {
            newData[i*newM+j] = this->get(i+startRow,j+startCol);
        }
    }

    Matrix subMatrix (newN, newM, newData);
    return subMatrix;

}

Matrix Matrix::lowerDiag() {

    int n = this->getHeight();
    int m = this->getWidth();

    vector<int> newData (n*m);
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            newData[i*m+j] = this->get(i,j);
        }
    }

    Matrix newMat (n,m,newData);
    return newMat;

}


Matrix Matrix::upperDiag() {

    int n = this->getHeight();
    int m = this->getWidth();

    vector<int> newData (n*m);
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < m; ++j) {
            newData[i*m+j] = this->get(i,j);
        }
    }

    Matrix newMat (n,m,newData);
    return newMat;

}

Matrix Matrix::diag() {

    int n = this->getHeight();
    int m = this->getWidth();

    int k = min(n, m);

    vector<int> newData (k);
    for (int i = 0; i < k; ++i) {
        newData[i] = this->get(i,i);
    }

    Matrix diag (k,1,newData);
    return diag;

}

void Matrix::setDiag(Matrix mat) {

    int n1 = this->getHeight();
    int m1 = this->getWidth();

    int n2 = mat.getHeight();
    int m2 = mat.getHeight();

    bool column = true;

    if ( (n2 != 1 && m2 != 1) ) {
        throw logic_error("To set diagonal, must provide vector of length " + to_string(min(n1, m1)) + "\n");
    }
    if ( max(n2, m2) != min(n1, m1) ) {
        throw logic_error("Matrix dimensions must match to set diagonal\n");
    }

    if ( n2 < m2 ) {
        column = false;
    }

    if ( column ) {
        for (int i = 0; i < n2; ++i) {
            this->set(i,i, mat.get(i,0)); 
        }
    }
    else {
        for (int i = 0; i < m2; ++i) {
            this->set(i,i, mat.get(0,i));
        }
    }
} 

bool Matrix::isUpperDiagonal() {

    int n = this->getHeight();
    int m = this->getWidth();

    if ( m != n ) {
        return false;
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if ( this->get(i,j) != 0 ) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::isLowerDiagonal() {

    int n = this->getHeight();
    int m = this->getWidth();

    if ( n != m ) {
        return false;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if ( this->get(i,j) != 0 ) {
                return false;
            }
        }
    }
    return true;
}

Matrix Matrix::multiply_alg1(Matrix mat) {

    int n1 = this->height;
    int m1 = this->width;
    int n2 = mat.getHeight();
    int m2 = mat.getWidth();

    if ( m1 != n2 ) {
        throw logic_error("Matrix dimensions must match for multiplication");
    }

    vector<int> newData (n1*m2);
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m2; ++j) {
            for (int k = 0; k < m1; ++k) {
                newData[i*m2+j] = newData[i*m2+j] ^ (this->get(i,k) & mat.get(k,j));
            }
        }
    }

    Matrix newMat (n1, m2, newData);
    return newMat;

}

Matrix Matrix::multiply_alg2(Matrix mat) {

    int n1 = this->height;
    int m1 = this->width;
    int n2 = mat.getHeight();
    int m2 = mat.getWidth();

    if ( m1 != n2 ) {
        throw logic_error("Matrix dimensions must match for multiplication");
    }

    vector<int> newData (n1*m2);

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            for (int k = 0; k < m2; ++k) {
                newData[i*m2+k] = newData[i*m2+k] ^ (this->get(i,j) && mat.get(j,k));
            }
        }
    }
    
    Matrix newMat (n1, m2, newData);
    return newMat;

}

Matrix Matrix::solve(Matrix b) {

    int n = this->getHeight();
    int m = this->getWidth();

    if ( n < m) {
        throw logic_error("Matrix Solve Error, can only solve for square matrix or overdetermined system");
    }

    if ( b.getWidth() != 1 || b.getHeight() != n) {
        throw logic_error("Matrix Solve Error: result vector not the correct dimension");
    }

    if ( n == m && this->isUpperDiagonal() ) {
        Matrix x = this->backSubstitution(b);
        return x;
    }
    if ( n == m && this->isLowerDiagonal() ) {
        Matrix x = this->forwardSubstitution(b);
        return x;
    }

    Matrix L;
    Matrix U;
    Matrix P;

    tie(L,U,P) = this->LU();

    Matrix p = P * b;

    Matrix y = L.forwardSubstitution(p);

    Matrix x = U.backSubstitution(y);
    return x;
}

tuple<Matrix, Matrix, Matrix> Matrix::LU() {

    int n = this->getHeight();
    int m = this->getWidth();

    Matrix U (n,m,this->data_tmp);
    Matrix L = Matrix::eye(n);
    Matrix P = Matrix::eye(n);

    if ( U.isUpperDiagonal() ) {
        return make_tuple(L,U,P);
    }
    if ( U.isLowerDiagonal() ) {
        return make_tuple(U,L,P);
    }

    for (int i = 0; i < (m-1); ++i) {
        if ( U.get(i,i) == 0 ) {
            cout << "Diagonal element 0" << endl;
            for (int j = i+1; j < n; ++j) {
                if ( U.get(j,i) != 0) {
                    U = U.sub_row(i,j);
                    P = P.sub_row(i,j);
                    L = L.sub_row(i,j);
                }
            }
        }
        if ( U.get(i,i) == 1 ) {

            Matrix L_i = Matrix::eye(n);
            
            Matrix a = U.subMatrix(i+1, n-1, i, i);

            L_i.setSubMatrix(i+1, i, a);

            U = L_i.multiply_alg2(U);

            L.setSubMatrix(i+1, i, a);

        }
        else {
            throw logic_error("Unable to decompose, matrix was singular\n");
        }
    }

    return make_tuple(L,U,P);
}

Matrix Matrix::forwardSubstitution(Matrix y) {

    int n = this->getHeight();
    int m = this->getWidth();

    vector<int> newData (n);
    for (int i = 0; i < n; ++i) {
        int sum = y.get(i);
        for (int j = 0; j < i; ++j ){
            sum = sum ^ (this->get(i,j) & newData[j]);
        }
        newData[i] = sum;
    }

    Matrix solution (n, 1, newData);
    return solution;

}

Matrix Matrix::backSubstitution(Matrix y) {

    int n = this->getHeight();
    int m = this->getWidth();

    vector<int> newData (m);
    for (int i = m-1; i >= 0; --i) {

        int sum = y.get(i);

        for (int j = i+1; j < m; ++j) {
            sum = sum ^ (this->get(i,j) && newData[j]);
        }
        newData[i] = sum;
    }

    Matrix solution (m, 1, newData);
    return solution;

}

void Matrix::insertVariable(string var, int val) {

    this->variables[var] = val;

}

int Matrix::getVariable(string var) {

    if ( (this->variables.find(var)) == this->variables.end()) {
        cout << "Could not find element" << endl;
    }
    return this->variables[var];
}

void Matrix::printVariable(string var) {

    cout << this->getVariable(var) << endl;
}


Matrix Matrix::operator*(const Matrix& mat) {

    Matrix newMat = this->multiply_alg2(mat);
    return newMat;

}

Matrix Matrix::operator+(const Matrix& mat) {

    Matrix newMat = this->add(mat);
    return newMat;

}

Matrix Matrix::operator~() {

    Matrix mat = this->add(ones(this->height, this->width));
    return mat;

}

Matrix Matrix::operator()(string querry) {


    regex rowsAndCols("(.+),(.+)");
    string number ("[[:space:]]*([[:digit:]]+|end)");
    regex fromTo(number+"[[:space:]]*:[[:space:]]*"+number);
    regex singleColon ("[[:space:]]*:[[:space:]]*");
    regex endKeyword ("[[:space:]]*end[[:space:]]*");
    smatch match;
    smatch numberMatch;

    int rowFrom = 0;
    int rowTo = 0;
    int colFrom = 0;
    int colTo = 0;

    bool matchedRow = false;
    bool matchedCol = false;

    if ( regex_search(querry, match, rowsAndCols) ) {
        if ( match.size() != 3) {
            cout << "Not 2 matches" << endl;
        }
        string row = match.str(1);
        string col = match.str(2);

        if ( regex_match(row, singleColon) ) {
            rowTo = this->getHeight()-1;
            matchedRow = true;
        }
        else if ( regex_match(row, numberMatch, fromTo) ) {
            if ( numberMatch.size() != 3 ) {
                cerr << "Not 2 matches" << endl;
            }
            try {
                rowFrom = stoi(numberMatch.str(1));
                string rowMatch = numberMatch.str(2);

                if ( regex_match(rowMatch, endKeyword) ) {
                    rowTo = this->getHeight()-1;
                }
                else {
                    rowTo = stoi(rowMatch);
                }
                matchedRow = true;
            }
            catch(invalid_argument e) {
                cerr << e.what();
            }

        }
        else {
            cout << "Error, could not match input" << endl;
        }

        if ( regex_match(col, singleColon) ) {
            colTo = this->getWidth()-1;
            matchedCol = true;
        }
        else if ( regex_match(col, numberMatch, fromTo) ) {
            if ( numberMatch.size() != 3 ) {
                cerr << "Not 2 matches" << endl;
            }
            try {
                colFrom = stoi(numberMatch.str(1));
                string colMatch = numberMatch.str(2);
                if ( regex_match(colMatch, endKeyword) ) {
                    colTo = this->getWidth()-1;
                }
                else {
                    colTo = stoi(numberMatch.str(2));
                }
                matchedCol = true;
            }
            catch(invalid_argument e) {
                cerr << e.what();
            }
        }
        else {
            cout << "Error, could not match input" << endl;
        }

    }
    else {
        cout << "No no" << endl;
    }

    if ( matchedRow && matchedCol ) {
        Matrix mat = this->subMatrix(rowFrom, rowTo, colFrom, colTo);
        return mat;
    }

}

Matrix Matrix::operator-(const Matrix& mat) {

    Matrix newMat = this->sub(mat);
    return newMat;

}

Matrix Matrix::operator/(const Matrix& mat) {

    try {
        Matrix newMat = this->solve(mat);
        return newMat;
    }
    catch(const logic_error& e) {
        cout << "Unable to solve system" << endl;
        throw e;
    }

}

Matrix Matrix::operator|(const Matrix& mat) {

    try {
        Matrix newMat = this->concat(mat, 'h');
        return newMat;
    }
    catch(const invalid_argument& e) {
        cout << "Unable to concatenate matrices" << endl;
        throw e;
    }
}

Matrix Matrix::operator,(const Matrix& mat) {

    try {
        Matrix newMat = this->concat(mat, 'v');
        return newMat;
    }
    catch(const invalid_argument& e) {
        cout << "Unable to concatenate matrices" << endl;
        throw e;
    }
}




