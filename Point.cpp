#include "Point.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
using namespace std;

void Point::check(double x, double y)
{
    if (!isfinite(x) || !isfinite(y))
        throw invalid_argument("coordinates must be finite numbers");
}

Point::Point() : x(0), y(0)
{
}

Point::Point(double x, double y)
{
    check(x, y);
    this->x = x;
    this->y = y;
}

Point::Point(const Point& other) : x(other.x), y(other.y)
{
}

Point::~Point()
{
}

double Point::GetX() const
{
    return x;
}

double Point::GetY() const
{
    return y;
}

void Point::SetX(double value)
{
    check(value, y);
    x = value;
}

void Point::SetY(double value)
{
    check(x, value);
    y = value;
}

Point& Point::operator = (const Point& other)
{
    x = other.x;
    y = other.y;
    return *this;
}

Point::operator string() const
{
    ostringstream out;
    out.precision(15);
    out << "(" << x << "; " << y << ")";
    return out.str();
}

Point Point::operator + (const Point& other) const
{
    return Point(x + other.x, y + other.y);
}

Point Point::operator + (double number) const
{
    return Point(x + number, y + number);
}

Point operator - (const Point& a, const Point& b)
{
    return Point(a.x - b.x, a.y - b.y);
}

Point operator - (const Point& a, double number)
{
    return Point(a.x - number, a.y - number);
}

bool operator == (const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator != (const Point& a, const Point& b)
{
    return !(a == b);
}

ostream& operator << (ostream& out, const Point& p)
{
    out << string(p);
    return out;
}

istream& operator >> (istream& in, Point& p)
{
    double a, b;
    cout << "x = ";
    if (!(in >> a))
        return in;
    cout << "y = ";
    if (!(in >> b))
        return in;
    try
    {
        Point::check(a, b);
    }
    catch (const invalid_argument&)
    {
        in.setstate(ios::failbit);
        return in;
    }
    p.x = a;
    p.y = b;
    return in;
}
