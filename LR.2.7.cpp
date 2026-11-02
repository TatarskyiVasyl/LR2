#include "Triangle.h"
#include <iomanip>
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
struct PackedAngle { int grades; int minutes; };
struct PackedMixed { char c; double d; };
#pragma pack(pop)
struct AlignedAngle { int grades; int minutes; };
struct AlignedMixed { char c; double d; };

static void PrintComparison(const Angle& x, const Angle& y)
{
    cout << x << " vs " << y << ":  ==" << (x == y) << "  !=" << (x != y) << "  <" << (x < y)
        << "  >" << (x > y) << "  <=" << (x <= y) << "  >=" << (x >= y) << endl;
}

int main()
{
    cout << boolalpha;

    cout << "--- sizes ---" << endl;
    cout << "sizeof(Angle)                      = " << sizeof(Angle) << endl;
    cout << "sizeof(Triangle)                   = " << sizeof(Triangle) << "  (3 doubles + 3 Angle objects)" << endl;
    cout << "two ints, default alignment        = " << sizeof(AlignedAngle) << endl;
    cout << "two ints, #pragma pack(1)          = " << sizeof(PackedAngle) << endl;
    cout << "char + double, default alignment   = " << sizeof(AlignedMixed) << endl;
    cout << "char + double, #pragma pack(1)     = " << sizeof(PackedMixed) << endl;

    cout << endl << "========== class Angle ==========" << endl;

    cout << endl << "--- creation of objects ---" << endl;
    Angle a1;
    Angle a2(30, 15);
    Angle a3(45);
    Angle a4(a2);
    Angle a5 = a2;
    Angle a6;
    a6 = a2;
    cout << "a1 (default) = " << a1 << endl;
    cout << "a2 (30, 15)  = " << a2 << endl;
    cout << "a3 (45)      = " << a3 << endl;
    cout << "a4 (copy)    = " << a4 << endl;
    cout << "a6 (assigned)= " << a6 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    Angle arr1[2];
    Angle arr2[3] = { Angle(10, 0), Angle(100, 30), Angle(-5, 20) };
    Angle* pa = new Angle(180);
    Angle* parr = new Angle[2];
    parr[0] = arr2[1];
    parr[1] = *pa;
    for (int i = 0; i < 3; i++)
        cout << "arr2[" << i << "] = " << arr2[i] << endl;
    cout << "*pa = " << *pa << ",  parr[0] = " << parr[0] << ",  parr[1] = " << parr[1] << endl;
    Angle best = arr2[0];
    for (int i = 1; i < 3; i++)
        if (arr2[i] > best)
            best = arr2[i];
    cout << "largest angle in arr2 = " << best << endl;
    delete pa;
    delete[] parr;

    cout << endl << "--- getters and setters ---" << endl;
    Angle s(30, 15);
    cout << "s = " << s << ", GetGrades() = " << s.GetGrades() << ", GetMinutes() = " << s.GetMinutes()
        << ", GetTotalMinutes() = " << s.GetTotalMinutes() << endl;
    s.SetGrades(45);
    s.SetMinutes(59);
    cout << "after SetGrades(45), SetMinutes(59): s = " << s << endl;
    Attempt("s.SetMinutes(60)      ", [&] { s.SetMinutes(60); });
    Attempt("s.SetMinutes(-1)      ", [&] { s.SetMinutes(-1); });
    Attempt("s.SetGrades(100001)   ", [&] { s.SetGrades(100001); });
    Attempt("Angle(10, 60)         ", [&] { Angle x(10, 60); });
    cout << "s after rejected calls = " << s << endl;

    cout << endl << "--- conversion to radians ---" << endl;
    Angle straight(180), right(90), small(0, 30);
    cout << setprecision(10);
    cout << straight << " = " << straight.ToRadians() << " rad (" << straight.ToDegrees() << " deg)" << endl;
    cout << right << "  = " << right.ToRadians() << " rad (" << right.ToDegrees() << " deg)" << endl;
    cout << small << "   = " << small.ToRadians() << " rad (" << small.ToDegrees() << " deg)" << endl;

    cout << endl << "--- reduction to the range 0 - 360 ---" << endl;
    Angle big(725, 10), negative(-1, 30);
    cout << big << " -> ";
    big.Normalize();
    cout << big << endl;
    cout << negative << " -> ";
    negative.Normalize();
    cout << negative << endl;

    cout << endl << "--- increase and decrease by a given value (+=, -=, +, -) ---" << endl;
    Angle x(350, 40), delta(20, 30);
    x += delta;   cout << "x += 20 deg 30 min : x = " << x << endl;
    x -= delta;   cout << "x -= 20 deg 30 min : x = " << x << endl;
    cout << "x + delta = " << x + delta << ",  x - delta = " << x - delta << endl;
    Angle y(0, 10), d2(0, 20);
    y -= d2;      cout << "y = 0 deg 10 min; y -= 0 deg 20 min : y = " << y << "   (angles below zero are allowed)" << endl;
    Angle edge(Angle::MAX_GRADES);
    Attempt("edge += 1 deg         ", [&] { edge += Angle(1); });
    cout << "edge after rejected call = " << edge << endl;

    cout << endl << "--- increment and decrement (prefix: 1 degree, postfix: 1 minute) ---" << endl;
    Angle t(10, 30), tcopy;
    cout << "t = " << t << endl;
    tcopy = ++t;   cout << "tcopy = ++t : t = " << t << ", tcopy = " << tcopy << endl;
    tcopy = --t;   cout << "tcopy = --t : t = " << t << ", tcopy = " << tcopy << endl;
    tcopy = t++;   cout << "tcopy = t++ : t = " << t << ", tcopy = " << tcopy << "   (old value returned)" << endl;
    tcopy = t--;   cout << "tcopy = t-- : t = " << t << ", tcopy = " << tcopy << "   (old value returned)" << endl;
    Angle limit(Angle::MAX_GRADES, 59);
    Attempt("++limit               ", [&] { ++limit; });

    cout << endl << "--- sine ---" << endl;
    const int sample[][2] = { { 0, 0 }, { 30, 0 }, { 45, 0 }, { 90, 0 }, { 180, 0 }, { 270, 0 }, { 390, 0 } };
    for (const auto& v : sample)
    {
        Angle angle(v[0], v[1]);
        cout << "sin(" << angle << ") = " << angle.Sin() << endl;
    }

    cout << endl << "--- comparison ---" << endl;
    PrintComparison(Angle(10, 30), Angle(10, 45));
    PrintComparison(Angle(10, 30), Angle(10, 30));
    PrintComparison(Angle(100, 0), Angle(99, 59));

    cout << endl << "--- type conversion to string ---" << endl;
    string text = a2;
    cout << "string(a2) = \"" << text << "\"" << endl;

    cout << endl << "========== class Triangle ==========" << endl;

    cout << endl << "--- creation of objects ---" << endl;
    Triangle t1;
    Triangle t2(3, 4, 5);
    Triangle t3(t2);
    Triangle t4 = t2;
    Triangle t5;
    t5 = t2;
    cout << "t1 (default) : " << t1 << endl;
    cout << "t2 (3, 4, 5) : " << t2 << endl;
    cout << "t3 (copy)    : " << t3 << endl;
    cout << "t5 (assigned): " << t5 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    Triangle tarr[4] = { Triangle(2, 2, 2), Triangle(5, 5, 6), Triangle(1, 1, 1.4142135623730951), Triangle(4, 5, 6) };
    Triangle tarr0[2];
    Triangle* pt = new Triangle(7, 8, 9);
    Triangle* ptarr = new Triangle[2];
    ptarr[0] = tarr[1];
    ptarr[1] = *pt;
    for (int i = 0; i < 4; i++)
        cout << "tarr[" << i << "]  : " << tarr[i] << endl;
    cout << "tarr0[0]  : " << tarr0[0] << endl;
    cout << "*pt       : " << *pt << ",  ptarr[0]: " << ptarr[0] << endl;
    delete pt;
    delete[] ptarr;

    cout << endl << "--- constructors with wrong values ---" << endl;
    Attempt("Triangle(1, 2, 3)         ", [&] { Triangle x(1, 2, 3); });
    Attempt("Triangle(1, 1, 3)         ", [&] { Triangle x(1, 1, 3); });
    Attempt("Triangle(-1, 2, 2)        ", [&] { Triangle x(-1, 2, 2); });
    Attempt("Triangle(0, 2, 2)         ", [&] { Triangle x(0, 2, 2); });
    Attempt("Triangle(1, 1, 1.99999999)", [&] { Triangle x(1, 1, 1.99999999); });

    cout << endl << "--- getters ---" << endl;
    cout << "sides: " << t2.GetSideA() << ", " << t2.GetSideB() << ", " << t2.GetSideC() << endl;
    cout << "angles: " << t2.GetAngleA() << "; " << t2.GetAngleB() << "; " << t2.GetAngleC() << endl;

    cout << endl << "--- setters ---" << endl;
    Triangle u = t2;
    u.SetSideA(4);
    cout << "u.SetSideA(4)        : " << u << endl;
    Attempt("u.SetSideC(100)       ", [&] { u.SetSideC(100); });
    u.SetAngleC(Angle(90));
    cout << "u.SetAngleC(90 deg)   : " << u << " -> " << u.TypeToString() << endl;
    Attempt("u.SetAngleA(0 deg)    ", [&] { u.SetAngleA(Angle(0)); });
    Attempt("u.SetAngleB(180 deg)  ", [&] { u.SetAngleB(Angle(180)); });
    cout << "u after rejected calls : " << u << endl;

    cout << endl << "--- area, perimeter, heights, type ---" << endl;
    cout << setprecision(8);
    for (int i = 0; i < 4; i++)
    {
        const Triangle& tr = tarr[i];
        cout << "tarr[" << i << "]: " << tr << endl;
        cout << "   area = " << tr.Area() << ", perimeter = " << tr.Perimeter() << endl;
        cout << "   heights: hA = " << tr.HeightA() << ", hB = " << tr.HeightB() << ", hC = " << tr.HeightC() << endl;
        cout << "   equilateral: " << tr.IsEquilateral() << ", isosceles: " << tr.IsIsosceles()
            << ", right: " << tr.IsRight() << " -> " << tr.TypeToString() << endl;
    }

    cout << endl << "--- increment and decrement (every side changes by 1) ---" << endl;
    Triangle w(3, 4, 5), wcopy;
    wcopy = ++w;   cout << "wcopy = ++w : w = " << w.GetSideA() << ", " << w.GetSideB() << ", " << w.GetSideC()
        << "; wcopy sides = " << wcopy.GetSideA() << ", " << wcopy.GetSideB() << ", " << wcopy.GetSideC() << endl;
    wcopy = w--;   cout << "wcopy = w-- : w = " << w.GetSideA() << ", " << w.GetSideB() << ", " << w.GetSideC()
        << "; wcopy sides = " << wcopy.GetSideA() << ", " << wcopy.GetSideB() << ", " << wcopy.GetSideC()
        << "   (old value returned)" << endl;
    Triangle thin(1, 1, 1.5);
    Attempt("--thin (sides 0, 0, 0.5)", [&] { --thin; });
    cout << "thin after rejected call: " << thin << endl;

    cout << endl << "--- type conversion to string ---" << endl;
    string ttext = t2;
    cout << "string(t2) = \"" << ttext << "\"" << endl;

    cout << endl << "--- input from keyboard ---" << endl;
    Angle ainput;
    cout << "Enter an angle (grades, minutes):" << endl;
    while (!(cin >> ainput))
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
    cout << "You entered: " << ainput << ",  sin = " << ainput.Sin() << endl;
    Triangle tinput;
    cout << "Enter the sides of a triangle:" << endl;
    while (!(cin >> tinput))
    {
        if (cin.eof())
        {
            cout << endl << "Input ended." << endl;
            return 0;
        }
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        cout << "Such a triangle does not exist, try again:" << endl;
    }
    cout << "You entered: " << tinput << endl;
    cout << "type: " << tinput.TypeToString() << ", area = " << tinput.Area() << endl;

    return 0;
}
