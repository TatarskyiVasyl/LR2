#include "Polinom.h"
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
struct PackedLayout { int size; int count; double coef[100]; };
struct PackedMixed { char c; double d; };
#pragma pack(pop)
struct AlignedLayout { int size; int count; double coef[100]; };
struct AlignedMixed { char c; double d; };

int main()
{
    cout << boolalpha;
    const int maxSize = Polinom::MAX_SIZE;

    cout << "--- sizes ---" << endl;
    cout << "MAX_SIZE                           = " << maxSize << endl;
    cout << "sizeof(Polinom)                    = " << sizeof(Polinom) << "  (the whole array is inside the object)" << endl;
    cout << "same fields, default alignment     = " << sizeof(AlignedLayout) << endl;
    cout << "same fields, #pragma pack(1)       = " << sizeof(PackedLayout) << endl;
    cout << "char + double, default alignment   = " << sizeof(AlignedMixed) << endl;
    cout << "char + double, #pragma pack(1)     = " << sizeof(PackedMixed) << endl;

    cout << endl << "--- creation of objects ---" << endl;
    Polinom p1;
    Polinom p2(5);
    Polinom p3(5, { 1, -2, 3 });
    Polinom p4("x^3 - 4x + 2.5", 10);
    Polinom p5(p3);
    Polinom p6 = p3;
    Polinom p7;
    p7 = p4;
    cout << "p1 (default)     : " << p1 << "   size = " << p1.Size() << ", count = " << p1.Count() << endl;
    cout << "p2 (size 5)      : " << p2 << "   size = " << p2.Size() << ", count = " << p2.Count() << endl;
    cout << "p3 (init list)   : " << p3 << "   size = " << p3.Size() << ", count = " << p3.Count() << ", degree = " << p3.Degree() << endl;
    cout << "p4 (from string) : " << p4 << "   size = " << p4.Size() << ", count = " << p4.Count() << endl;
    cout << "p5 (copy of p3)  : " << p5 << endl;
    cout << "p7 (assigned)    : " << p7 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    Polinom arr1[2];
    Polinom arr2[3] = { Polinom("x", 4), Polinom(3, { 5 }), Polinom("x^2 + 1", 6) };
    Polinom* p = new Polinom("2x - 7", 3);
    Polinom* parr = new Polinom[2];
    parr[0] = arr2[2];
    parr[1] = *p;
    cout << "arr1[0] = " << arr1[0] << ",  arr2 = " << arr2[0] << " | " << arr2[1] << " | " << arr2[2] << endl;
    cout << "*p = " << *p << ",  parr[0] = " << parr[0] << ",  parr[1] = " << parr[1] << endl;
    delete p;
    delete[] parr;

    cout << endl << "--- getters, setters, indexing ---" << endl;
    Polinom a(6);
    a.SetCoefficient(0, 4);
    a[1] = -3;
    a[2] = 2;
    cout << "a = " << a << ", count = " << a.Count() << endl;
    cout << "GetCoefficient(2) = " << a.GetCoefficient(2) << ", a[1] = " << a[1] << endl;
    a[2] += 1;
    a[4] = 5;
    cout << "after a[2] += 1, a[4] = 5: a = " << a << ", count = " << a.Count() << endl;
    a[4] = 0;
    cout << "after a[4] = 0:            a = " << a << ", count = " << a.Count() << endl;
    Attempt("a[6] = 1            ", [&] { a[6] = 1; });
    Attempt("a.GetCoefficient(-1)", [&] { a.GetCoefficient(-1); });
    Attempt("a[1] = NaN          ", [&] { a[1] = numeric_limits<double>::quiet_NaN(); });
    Attempt("a.SetCoefficient(0, inf)", [&] { a.SetCoefficient(0, numeric_limits<double>::infinity()); });
    Attempt("Polinom(0)          ", [&] { Polinom x(0); });
    Attempt("Polinom(101)        ", [&] { Polinom x(maxSize + 1); });
    Attempt("Polinom(2, {1,2,3}) ", [&] { Polinom x(2, { 1, 2, 3 }); });
    Attempt("Polinom(\"x^5\", 5)   ", [&] { Polinom x("x^5", 5); });
    Attempt("Polinom(\"3x 2\")     ", [&] { Polinom x("3x 2"); });
    Attempt("Polinom(\"\")         ", [&] { Polinom x(""); });
    cout << "a after rejected calls = " << a << endl;

    cout << endl << "--- type conversion to string ---" << endl;
    string text = p3;
    cout << "string(p3) = \"" << text << "\"" << endl;
    Polinom again(text, 5);
    cout << "Polinom(string(p3)) == p3 : " << (again == p3) << endl;
    cout << "-x^2 - 1   : " << Polinom("-x^2 - 1", 4) << endl;
    cout << "0.5x + 0.25: " << Polinom("0.5x+0.25", 4) << endl;

    cout << endl << "--- value for given x, operator () ---" << endl;
    cout << "p3 = " << p3 << endl;
    for (double x : { 0.0, 1.0, 2.0, -1.0 })
        cout << "p3(" << x << ") = " << p3.Value(x) << endl;
    cout << "p3(10) via operator () = " << p3(10) << endl;

    cout << endl << "--- differentiation and integration ---" << endl;
    cout << "p3             = " << p3 << endl;
    cout << "p3'            = " << p3.Derivative() << endl;
    cout << "p3''           = " << p3.Derivative().Derivative() << endl;
    cout << "integral of p3 = " << p3.Integral() << " (constant 0)" << endl;
    cout << "integral of p3 = " << p3.Integral(7) << " (constant 7)" << endl;
    cout << "integral of p3 from 0 to 2 = " << p3.Integral(0, 2) << "   (8 - 4 + 2)" << endl;
    cout << "derivative of integral == p3 : " << (p3.Integral(5).Derivative() == p3) << endl;

    cout << endl << "--- arithmetic ---" << endl;
    Polinom b("x - 1", 4), c("x^2 + 2x + 1", 4);
    cout << "b = " << b << ",  c = " << c << endl;
    cout << "b + c = " << b + c << ",  size = " << (b + c).Size() << endl;
    cout << "b - c = " << b - c << endl;
    cout << "b * c = " << b * c << ",  size = " << (b * c).Size() << endl;
    cout << "-b    = " << -b << endl;
    Polinom d("x^3 + 2x^2 - x + 5", 6), e("x^2 + 1", 3);
    cout << "d = " << d << ",  e = " << e << endl;
    cout << "d / e = " << d / e << "   (quotient)" << endl;
    cout << "d % e = " << d % e << "   (remainder)" << endl;
    cout << "(d / e) * e + d % e == d : " << ((d / e) * e + d % e == d) << endl;
    Polinom t = b;
    t += c;   cout << "t = b; t += c : " << t << endl;
    t -= c;   cout << "t -= c        : " << t << endl;
    t *= c;   cout << "t *= c        : " << t << endl;
    t /= c;   cout << "t /= c        : " << t << endl;
    Attempt("d / zero            ", [&] { Polinom x = d / Polinom(2); });
    Polinom high(maxSize);
    high[maxSize - 1] = 1;
    Attempt("x^99 * x^99 (degree overflow)", [&] { Polinom x = high * high; });
    Attempt("x^99 integral (no room)", [&] { Polinom x = high.Integral(); });

    cout << endl << "--- comparison ---" << endl;
    Polinom q1("x^2 + 1", 4), q2("x^2 + 1", 9), q3("x^2 + 2", 4), q4("x^3", 4), q5("-5x^2", 4);
    cout << "q1 == q2 : " << (q1 == q2) << "   (equal even with different size)" << endl;
    cout << "q1 != q3 : " << (q1 != q3) << endl;
    cout << "q1 <  q3 : " << (q1 < q3) << endl;
    cout << "q3 >  q1 : " << (q3 > q1) << endl;
    cout << "q1 <= q2 : " << (q1 <= q2) << endl;
    cout << "q1 >= q3 : " << (q1 >= q3) << endl;
    cout << "q4 >  q3 : " << (q4 > q3) << "   (higher degree)" << endl;
    cout << "q5 <  q3 : " << (q5 < q3) << "   (-5x^2 < x^2 + 2)" << endl;

    cout << endl << "--- input from keyboard ---" << endl;
    Polinom input(3);
    cout << "Enter a polynomial (size, then coefficients from degree 0):" << endl;
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
    cout << "input' = " << input.Derivative() << ",  integral = " << input.Integral() << endl;
    double x;
    cout << "Enter x: ";
    if (cin >> x)
        cout << "input(" << x << ") = " << input(x) << endl;

    return 0;
}
