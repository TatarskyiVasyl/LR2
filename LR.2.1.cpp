#include "FloatRange.h"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

FloatRange makeFloatRange(double first, double second)
{
    try
    {
        return FloatRange(first, second);
    }
    catch (const invalid_argument& e)
    {
        cout << "Error: " << e.what() << " (first = " << first << ", second = " << second << ")" << endl;
        exit(1);
    }
}

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
struct PackedLayout { double first; double second; };
struct PackedMixed { char c; double d; };
#pragma pack(pop)
struct AlignedLayout { double first; double second; };
struct AlignedMixed { char c; double d; };

int main()
{
    cout << boolalpha;

    cout << "--- sizes ---" << endl;
    cout << "sizeof(FloatRange)                 = " << sizeof(FloatRange) << endl;
    cout << "two doubles, default alignment     = " << sizeof(AlignedLayout) << endl;
    cout << "two doubles, #pragma pack(1)       = " << sizeof(PackedLayout) << endl;
    cout << "char + double, default alignment   = " << sizeof(AlignedMixed) << endl;
    cout << "char + double, #pragma pack(1)     = " << sizeof(PackedMixed) << endl;

    cout << endl << "--- creation of objects ---" << endl;
    FloatRange r1;
    FloatRange r2(2.5, 7.5);
    FloatRange r3(r2);
    FloatRange r4 = r2;
    FloatRange r5 = makeFloatRange(-1, 1);
    FloatRange r6;
    r6 = r2;
    cout << "r1 (default)  = " << r1 << endl;
    cout << "r2 (init)     = " << r2 << endl;
    cout << "r3 (copy)     = " << r3 << endl;
    cout << "r4 (= r2)     = " << r4 << endl;
    cout << "r5 (make)     = " << r5 << endl;
    cout << "r6 (assigned) = " << r6 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    FloatRange arr1[3];
    FloatRange arr2[3] = { FloatRange(0, 1), FloatRange(10, 20), FloatRange(-5, 5) };
    FloatRange* p = new FloatRange(100, 200);
    FloatRange* parr = new FloatRange[2];
    parr[0] = arr2[1];
    parr[1] = *p;
    for (int i = 0; i < 3; i++)
        cout << "arr1[" << i << "] = " << arr1[i] << ",  arr2[" << i << "] = " << arr2[i] << endl;
    cout << "*p = " << *p << ",  parr[0] = " << parr[0] << ",  parr[1] = " << parr[1] << endl;
    delete p;
    delete[] parr;

    cout << endl << "--- getters and setters ---" << endl;
    FloatRange s(0, 10);
    cout << "s = " << s << ", GetFirst() = " << s.GetFirst() << ", GetSecond() = " << s.GetSecond() << endl;
    s.SetFirst(2);
    s.SetSecond(8);
    cout << "after SetFirst(2), SetSecond(8): s = " << s << endl;
    Attempt("s.SetFirst(8)       ", [&] { s.SetFirst(8); });
    Attempt("s.SetSecond(1)      ", [&] { s.SetSecond(1); });
    Attempt("s.SetFirst(NaN)     ", [&] { s.SetFirst(numeric_limits<double>::quiet_NaN()); });
    Attempt("FloatRange(5, 5)    ", [&] { FloatRange x(5, 5); });
    Attempt("FloatRange(9, 1)    ", [&] { FloatRange x(9, 1); });
    Attempt("FloatRange(0, inf)  ", [&] { FloatRange x(0, numeric_limits<double>::infinity()); });
    cout << "s after rejected calls = " << s << endl;

    cout << endl << "--- rangeCheck() ---" << endl;
    FloatRange range(2, 8);
    const double values[] = { 1.99, 2, 5, 8, 8.01 };
    for (double v : values)
        cout << v << " in " << range << ": " << range.rangeCheck(v) << endl;

    cout << endl << "--- type conversion to string ---" << endl;
    string text = range;
    cout << "string(range) = \"" << text << "\"" << endl;

    cout << endl << "--- increment and decrement ---" << endl;
    FloatRange t(1, 5), copy;
    cout << "t = " << t << endl;
    copy = ++t;   cout << "copy = ++t : t = " << t << ", copy = " << copy << "   (first + 1)" << endl;
    copy = --t;   cout << "copy = --t : t = " << t << ", copy = " << copy << "   (first - 1)" << endl;
    copy = t++;   cout << "copy = t++ : t = " << t << ", copy = " << copy << "   (second + 1, old value returned)" << endl;
    copy = t--;   cout << "copy = t-- : t = " << t << ", copy = " << copy << "   (second - 1, old value returned)" << endl;
    FloatRange narrow(0, 1);
    Attempt("++narrow (first becomes 1)", [&] { ++narrow; });
    Attempt("narrow-- (second becomes 0)", [&] { narrow--; });
    cout << "narrow after rejected calls = " << narrow << endl;

    cout << endl << "--- input from keyboard ---" << endl;
    FloatRange input;
    cout << "Enter a range (first < second):" << endl;
    while (!(cin >> input))
    {
        if (cin.eof())
        {
            cout << endl << "Input ended." << endl;
            return 0;
        }
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        cout << "Wrong range, try again:" << endl;
    }
    cout << "You entered: " << input << endl;
    double x;
    cout << "Enter a number to check: ";
    if (cin >> x)
        cout << x << (input.rangeCheck(x) ? " belongs to " : " does not belong to ") << input << endl;

    return 0;
}
