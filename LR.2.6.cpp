#include "Fraction.h"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

using Pair = Fraction::Pair;

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
struct PackedPair { double first; double second; };
struct PackedMixed { char c; double d; };
#pragma pack(pop)
struct AlignedPair { double first; double second; };
struct AlignedMixed { char c; double d; };

int main()
{
    cout << boolalpha;
    cout << "Fraction counter       = " << Fraction::GetCounter() << endl;
    cout << "Fraction::Pair counter = " << Fraction::Pair::GetCounter() << endl;

    cout << "--- sizes ---" << endl;
    cout << "sizeof(Pair)                       = " << sizeof(Pair) << endl;
    cout << "sizeof(Fraction)                   = " << sizeof(Fraction) << "  (contains one Pair)" << endl;
    cout << "two doubles, default alignment     = " << sizeof(AlignedPair) << endl;
    cout << "two doubles, #pragma pack(1)       = " << sizeof(PackedPair) << endl;
    cout << "char + double, default alignment   = " << sizeof(AlignedMixed) << endl;
    cout << "char + double, #pragma pack(1)     = " << sizeof(PackedMixed) << endl;

    cout << endl << "========== class Pair ==========" << endl;

    cout << endl << "--- creation of objects ---" << endl;
    Pair p1;
    Pair p2(2, 0.5);
    Pair p3(p2);
    Pair p4 = p2;
    Pair p5;
    p5 = p2;
    cout << "p1 (default)  = " << p1 << endl;
    cout << "p2 (init)     = " << p2 << endl;
    cout << "p3 (copy)     = " << p3 << endl;
    cout << "p4 (= p2)     = " << p4 << endl;
    cout << "p5 (assigned) = " << p5 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    Pair parr1[2];
    Pair parr2[3] = { Pair(1, 1), Pair(3, 0.5), Pair(3, 0.25) };
    Pair* pp = new Pair(7, 7);
    Pair* pdyn = new Pair[2];
    pdyn[0] = parr2[1];
    pdyn[1] = *pp;
    for (int i = 0; i < 2; i++)
        cout << "parr1[" << i << "] = " << parr1[i] << endl;
    for (int i = 0; i < 3; i++)
        cout << "parr2[" << i << "] = " << parr2[i] << endl;
    cout << "*pp = " << *pp << ",  pdyn[0] = " << pdyn[0] << ",  pdyn[1] = " << pdyn[1] << endl;
    delete pp;
    delete[] pdyn;

    cout << endl << "--- getters and setters ---" << endl;
    Pair s(1, 2);
    cout << "s = " << s << ", GetFirst() = " << s.GetFirst() << ", GetSecond() = " << s.GetSecond() << endl;
    s.SetFirst(10);
    s.SetSecond(-2.5);
    cout << "after SetFirst(10), SetSecond(-2.5): s = " << s << endl;
    Attempt("s.SetFirst(NaN)     ", [&] { s.SetFirst(numeric_limits<double>::quiet_NaN()); });
    Attempt("s.SetSecond(inf)    ", [&] { s.SetSecond(numeric_limits<double>::infinity()); });
    Attempt("Pair(1, inf)        ", [&] { Pair x(1, numeric_limits<double>::infinity()); });
    cout << "s after rejected calls = " << s << endl;

    cout << endl << "--- comparison of pairs ---" << endl;
    Pair a(2, 0.5), b(2, 0.75), c(3, 0), d(2, 0.5);
    cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << endl;
    cout << "a == d : " << (a == d) << endl;
    cout << "a != b : " << (a != b) << endl;
    cout << "a <  b : " << (a < b) << "   (same first, second decides)" << endl;
    cout << "b >  a : " << (b > a) << endl;
    cout << "c >  b : " << (c > b) << "   (first decides)" << endl;
    cout << "a <= d : " << (a <= d) << ",  a >= d : " << (a >= d) << endl;
    cout << "a >= c : " << (a >= c) << endl;
    Pair largest = parr2[0];
    for (int i = 1; i < 3; i++)
        if (parr2[i] > largest)
            largest = parr2[i];
    cout << "largest pair in parr2 = " << largest << endl;

    cout << endl << "--- increment and decrement (prefix: first, postfix: second) ---" << endl;
    Pair t(1, 5), copy;
    cout << "t = " << t << endl;
    copy = ++t;   cout << "copy = ++t : t = " << t << ", copy = " << copy << endl;
    copy = --t;   cout << "copy = --t : t = " << t << ", copy = " << copy << endl;
    copy = t++;   cout << "copy = t++ : t = " << t << ", copy = " << copy << "   (old value returned)" << endl;
    copy = t--;   cout << "copy = t-- : t = " << t << ", copy = " << copy << "   (old value returned)" << endl;

    cout << endl << "--- type conversion to string ---" << endl;
    string text = a;
    cout << "string(a) = \"" << text << "\"" << endl;

    cout << endl << "========== class Fraction ==========" << endl;

    cout << endl << "--- creation of objects ---" << endl;
    Fraction f1;
    Fraction f2(3, 0.25);
    Fraction f3(Pair(-3, 0.75));
    Fraction f4(7.125);
    Fraction f5(f2);
    Fraction f6 = f2;
    Fraction f7;
    f7 = f3;
    cout << "f1 (default)    = " << f1 << endl;
    cout << "f2 (whole, frac)= " << f2 << endl;
    cout << "f3 (from pair)  = " << f3 << endl;
    cout << "f4 (from double)= " << f4 << endl;
    cout << "f5 (copy)       = " << f5 << endl;
    cout << "f7 (assigned)   = " << f7 << endl;

    cout << endl << "--- arrays of objects ---" << endl;
    Fraction farr1[2];
    Fraction farr2[3] = { Fraction(1, 0.5), Fraction(7.125), Fraction(-4, 0.0) };
    Fraction* pf = new Fraction(10, 0.9);
    Fraction* pfa = new Fraction[2];
    pfa[0] = farr2[0];
    pfa[1] = *pf;
    for (int i = 0; i < 2; i++)
        cout << "farr1[" << i << "] = " << farr1[i] << endl;
    for (int i = 0; i < 3; i++)
        cout << "farr2[" << i << "] = " << farr2[i] << endl;
    cout << "*pf = " << *pf << ",  pfa[0] = " << pfa[0] << ",  pfa[1] = " << pfa[1] << endl;
    delete pf;
    delete[] pfa;

    cout << endl << "--- getters and setters ---" << endl;
    Fraction g(4, 0.5);
    cout << "g = " << g << endl;
    cout << "GetWhole() = " << g.GetWhole() << ", GetFrac() = " << g.GetFrac() << ", GetPair() = " << g.GetPair()
        << ", ToDouble() = " << g.ToDouble() << endl;
    g.SetWhole(10);
    g.SetFrac(0.125);
    cout << "after SetWhole(10), SetFrac(0.125): g = " << g << endl;
    g.SetPair(Pair(2, 0.75));
    cout << "after SetPair((2; 0.75)): g = " << g << endl;
    Pair inner = g.GetPair();
    inner.SetFirst(1000.5);
    cout << "g after changing the copy of its pair = " << g << endl;
    Attempt("g.SetFrac(1.0)        ", [&] { g.SetFrac(1.0); });
    Attempt("g.SetFrac(-0.1)       ", [&] { g.SetFrac(-0.1); });
    Attempt("g.SetPair((1.5; 0.2)) ", [&] { g.SetPair(Pair(1.5, 0.2)); });
    Attempt("g.SetWhole(2^60)      ", [&] { g.SetWhole(1LL << 60); });
    Attempt("Fraction(1, NaN)      ", [&] { Fraction x(1, numeric_limits<double>::quiet_NaN()); });
    Attempt("Fraction(inf)         ", [&] { Fraction x(numeric_limits<double>::infinity()); });
    cout << "g after rejected calls = " << g << endl;

    cout << endl << "--- comparison of fractions ---" << endl;
    cout << "f2 = " << f2.ToDouble() << ", f3 = " << f3.ToDouble() << ", f4 = " << f4.ToDouble() << endl;
    cout << "f2 == f5 : " << (f2 == f5) << endl;
    cout << "f2 != f3 : " << (f2 != f3) << endl;
    cout << "f3 <  f2 : " << (f3 < f2) << "   (-2.25 < 3.25)" << endl;
    cout << "f4 >  f2 : " << (f4 > f2) << endl;
    cout << "f2 <= f5 : " << (f2 <= f5) << ",  f2 >= f4 : " << (f2 >= f4) << endl;
    Fraction best = farr2[0];
    for (int i = 1; i < 3; i++)
        if (farr2[i] > best)
            best = farr2[i];
    cout << "largest fraction in farr2 = " << best << endl;

    cout << endl << "--- increment and decrement (both forms change the number by 1) ---" << endl;
    Fraction u(2, 0.5), ucopy;
    cout << "u = " << u << endl;
    ucopy = ++u;   cout << "ucopy = ++u : u = " << u.ToDouble() << ", ucopy = " << ucopy.ToDouble() << endl;
    ucopy = --u;   cout << "ucopy = --u : u = " << u.ToDouble() << ", ucopy = " << ucopy.ToDouble() << endl;
    ucopy = u++;   cout << "ucopy = u++ : u = " << u.ToDouble() << ", ucopy = " << ucopy.ToDouble() << "   (old value returned)" << endl;
    ucopy = u--;   cout << "ucopy = u-- : u = " << u.ToDouble() << ", ucopy = " << ucopy.ToDouble() << "   (old value returned)" << endl;
    Fraction edge(1LL << 53, 0.0);
    Attempt("++edge (whole > 2^53)  ", [&] { ++edge; });
    cout << "edge after rejected call = " << edge << endl;

    cout << endl << "--- type conversion to string ---" << endl;
    string ftext = f4;
    cout << "string(f4) = \"" << ftext << "\"" << endl;

    cout << endl << "--- number of objects ---" << endl;
    cout << "Fraction: " << Fraction::GetCounter() << ",  Fraction::Pair: " << Fraction::Pair::GetCounter()
        << "   (every Fraction contains one Pair)" << endl;
    {
        Fraction local(5, 0.5);
        Pair loosePair(1, 2);
        cout << "in the block (+ Fraction, + Pair): Fraction: " << Fraction::GetCounter()
            << ",  Fraction::Pair: " << Fraction::Pair::GetCounter() << "   (Fraction adds 2 pairs: its own and the loose one)" << endl;
        Fraction copyOfLocal(local);
        cout << "after a copy: Fraction: " << Fraction::GetCounter() << ",  Fraction::Pair: " << Fraction::Pair::GetCounter() << endl;
    }
    cout << "after the block: Fraction: " << Fraction::GetCounter() << ",  Fraction::Pair: " << Fraction::Pair::GetCounter() << endl;
    Fraction* heap = new Fraction(9, 0.25);
    cout << "after new: Fraction: " << Fraction::GetCounter() << ",  Fraction::Pair: " << Fraction::Pair::GetCounter() << endl;
    delete heap;
    cout << "after delete: Fraction: " << Fraction::GetCounter() << ",  Fraction::Pair: " << Fraction::Pair::GetCounter() << endl;
    Attempt("Fraction(1, 2) in a try block", [&] { Fraction bad(1, 2); });
    cout << "after a failed constructor: Fraction: " << Fraction::GetCounter() << ",  Fraction::Pair: " << Fraction::Pair::GetCounter()
        << "   (nothing is leaked)" << endl;

    cout << endl << "--- input from keyboard ---" << endl;
    Pair pinput;
    cout << "Enter a pair:" << endl;
    while (!(cin >> pinput))
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
    cout << "You entered: " << pinput << endl;
    Fraction finput;
    cout << "Enter a fraction (whole part, then fractional part from [0; 1)):" << endl;
    while (!(cin >> finput))
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
    cout << "You entered: " << finput << endl;
    cout << "compared with f2: " << (finput > f2 ? "greater" : (finput == f2 ? "equal" : "less")) << endl;

    return 0;
}
