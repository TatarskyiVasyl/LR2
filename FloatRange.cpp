#include "FloatRange.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
using namespace std;

void FloatRange::check(double first, double second)
{
    if (!isfinite(first) || !isfinite(second))
        throw invalid_argument("range borders must be finite numbers");
    if (!(first < second))
        throw invalid_argument("left border must be less than right border");
}

FloatRange::FloatRange() : first(0), second(1)
{
}

FloatRange::FloatRange(double first, double second)
{
    check(first, second);
    this->first = first;
    this->second = second;
}

FloatRange::FloatRange(const FloatRange& other) : first(other.first), second(other.second)
{
}

FloatRange::~FloatRange()
{
}

double FloatRange::GetFirst() const
{
    return first;
}

double FloatRange::GetSecond() const
{
    return second;
}

void FloatRange::SetFirst(double value)
{
    check(value, second);
    first = value;
}

void FloatRange::SetSecond(double value)
{
    check(first, value);
    second = value;
}

bool FloatRange::rangeCheck(double x) const
{
    return first <= x && x <= second;
}

FloatRange& FloatRange::operator = (const FloatRange& other)
{
    first = other.first;
    second = other.second;
    return *this;
}

FloatRange::operator string() const
{
    ostringstream out;
    out.precision(15);
    out << "[" << first << "; " << second << "]";
    return out.str();
}

FloatRange& FloatRange::operator ++ ()
{
    SetFirst(first + 1);
    return *this;
}

FloatRange& FloatRange::operator -- ()
{
    SetFirst(first - 1);
    return *this;
}

FloatRange FloatRange::operator ++ (int)
{
    FloatRange old(*this);
    SetSecond(second + 1);
    return old;
}

FloatRange FloatRange::operator -- (int)
{
    FloatRange old(*this);
    SetSecond(second - 1);
    return old;
}

ostream& operator << (ostream& out, const FloatRange& r)
{
    out << string(r);
    return out;
}

istream& operator >> (istream& in, FloatRange& r)
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
        FloatRange::check(a, b);
    }
    catch (const invalid_argument&)
    {
        in.setstate(ios::failbit);
        return in;
    }
    r.first = a;
    r.second = b;
    return in;
}