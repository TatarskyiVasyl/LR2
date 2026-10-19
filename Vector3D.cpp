#include "Vector3D.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
using namespace std;

void Vector3D::check(double x, double y, double z)
{
    if (!isfinite(x) || !isfinite(y) || !isfinite(z))
        throw invalid_argument("coordinates must be finite numbers");
}

Vector3D::Vector3D() : x(0), y(0), z(0)
{
}

Vector3D::Vector3D(double x, double y, double z)
{
    check(x, y, z);
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3D::Vector3D(const Vector3D& other) : x(other.x), y(other.y), z(other.z)
{
}

Vector3D::~Vector3D()
{
}

double Vector3D::GetX() const { return x; }
double Vector3D::GetY() const { return y; }
double Vector3D::GetZ() const { return z; }

void Vector3D::SetX(double value)
{
    check(value, y, z);
    x = value;
}

void Vector3D::SetY(double value)
{
    check(x, value, z);
    y = value;
}

void Vector3D::SetZ(double value)
{
    check(x, y, value);
    z = value;
}

Vector3D& Vector3D::operator = (const Vector3D& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

Vector3D::operator string() const
{
    ostringstream out;
    out.precision(15);
    out << "(" << x << "; " << y << "; " << z << ")";
    return out.str();
}

Vector3D& Vector3D::operator += (const Vector3D& other)
{
    Vector3D t(x + other.x, y + other.y, z + other.z);
    return *this = t;
}

Vector3D& Vector3D::operator -= (const Vector3D& other)
{
    Vector3D t(x - other.x, y - other.y, z - other.z);
    return *this = t;
}

Vector3D& Vector3D::operator ++ ()
{
    Vector3D t(x + 1, y + 1, z + 1);
    return *this = t;
}

Vector3D& Vector3D::operator -- ()
{
    Vector3D t(x - 1, y - 1, z - 1);
    return *this = t;
}

Vector3D Vector3D::operator ++ (int)
{
    Vector3D old(*this);
    ++(*this);
    return old;
}

Vector3D Vector3D::operator -- (int)
{
    Vector3D old(*this);
    --(*this);
    return old;
}

Vector3D operator + (const Vector3D& a, const Vector3D& b)
{
    return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3D operator - (const Vector3D& a, const Vector3D& b)
{
    return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

double operator * (const Vector3D& a, const Vector3D& b)
{
    double result = a.x * b.x + a.y * b.y + a.z * b.z;
    if (!isfinite(result))
        throw invalid_argument("scalar product is out of range");
    return result;
}

ostream& operator << (ostream& out, const Vector3D& v)
{
    out << string(v);
    return out;
}

istream& operator >> (istream& in, Vector3D& v)
{
    double a, b, c;
    cout << "x = ";
    if (!(in >> a))
        return in;
    cout << "y = ";
    if (!(in >> b))
        return in;
    cout << "z = ";
    if (!(in >> c))
        return in;
    try
    {
        Vector3D::check(a, b, c);
    }
    catch (const invalid_argument&)
    {
        in.setstate(ios::failbit);
        return in;
    }
    v.x = a;
    v.y = b;
    v.z = c;
    return in;
}
