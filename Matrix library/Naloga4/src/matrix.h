#include <unordered_map>
#include <string>
#include <vector>

class Matrix {

    private:    
        int height;
        int width;
        int** data;
        std::vector<int> data_tmp;
        std::unordered_map<std::string,int> variables;
        Matrix(int n, int m, int entries[]); // this is a private constructor
        Matrix(int n, int m, int** entries); // private constructor
        Matrix(int n, int m, std::vector<int> entries);
        Matrix(int n, int m, std::string mode);

    public:
        Matrix() {}; // public empty constructor for convience, possibly remove later
        ~Matrix(); // this is the destructor
        std::vector<int> getData() { return data_tmp; };
        static Matrix construct(int n, int m, int entries[]);
        static Matrix construct(int n, int m, int** entries);
        static Matrix construct(int n, int m, std::vector<int> entries);
        static Matrix eye(int n);
        static Matrix zeros(int n, int m);
        static Matrix ones(int n, int m);
        static Matrix read(std::string file);
        Matrix concat(Matrix mat, char orientation);
        void write(std::string fileName, bool overrideFile);
        void write(std::string fileName);
        Matrix transpose();
        void set(int i, int j, int value);
        int get(int i, int j);
        int get(int i);
        int getHeight() { return height; };
        int getWidth() { return width; };
        Matrix sub_row(int i, int j);
        Matrix sub_col(int i, int j);
        Matrix subMatrix(int startRow, int endRow, int startCol, int endCol);
        void setSubMatrix(int row, int col, Matrix  subMatrix);
        //Matrix concat(Matrix mat, char orientation);
        Matrix lowerDiag();
        Matrix upperDiag();
        Matrix diag();
        void setDiag(Matrix mat);
        bool isUpperDiagonal();
        bool isLowerDiagonal();
        std::vector<int> dim();
        void print();
        void insertVariable(std::string var, int val);
        int getVariable(std::string var);
        void printVariable(std::string var);
        Matrix add(Matrix mat);
        Matrix sub(Matrix mat);
        Matrix multiply_alg1(Matrix mat);
        Matrix multiply_alg2(Matrix mat);
        Matrix solve(Matrix b);
        Matrix forwardSubstitution(Matrix y);
        Matrix backSubstitution(Matrix y);
        std::tuple<Matrix, Matrix, Matrix> LU();

        // Overloaded operators
        Matrix operator+(const Matrix& mat);
        Matrix operator~();
        Matrix operator()(std::string querry);
        Matrix operator()(int i);
        Matrix operator-(const Matrix& mat);
        Matrix operator*(const Matrix& mat);    
        Matrix operator/(const Matrix& mat);
        Matrix operator|(const Matrix& mat);
        Matrix operator,(const Matrix& mat);
};

