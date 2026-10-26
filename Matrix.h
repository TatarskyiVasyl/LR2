#pragma once
#include <iostream>
#include <string>

class Matrix
{
public:
    static const int MAX_SIZE = 10;

    class Row
    {
        int* cells;
        int  length;
    public:
        Row(int* cells, int length);
        int& operator [] (int j) const;
    };

    class ConstRow
    {
        const int* cells;
        int        length;
    public:
        ConstRow(const int* cells, int length);
        const int& operator [] (int j) const;
    };

private:
    int size;
    int data[MAX_SIZE][MAX_SIZE];

    static void checkSize(int size);
    void        checkIndex(int row, int col) const;

public:
    Matrix();
    explicit Matrix(int size);
    Matrix(int size, int value);
    explicit Matrix(const std::string& text);
    Matrix(const Matrix& other);
    ~Matrix();

    int  Size() const;
    int  Get(int row, int col) const;
    void Set(int row, int col, int value);

    Row      operator [] (int row);
    ConstRow operator [] (int row) const;

    Matrix& operator = (const Matrix& other);
    Matrix& operator += (const Matrix& other);
    operator std::string() const;

    double Norm() const;

    friend Matrix operator + (const Matrix& a, const Matrix& b);
    friend bool   operator == (const Matrix& a, const Matrix& b);
    friend bool   operator != (const Matrix& a, const Matrix& b);

    friend std::ostream& operator << (std::ostream& out, const Matrix& m);
    friend std::istream& operator >> (std::istream& in, Matrix& m);
};
