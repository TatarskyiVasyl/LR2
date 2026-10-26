#include "Matrix.h"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

template <typename Action>
static void Attempt(const string& what, Action action)
{
    try
    {
        action();
        cout << what << ": accepted" << endl;
    }
    catch (const exception& e)
    {
        cout << what << ": rejected (" << e.what() << ")" << endl;
    }
}

#pragma pack(push, 1)
struct PackedLayout { int size; int data[10][10]; };
struct PackedMixed { char c; double d; };
#pragma pack(pop)
struct AlignedLayout { int size; int data[10][10]; };
struct AlignedMixed { char c; double d; };

int main()
{
    cout << boolalpha;
    const int maxSize = Matrix::MAX_SIZE;

    cout << "--- sizes ---" << endl;
    cout << "MAX_SIZE                           = " << maxSize << endl;
    cout << "sizeof(Matrix)                     = " << sizeof(Matrix) << "  (the whole array is inside the object)" << endl;
    cout << "same fields, default alignment     = " << sizeof(AlignedLayout) << endl;
    cout << "same fields, #pragma pack(1)       = " << sizeof(PackedLayout) << endl;
    cout << "char + double, default alignment   = " << sizeof(AlignedMixed) << endl;
    cout << "char + double, #pragma pack(1)     = " << sizeof(PackedMixed) << endl;

    cout << endl << "--- creation of objects ---" << endl;
    Matrix m1;
    Matrix m2(2);
    Matrix m3(2, 5);
    Matrix m4("1 2 3; 4 5 6; 7 8 9");
    Matrix m5(m4);
    Matrix m6 = m4;
    Matrix m7(1);
    m7 = m3;
    cout << "m1 (default), size " << m1.Size() << ":" << endl << m1 << endl;
    cout << "m2 (zero 2x2):" << endl << m2 << endl;
    cout << "m3 (all 5):" << endl << m3 << endl;
    cout << "m4 (from string):" << endl << m4 << endl;
    cout << "m5 (copy of m4):" << endl << m5 << endl;
    cout << "m7 (assigned from m3), size " << m7.Size() << ":" << endl << m7 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    Matrix arr1[2];
    Matrix arr2[3] = { Matrix(1, 7), Matrix("1 0; 0 1"), Matrix(3, -1) };
    Matrix* p = new Matrix("2 3; 4 5");
    Matrix* parr = new Matrix[2];
    parr[0] = arr2[1];
    parr[1] = *p;
    cout << "arr1[0] size = " << arr1[0].Size() << ", arr2 sizes = " << arr2[0].Size() << ", "
        << arr2[1].Size() << ", " << arr2[2].Size() << endl;
    cout << "*p:" << endl << *p << endl;
    cout << "parr[0]:" << endl << parr[0] << endl;
    delete p;
    delete[] parr;

    cout << endl << "--- getters, setters, indexing ---" << endl;
    Matrix a(3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            a[i][j] = i * 3 + j + 1;
    cout << "a filled through a[i][j]:" << endl << a << endl;
    cout << "Size() = " << a.Size() << ", Get(1, 2) = " << a.Get(1, 2) << ", a[2][0] = " << a[2][0] << endl;
    a.Set(0, 0, 100);
    a[2][2] += 1;
    cout << "after Set(0, 0, 100) and a[2][2] += 1:" << endl << a << endl;
    Attempt("a.Get(3, 0)         ", [&] { a.Get(3, 0); });
    Attempt("a.Set(0, -1, 5)     ", [&] { a.Set(0, -1, 5); });
    Attempt("a[3][0]             ", [&] { int v = a[3][0]; (void)v; });
    Attempt("a[0][3]             ", [&] { int v = a[0][3]; (void)v; });
    Attempt("Matrix(0)           ", [&] { Matrix x(0); });
    Attempt("Matrix(11)          ", [&] { Matrix x(maxSize + 1); });
    Attempt("Matrix(\"1 2; 3\")    ", [&] { Matrix x("1 2; 3"); });
    Attempt("Matrix(\"1 2 3\")     ", [&] { Matrix x("1 2 3"); });
    Attempt("Matrix(\"1 x; 3 4\")  ", [&] { Matrix x("1 x; 3 4"); });

    cout << endl << "--- addition ---" << endl;
    Matrix b("1 1 1; 1 1 1; 1 1 1");
    cout << "a + b:" << endl << a + b << endl;
    Matrix c = m4;
    c += b;
    cout << "c = m4; c += b:" << endl << c << endl;
    Attempt("m3 + m4 (2x2 + 3x3) ", [&] { Matrix x = m3 + m4; });
    Matrix big(2, 2000000000);
    Attempt("big + big (overflow)", [&] { Matrix x = big + big; });
    cout << "big after rejected call:" << endl << big << endl;

    cout << endl << "--- comparison ---" << endl;
    cout << "m4 == m5 : " << (m4 == m5) << endl;
    cout << "m4 != m5 : " << (m4 != m5) << endl;
    cout << "m4 == c  : " << (m4 == c) << endl;
    cout << "m3 == m4 : " << (m3 == m4) << "   (different sizes)" << endl;

    cout << endl << "--- norm ---" << endl;
    Matrix unit("1 0; 0 1"), three("3 0; 0 4");
    cout << "norm of identity 2x2  = " << unit.Norm() << "   (sqrt(1 + 1))" << endl;
    cout << "norm of [3 0; 0 4]    = " << three.Norm() << "   (sqrt(9 + 16))" << endl;
    cout << "norm of m4            = " << m4.Norm() << "   (sqrt(285))" << endl;

    cout << endl << "--- type conversion to string ---" << endl;
    string text = three;
    cout << "string(three) = \"" << text << "\"" << endl;
    Matrix again(text);
    cout << "Matrix(string(three)) == three : " << (again == three) << endl;

    cout << endl << "--- input from keyboard ---" << endl;
    Matrix input;
    cout << "Enter a matrix (size, then elements):" << endl;
    while (!(cin >> input))
    {
        if (cin.eof())
        {
            cout << endl << "Input ended." << endl;
            return 0;
        }
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        cout << "Wrong value, try again:" << endl;
    }
    cout << "You entered:" << endl << input << endl;
    cout << "norm = " << input.Norm() << endl;

    return 0;
}
