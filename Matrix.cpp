#include "Matrix.h"
#include <climits>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

namespace
{
    int parseInt(const string& token)
    {
        size_t i = 0;
        bool negative = false;
        if (i < token.size() && (token[i] == '+' || token[i] == '-'))
            negative = token[i++] == '-';
        if (i == token.size())
            throw invalid_argument("number expected in matrix text");
        long long value = 0;
        for (; i < token.size(); i++)
        {
            if (token[i] < '0' || token[i] > '9')
                throw invalid_argument("wrong character in matrix text");
            value = value * 10 + (token[i] - '0');
            if (value > static_cast<long long>(INT_MAX) + 1)
                throw invalid_argument("number in matrix text is out of range");
        }
        if (negative)
            value = -value;
        if (value < INT_MIN || value > INT_MAX)
            throw invalid_argument("number in matrix text is out of range");
        return static_cast<int>(value);
    }

    vector<vector<int>> parseRows(const string& text)
    {
        vector<vector<int>> rows;
        vector<int> row;
        string token;
        auto endToken = [&]()
            {
                if (!token.empty())
                {
                    row.push_back(parseInt(token));
                    token.clear();
                }
            };
        auto endRow = [&]()
            {
                endToken();
                if (!row.empty())
                {
                    rows.push_back(row);
                    row.clear();
                }
            };
        for (char c : text)
        {
            if (c == ';' || c == '\n' || c == '\r')
                endRow();
            else if (c == ' ' || c == '\t' || c == ',')
                endToken();
            else
                token += c;
        }
        endRow();
        return rows;
    }
}

Matrix::Row::Row(int* cells, int length) : cells(cells), length(length)
{
}

int& Matrix::Row::operator [] (int j) const
{
    if (j < 0 || j >= length)
        throw out_of_range("column index is out of range");
    return cells[j];
}

Matrix::ConstRow::ConstRow(const int* cells, int length) : cells(cells), length(length)
{
}

const int& Matrix::ConstRow::operator [] (int j) const
{
    if (j < 0 || j >= length)
        throw out_of_range("column index is out of range");
    return cells[j];
}

void Matrix::checkSize(int size)
{
    if (size < 1 || size > MAX_SIZE)
        throw invalid_argument("matrix size must be in range [1; 10]");
}

void Matrix::checkIndex(int row, int col) const
{
    if (row < 0 || row >= size || col < 0 || col >= size)
        throw out_of_range("matrix index is out of range");
}

Matrix::Matrix() : size(3)
{
    for (int i = 0; i < MAX_SIZE; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            data[i][j] = 0;
}

Matrix::Matrix(int size)
{
    checkSize(size);
    this->size = size;
    for (int i = 0; i < MAX_SIZE; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            data[i][j] = 0;
}

Matrix::Matrix(int size, int value)
{
    checkSize(size);
    this->size = size;
    for (int i = 0; i < MAX_SIZE; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            data[i][j] = (i < size && j < size) ? value : 0;
}

Matrix::Matrix(const string& text)
{
    vector<vector<int>> rows = parseRows(text);
    int n = static_cast<int>(rows.size());
    checkSize(n);
    for (const vector<int>& r : rows)
        if (static_cast<int>(r.size()) != n)
            throw invalid_argument("matrix must be square: every row must contain N numbers");
    size = n;
    for (int i = 0; i < MAX_SIZE; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            data[i][j] = (i < n && j < n) ? rows[i][j] : 0;
}

Matrix::Matrix(const Matrix& other) : size(other.size)
{
    for (int i = 0; i < MAX_SIZE; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            data[i][j] = other.data[i][j];
}

Matrix::~Matrix()
{
}

int Matrix::Size() const
{
    return size;
}

int Matrix::Get(int row, int col) const
{
    checkIndex(row, col);
    return data[row][col];
}

void Matrix::Set(int row, int col, int value)
{
    checkIndex(row, col);
    data[row][col] = value;
}

Matrix::Row Matrix::operator [] (int row)
{
    if (row < 0 || row >= size)
        throw out_of_range("row index is out of range");
    return Row(data[row], size);
}

Matrix::ConstRow Matrix::operator [] (int row) const
{
    if (row < 0 || row >= size)
        throw out_of_range("row index is out of range");
    return ConstRow(data[row], size);
}

Matrix& Matrix::operator = (const Matrix& other)
{
    size = other.size;
    for (int i = 0; i < MAX_SIZE; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            data[i][j] = other.data[i][j];
    return *this;
}

Matrix& Matrix::operator += (const Matrix& other)
{
    return *this = *this + other;
}

Matrix::operator string() const
{
    ostringstream out;
    for (int i = 0; i < size; i++)
    {
        if (i > 0)
            out << '\n';
        for (int j = 0; j < size; j++)
            out << (j > 0 ? " " : "") << data[i][j];
    }
    return out.str();
}

double Matrix::Norm() const
{
    double sum = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            sum += static_cast<double>(data[i][j]) * data[i][j];
    return sqrt(sum);
}

Matrix operator + (const Matrix& a, const Matrix& b)
{
    if (a.size != b.size)
        throw invalid_argument("matrices must have the same size");
    Matrix result(a.size);
    for (int i = 0; i < a.size; i++)
        for (int j = 0; j < a.size; j++)
        {
            long long sum = static_cast<long long>(a.data[i][j]) + b.data[i][j];
            if (sum > INT_MAX || sum < INT_MIN)
                throw overflow_error("sum of elements does not fit into int");
            result.data[i][j] = static_cast<int>(sum);
        }
    return result;
}

bool operator == (const Matrix& a, const Matrix& b)
{
    if (a.size != b.size)
        return false;
    for (int i = 0; i < a.size; i++)
        for (int j = 0; j < a.size; j++)
            if (a.data[i][j] != b.data[i][j])
                return false;
    return true;
}

bool operator != (const Matrix& a, const Matrix& b)
{
    return !(a == b);
}

ostream& operator << (ostream& out, const Matrix& m)
{
    out << string(m);
    return out;
}

istream& operator >> (istream& in, Matrix& m)
{
    int n;
    cout << "size = ";
    if (!(in >> n))
        return in;
    if (n < 1 || n > Matrix::MAX_SIZE)
    {
        in.setstate(ios::failbit);
        return in;
    }
    Matrix t(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            cout << "[" << i << "][" << j << "] = ";
            if (!(in >> t.data[i][j]))
                return in;
        }
    m = t;
    return in;
}
