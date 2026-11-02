#include "Pair.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
using namespace std;

void Pair::check(double first, double second)
{
    if (!isfinite(first) || !isfinite(second))
        throw invalid_argument("pair elements must be finite numbers");
}

Pair::Pair() : first(0), second(0)
{
}

Pair::Pair(double first, double second)
{
    check(first, second);
    this->first = first;
    this->second = second;
}

Pair::Pair(const Pair& other) : first(other.first), second(other.second)
{
}

Pair::~Pair()
{
}

double Pair::GetFirst() const
{
    return first;
}

double Pair::GetSecond() const
{
    return second;
}

void Pair::SetFirst(double value)
{
    check(value, second);
    first = value;
}

void Pair::SetSecond(double value)
{
    check(first, value);
    second = value;
}

Pair& Pair::operator = (const Pair& other)
{
    first = other.first;
    second = other.second;
    return *this;
}

Pair::operator string() const
{
    ostringstream out;
    out.precision(15);
    out << "(first = " << first << ", second = " << second << ")";
    return out.str();
}

Pair& Pair::operator ++ ()
{
    SetFirst(first + 1);
    return *this;
}

Pair& Pair::operator -- ()
{
    SetFirst(first - 1);
    return *this;
}

Pair Pair::operator ++ (int)
{
    Pair old(*this);
    SetSecond(second + 1);
    return old;
}

Pair Pair::operator -- (int)
{
    Pair old(*this);
    SetSecond(second - 1);
    return old;
}

bool operator == (const Pair& a, const Pair& b)
{
    return a.first == b.first && a.second == b.second;
}

bool operator != (const Pair& a, const Pair& b)
{
    return !(a == b);
}

bool operator > (const Pair& a, const Pair& b)
{
    return a.first > b.first || (a.first == b.first && a.second > b.second);
}

bool operator < (const Pair& a, const Pair& b)
{
    return b > a;
}

bool operator >= (const Pair& a, const Pair& b)
{
    return !(a < b);
}

bool operator <= (const Pair& a, const Pair& b)
{
    return !(a > b);
}

ostream& operator << (ostream& out, const Pair& p)
{
    out << string(p);
    return out;
}

istream& operator >> (istream& in, Pair& p)
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
        Pair::check(a, b);
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
