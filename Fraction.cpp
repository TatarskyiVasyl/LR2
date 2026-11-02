#include "Fraction.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
using namespace std;

int Fraction::Fraction::Pair::counter = 0;

int Fraction::Fraction::Pair::GetCounter()
{
    return counter;
}

void Fraction::Pair::check(double first, double second)
{
    if (!isfinite(first) || !isfinite(second))
        throw invalid_argument("pair elements must be finite numbers");
}

Fraction::Pair::Pair() : first(0), second(0)
{
    counter++;
}

Fraction::Pair::Pair(double first, double second)
{
    check(first, second);
    this->first = first;
    this->second = second;
    counter++;
}

Fraction::Pair::Pair(const Fraction::Pair& other) : first(other.first), second(other.second)
{
    counter++;
}

Fraction::Pair::~Pair()
{
    counter--;
}

double Fraction::Pair::GetFirst() const
{
    return first;
}

double Fraction::Pair::GetSecond() const
{
    return second;
}

void Fraction::Pair::SetFirst(double value)
{
    check(value, second);
    first = value;
}

void Fraction::Pair::SetSecond(double value)
{
    check(first, value);
    second = value;
}

Fraction::Pair& Fraction::Pair::operator = (const Fraction::Pair& other)
{
    first = other.first;
    second = other.second;
    return *this;
}

Fraction::Pair::operator string() const
{
    ostringstream out;
    out.precision(15);
    out << "(first = " << first << ", second = " << second << ")";
    return out.str();
}

Fraction::Pair& Fraction::Pair::operator ++ ()
{
    SetFirst(first + 1);
    return *this;
}

Fraction::Pair& Fraction::Pair::operator -- ()
{
    SetFirst(first - 1);
    return *this;
}

Fraction::Pair Fraction::Pair::operator ++ (int)
{
    Fraction::Pair old(*this);
    SetSecond(second + 1);
    return old;
}

Fraction::Pair Fraction::Pair::operator -- (int)
{
    Fraction::Pair old(*this);
    SetSecond(second - 1);
    return old;
}

bool operator == (const Fraction::Pair& a, const Fraction::Pair& b)
{
    return a.first == b.first && a.second == b.second;
}

bool operator != (const Fraction::Pair& a, const Fraction::Pair& b)
{
    return !(a == b);
}

bool operator > (const Fraction::Pair& a, const Fraction::Pair& b)
{
    return a.first > b.first || (a.first == b.first && a.second > b.second);
}

bool operator < (const Fraction::Pair& a, const Fraction::Pair& b)
{
    return b > a;
}

bool operator >= (const Fraction::Pair& a, const Fraction::Pair& b)
{
    return !(a < b);
}

bool operator <= (const Fraction::Pair& a, const Fraction::Pair& b)
{
    return !(a > b);
}

ostream& operator << (ostream& out, const Fraction::Pair& p)
{
    out << string(p);
    return out;
}

istream& operator >> (istream& in, Fraction::Pair& p)
{
    double a, b;
    cout << "first  = ";
    if (!(in >> a))
        return in;
    cout << "second = ";
    if (!(in >> b))
        return in;
    try
    {
        Fraction::Pair::check(a, b);
    }
    catch (const invalid_argument&)
    {
        in.setstate(ios::failbit);
        return in;
    }
    p.first = a;
    p.second = b;
    return in;
}

namespace
{
    const long long MAX_WHOLE = 1LL << 53;
}

int Fraction::counter = 0;

int Fraction::GetCounter()
{
    return counter;
}

void Fraction::checkWhole(long long whole)
{
    if (whole > MAX_WHOLE || whole < -MAX_WHOLE)
        throw invalid_argument("whole part must not exceed 2^53 by absolute value");
}

void Fraction::checkFrac(double frac)
{
    if (!(frac >= 0.0 && frac < 1.0))
        throw invalid_argument("fractional part must be in range [0; 1)");
}

void Fraction::checkPair(const Fraction::Pair& p)
{
    double whole = p.GetFirst();
    if (whole != floor(whole))
        throw invalid_argument("whole part must be an integer");
    if (fabs(whole) > static_cast<double>(MAX_WHOLE))
        throw invalid_argument("whole part must not exceed 2^53 by absolute value");
    checkFrac(p.GetSecond());
}

Fraction::Fraction() : pair(0, 0)
{
    counter++;
}

Fraction::Fraction(long long whole, double frac)
{
    checkWhole(whole);
    checkFrac(frac);
    pair = Fraction::Pair(static_cast<double>(whole), frac);
    counter++;
}

Fraction::Fraction(const Fraction::Pair& pair)
{
    checkPair(pair);
    this->pair = pair;
    counter++;
}

Fraction::Fraction(double value)
{
    if (!isfinite(value))
        throw invalid_argument("value must be a finite number");
    double whole = floor(value);
    double frac = value - whole;
    if (frac >= 1.0)
    {
        whole += 1.0;
        frac = 0.0;
    }
    Fraction::Pair p(whole, frac);
    checkPair(p);
    pair = p;
    counter++;
}

Fraction::Fraction(const Fraction& other) : pair(other.pair)
{
    counter++;
}

Fraction::~Fraction()
{
    counter--;
}

Fraction::Pair Fraction::GetPair() const
{
    return pair;
}

long long Fraction::GetWhole() const
{
    return static_cast<long long>(pair.GetFirst());
}

double Fraction::GetFrac() const
{
    return pair.GetSecond();
}

void Fraction::SetPair(const Fraction::Pair& p)
{
    checkPair(p);
    pair = p;
}

void Fraction::SetWhole(long long whole)
{
    checkWhole(whole);
    Fraction::Pair t = pair;
    t.SetFirst(static_cast<double>(whole));
    SetPair(t);
}

void Fraction::SetFrac(double frac)
{
    checkFrac(frac);
    Fraction::Pair t = pair;
    t.SetSecond(frac);
    SetPair(t);
}

double Fraction::ToDouble() const
{
    return pair.GetFirst() + pair.GetSecond();
}

Fraction& Fraction::operator = (const Fraction& other)
{
    pair = other.pair;
    return *this;
}

Fraction::operator string() const
{
    ostringstream out;
    out.precision(15);
    out << "Fraction[whole = " << GetWhole() << ", frac = " << GetFrac()
        << ", value = " << ToDouble() << "]";
    return out.str();
}

Fraction& Fraction::operator ++ ()
{
    SetWhole(GetWhole() + 1);
    return *this;
}

Fraction& Fraction::operator -- ()
{
    SetWhole(GetWhole() - 1);
    return *this;
}

Fraction Fraction::operator ++ (int)
{
    Fraction old(*this);
    ++(*this);
    return old;
}

Fraction Fraction::operator -- (int)
{
    Fraction old(*this);
    --(*this);
    return old;
}

bool operator == (const Fraction& a, const Fraction& b) { return a.pair == b.pair; }
bool operator != (const Fraction& a, const Fraction& b) { return a.pair != b.pair; }
bool operator <  (const Fraction& a, const Fraction& b) { return a.pair < b.pair; }
bool operator >  (const Fraction& a, const Fraction& b) { return a.pair > b.pair; }
bool operator <= (const Fraction& a, const Fraction& b) { return a.pair <= b.pair; }
bool operator >= (const Fraction& a, const Fraction& b) { return a.pair >= b.pair; }

ostream& operator << (ostream& out, const Fraction& f)
{
    out << string(f);
    return out;
}

istream& operator >> (istream& in, Fraction& f)
{
    long long whole;
    double frac;
    cout << "whole = ";
    if (!(in >> whole))
        return in;
    cout << "frac  = ";
    if (!(in >> frac))
        return in;
    try
    {
        Fraction t(whole, frac);
        f.pair = t.pair;
    }
    catch (const invalid_argument&)
    {
        in.setstate(ios::failbit);
    }
    return in;
}
