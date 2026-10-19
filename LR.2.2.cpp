#include "Point.h"
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
struct PackedLayout { double x; double y; };
struct PackedMixed { char c; double d; };
#pragma pack(pop)
struct AlignedLayout { double x; double y; };
struct AlignedMixed { char c; double d; };

int main()
{
    cout << boolalpha;

    cout << "--- sizes ---" << endl;
    cout << "sizeof(Point)                      = " << sizeof(Point) << endl;
    cout << "two doubles, default alignment     = " << sizeof(AlignedLayout) << endl;
    cout << "two doubles, #pragma pack(1)       = " << sizeof(PackedLayout) << endl;
    cout << "char + double, default alignment   = " << sizeof(AlignedMixed) << endl;
    cout << "char + double, #pragma pack(1)     = " << sizeof(PackedMixed) << endl;

    cout << endl << "--- creation of objects ---" << endl;
    Point p1;
    Point p2(3, 4);
    Point p3(p2);
    Point p4 = p2;
    Point p5;
    p5 = p2;
    cout << "p1 (default)  = " << p1 << endl;
    cout << "p2 (init)     = " << p2 << endl;
    cout << "p3 (copy)     = " << p3 << endl;
    cout << "p4 (= p2)     = " << p4 << endl;
    cout << "p5 (assigned) = " << p5 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    Point arr1[3];
    Point arr2[3] = { Point(1, 2), Point(-3, 0.5), Point(10, -10) };
    Point* p = new Point(7, 8);
    Point* parr = new Point[2];
    parr[0] = arr2[2];
    parr[1] = *p;
    for (int i = 0; i < 3; i++)
        cout << "arr1[" << i << "] = " << arr1[i] << ",  arr2[" << i << "] = " << arr2[i] << endl;
    cout << "*p = " << *p << ",  parr[0] = " << parr[0] << ",  parr[1] = " << parr[1] << endl;
    delete p;
    delete[] parr;

    cout << endl << "--- getters and setters ---" << endl;
    Point s(1, 2);
    cout << "s = " << s << ", GetX() = " << s.GetX() << ", GetY() = " << s.GetY() << endl;
    s.SetX(10);
    s.SetY(20);
    cout << "after SetX(10), SetY(20): s = " << s << endl;
    Attempt("s.SetX(NaN)         ", [&] { s.SetX(numeric_limits<double>::quiet_NaN()); });
    Attempt("s.SetY(inf)         ", [&] { s.SetY(numeric_limits<double>::infinity()); });
    Attempt("Point(inf, 0)       ", [&] { Point x(numeric_limits<double>::infinity(), 0); });
    cout << "s after rejected calls = " << s << endl;

    cout << endl << "--- addition (method of the class) ---" << endl;
    Point a(1, 2), b(10, 20);
    cout << "a = " << a << ", b = " << b << endl;
    cout << "a + b   = " << a + b << endl;
    cout << "a + 5   = " << a + 5 << endl;
    cout << "a + b + 0.5 = " << a + b + 0.5 << endl;

    cout << endl << "--- subtraction (friend function) ---" << endl;
    cout << "b - a   = " << b - a << endl;
    cout << "b - 5   = " << b - 5 << endl;
    cout << "a - b - 1 = " << a - b - 1 << endl;

    cout << endl << "--- comparison ---" << endl;
    Point c(1, 2);
    cout << "a == c : " << (a == c) << endl;
    cout << "a != c : " << (a != c) << endl;
    cout << "a == b : " << (a == b) << endl;
    cout << "a != b : " << (a != b) << endl;
    cout << "(a + b) - b == a : " << ((a + b) - b == a) << endl;

    cout << endl << "--- overflow is detected ---" << endl;
    Point big(1.5e308, 0);
    Attempt("big + big           ", [&] { Point r = big + big; });
    Attempt("-big - big          ", [&] { Point r = Point(-1.5e308, 0) - big; });

    cout << endl << "--- type conversion to string ---" << endl;
    string text = a + b;
    cout << "string(a + b) = \"" << text << "\"" << endl;

    cout << endl << "--- input from keyboard ---" << endl;
    Point input;
    cout << "Enter a point:" << endl;
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
    cout << "input == a : " << (input == a) << endl;

    return 0;
}
