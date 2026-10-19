#include "Vector3D.h"
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
    catch (const invalid_argument& e)
    {
        cout << what << ": rejected (" << e.what() << ")" << endl;
    }
}

#pragma pack(push, 1)
struct PackedLayout { double x; double y; double z; };
struct PackedMixed { char c; double d; };
#pragma pack(pop)
struct AlignedLayout { double x; double y; double z; };
struct AlignedMixed { char c; double d; };

int main()
{
    cout << "--- sizes ---" << endl;
    cout << "sizeof(Vector3D)                   = " << sizeof(Vector3D) << endl;
    cout << "three doubles, default alignment   = " << sizeof(AlignedLayout) << endl;
    cout << "three doubles, #pragma pack(1)     = " << sizeof(PackedLayout) << endl;
    cout << "char + double, default alignment   = " << sizeof(AlignedMixed) << endl;
    cout << "char + double, #pragma pack(1)     = " << sizeof(PackedMixed) << endl;

    cout << endl << "--- creation of objects ---" << endl;
    Vector3D v1;
    Vector3D v2(1, 2, 3);
    Vector3D v3(v2);
    Vector3D v4 = v2;
    Vector3D v5;
    v5 = v2;
    cout << "v1 (default)  = " << v1 << endl;
    cout << "v2 (init)     = " << v2 << endl;
    cout << "v3 (copy)     = " << v3 << endl;
    cout << "v4 (= v2)     = " << v4 << endl;
    cout << "v5 (assigned) = " << v5 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    Vector3D arr1[3];
    Vector3D arr2[3] = { Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1) };
    Vector3D* p = new Vector3D(7, 8, 9);
    Vector3D* parr = new Vector3D[2];
    parr[0] = arr2[1];
    parr[1] = *p;
    for (int i = 0; i < 3; i++)
        cout << "arr1[" << i << "] = " << arr1[i] << ",  arr2[" << i << "] = " << arr2[i] << endl;
    cout << "*p = " << *p << ",  parr[0] = " << parr[0] << ",  parr[1] = " << parr[1] << endl;
    delete p;
    delete[] parr;

    cout << endl << "--- getters and setters ---" << endl;
    Vector3D s(1, 2, 3);
    cout << "s = " << s << ", GetX() = " << s.GetX() << ", GetY() = " << s.GetY() << ", GetZ() = " << s.GetZ() << endl;
    s.SetX(10);
    s.SetY(20);
    s.SetZ(30);
    cout << "after SetX(10), SetY(20), SetZ(30): s = " << s << endl;
    Attempt("s.SetX(NaN)         ", [&] { s.SetX(numeric_limits<double>::quiet_NaN()); });
    Attempt("s.SetZ(inf)         ", [&] { s.SetZ(numeric_limits<double>::infinity()); });
    Attempt("Vector3D(0, inf, 0) ", [&] { Vector3D x(0, numeric_limits<double>::infinity(), 0); });
    cout << "s after rejected calls = " << s << endl;

    cout << endl << "--- addition, subtraction, scalar product ---" << endl;
    Vector3D a(1, 2, 3), b(4, -5, 6);
    cout << "a = " << a << ", b = " << b << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << "   (1*4 + 2*(-5) + 3*6)" << endl;
    Vector3D i(1, 0, 0), j(0, 1, 0);
    cout << "i * j = " << i * j << "   (perpendicular vectors)" << endl;
    cout << "a * a = " << a * a << "   (squared length)" << endl;
    cout << "a + b - b = " << a + b - b << "   (back to a)" << endl;

    cout << endl << "--- += and -= ---" << endl;
    Vector3D c(1, 1, 1);
    c += a;  cout << "c += a : c = " << c << endl;
    c -= b;  cout << "c -= b : c = " << c << endl;
    Vector3D huge(1e308, 0, 0);
    Attempt("huge += huge        ", [&] { huge += huge; });
    cout << "huge after rejected call = " << huge << endl;

    cout << endl << "--- increment and decrement ---" << endl;
    Vector3D t(1, 2, 3), copy;
    cout << "t = " << t << endl;
    copy = ++t;   cout << "copy = ++t : t = " << t << ", copy = " << copy << "   (new value returned)" << endl;
    copy = --t;   cout << "copy = --t : t = " << t << ", copy = " << copy << "   (new value returned)" << endl;
    copy = t++;   cout << "copy = t++ : t = " << t << ", copy = " << copy << "   (old value returned)" << endl;
    copy = t--;   cout << "copy = t-- : t = " << t << ", copy = " << copy << "   (old value returned)" << endl;

    cout << endl << "--- type conversion to string ---" << endl;
    string text = a + b;
    cout << "string(a + b) = \"" << text << "\"" << endl;

    cout << endl << "--- input from keyboard ---" << endl;
    Vector3D input;
    cout << "Enter a vector:" << endl;
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
    cout << "You entered: " << input << endl;
    cout << "input + a = " << input + a << ", input - a = " << input - a << endl;
    cout << "input * a = " << input * a << endl;

    return 0;
}
