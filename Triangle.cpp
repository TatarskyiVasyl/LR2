#include "Triangle.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
using namespace std;

namespace
{
    const double kAnglePi = 3.14159265358979323846;
    const int    kMinutesInCircle = 360 * 60;
    const double kTrianglePi = 3.14159265358979323846;
    const double kRelativeTolerance = 1e-9;
    const int    kStraightMinutes = 180 * 60;

    bool isValidSide(double s)
    {
        return isfinite(s) && s >= Triangle::MIN_SIDE && s <= Triangle::MAX_SIDE;
    }

    bool almostEqual(double x, double y)
    {
        return fabs(x - y) <= kRelativeTolerance * max(fabs(x), fabs(y));
    }

    double oppositeSide(double s1, double s2, int angleMinutes)
    {
        double scale = max(s1, s2);
        double x = s1 / scale, y = s2 / scale;
        double half = sin(angleMinutes * (kTrianglePi / (180.0 * 60.0)) / 2.0);
        return scale * sqrt((x - y) * (x - y) + 4.0 * x * y * half * half);
    }

    bool computeAngles(double a, double b, double c, int fixedIndex, int fixedMinutes, int out[3])
    {
        double scale = max(a, max(b, c));
        double x = a / scale, y = b / scale, z = c / scale;

        double cosines[3] = { (y * y + z * z - x * x) / (2 * y * z),
                              (x * x + z * z - y * y) / (2 * x * z),
                              (x * x + y * y - z * z) / (2 * x * y) };

        double exact[3];
        for (int i = 0; i < 3; i++)
        {
            double cs = min(1.0, max(-1.0, cosines[i]));
            exact[i] = acos(cs) * (kStraightMinutes / kTrianglePi);
        }
        if (fixedIndex >= 0)
            exact[fixedIndex] = fixedMinutes;
        else
        {
            int equalIndex = -1, otherIndex = -1;
            if (almostEqual(a, b) && almostEqual(b, c)) { out[0] = out[1] = out[2] = kStraightMinutes / 3; return true; }
            if (almostEqual(a, b)) { equalIndex = 0; otherIndex = 2; }
            else if (almostEqual(b, c)) { equalIndex = 1; otherIndex = 0; }
            else if (almostEqual(a, c)) { equalIndex = 0; otherIndex = 1; }
            if (equalIndex >= 0)
            {
                int base = static_cast<int>(floor(exact[equalIndex] + 0.5));
                int apex = kStraightMinutes - 2 * base;
                if (base < 1 || apex < 1)
                    return false;
                for (int i = 0; i < 3; i++)
                    out[i] = (i == otherIndex) ? apex : base;
                return true;
            }
        }

        int    floors[3];
        double remainder[3];
        int    sum = 0;
        for (int i = 0; i < 3; i++)
        {
            floors[i] = static_cast<int>(floor(exact[i]));
            remainder[i] = exact[i] - floors[i];
            sum += floors[i];
        }

        int deficit = kStraightMinutes - sum;
        int order[3] = { 0, 1, 2 };
        sort(order, order + 3, [&](int p, int q) { return remainder[p] > remainder[q]; });
        for (int k = 0; k < 3 && deficit > 0; k++)
        {
            if (order[k] == fixedIndex)
                continue;
            floors[order[k]]++;
            deficit--;
        }
        if (deficit != 0)
            return false;

        for (int i = 0; i < 3; i++)
        {
            if (floors[i] < 1 || floors[i] > kStraightMinutes - 1)
                return false;
            out[i] = floors[i];
        }
        return true;
    }
}

int Triangle::counter = 0;

int Triangle::GetCounter()
{
    return counter;
}

int Triangle::Triangle::Angle::counter = 0;

int Triangle::Triangle::Angle::GetCounter()
{
    return counter;
}

void Triangle::Angle::check(int grades, int minutes)
{
    if (grades < -MAX_GRADES || grades > MAX_GRADES)
        throw invalid_argument("grades must be in range [-100000; 100000]");
    if (minutes < 0 || minutes > 59)
        throw invalid_argument("minutes must be in range [0; 59]");
}

void Triangle::Angle::setTotal(int total)
{
    if (total < -MAX_GRADES * 60 || total > MAX_GRADES * 60 + 59)
        throw overflow_error("angle is out of range");
    int g = total / 60;
    int m = total % 60;
    if (m < 0)
    {
        m += 60;
        g -= 1;
    }
    grades = g;
    minutes = m;
}

Triangle::Angle::Angle() : grades(0), minutes(0)
{
    counter++;
}

Triangle::Angle::Angle(int grades, int minutes)
{
    check(grades, minutes);
    this->grades = grades;
    this->minutes = minutes;
    counter++;
}

Triangle::Angle::Angle(const Triangle::Angle& other) : grades(other.grades), minutes(other.minutes)
{
    counter++;
}

Triangle::Angle::~Angle()
{
    counter--;
}

int Triangle::Angle::GetGrades() const
{
    return grades;
}

int Triangle::Angle::GetMinutes() const
{
    return minutes;
}

int Triangle::Angle::GetTotalMinutes() const
{
    return grades * 60 + minutes;
}

void Triangle::Angle::SetGrades(int value)
{
    check(value, minutes);
    grades = value;
}

void Triangle::Angle::SetMinutes(int value)
{
    check(grades, value);
    minutes = value;
}

double Triangle::Angle::ToRadians() const
{
    return GetTotalMinutes() * (kAnglePi / (180.0 * 60.0));
}

double Triangle::Angle::ToDegrees() const
{
    return GetTotalMinutes() / 60.0;
}

void Triangle::Angle::Normalize()
{
    int r = GetTotalMinutes() % kMinutesInCircle;
    if (r < 0)
        r += kMinutesInCircle;
    setTotal(r);
}

double Triangle::Angle::Sin() const
{
    int r = GetTotalMinutes() % kMinutesInCircle;
    if (r > kMinutesInCircle / 2)
        r -= kMinutesInCircle;
    else if (r < -kMinutesInCircle / 2)
        r += kMinutesInCircle;
    if (r > kMinutesInCircle / 4)
        r = kMinutesInCircle / 2 - r;
    else if (r < -kMinutesInCircle / 4)
        r = -kMinutesInCircle / 2 - r;
    return sin(r * (kAnglePi / (180.0 * 60.0)));
}

Triangle::Angle& Triangle::Angle::operator = (const Triangle::Angle& other)
{
    grades = other.grades;
    minutes = other.minutes;
    return *this;
}

Triangle::Angle& Triangle::Angle::operator += (const Triangle::Angle& other)
{
    setTotal(GetTotalMinutes() + other.GetTotalMinutes());
    return *this;
}

Triangle::Angle& Triangle::Angle::operator -= (const Triangle::Angle& other)
{
    setTotal(GetTotalMinutes() - other.GetTotalMinutes());
    return *this;
}

Triangle::Angle::operator string() const
{
    int total = GetTotalMinutes();
    ostringstream out;
    if (total < 0)
    {
        out << '-';
        total = -total;
    }
    out << total / 60 << " deg " << total % 60 << " min";
    return out.str();
}

Triangle::Angle& Triangle::Angle::operator ++ ()
{
    setTotal(GetTotalMinutes() + 60);
    return *this;
}

Triangle::Angle& Triangle::Angle::operator -- ()
{
    setTotal(GetTotalMinutes() - 60);
    return *this;
}

Triangle::Angle Triangle::Angle::operator ++ (int)
{
    Triangle::Angle old(*this);
    setTotal(GetTotalMinutes() + 1);
    return old;
}

Triangle::Angle Triangle::Angle::operator -- (int)
{
    Triangle::Angle old(*this);
    setTotal(GetTotalMinutes() - 1);
    return old;
}

Triangle::Angle operator + (const Triangle::Angle& a, const Triangle::Angle& b)
{
    Triangle::Angle result(a);
    result += b;
    return result;
}

Triangle::Angle operator - (const Triangle::Angle& a, const Triangle::Angle& b)
{
    Triangle::Angle result(a);
    result -= b;
    return result;
}

bool operator == (const Triangle::Angle& a, const Triangle::Angle& b) { return a.GetTotalMinutes() == b.GetTotalMinutes(); }
bool operator != (const Triangle::Angle& a, const Triangle::Angle& b) { return a.GetTotalMinutes() != b.GetTotalMinutes(); }
bool operator <  (const Triangle::Angle& a, const Triangle::Angle& b) { return a.GetTotalMinutes() < b.GetTotalMinutes(); }
bool operator >  (const Triangle::Angle& a, const Triangle::Angle& b) { return a.GetTotalMinutes() > b.GetTotalMinutes(); }
bool operator <= (const Triangle::Angle& a, const Triangle::Angle& b) { return a.GetTotalMinutes() <= b.GetTotalMinutes(); }
bool operator >= (const Triangle::Angle& a, const Triangle::Angle& b) { return a.GetTotalMinutes() >= b.GetTotalMinutes(); }

ostream& operator << (ostream& out, const Triangle::Angle& a)
{
    out << string(a);
    return out;
}

istream& operator >> (istream& in, Triangle::Angle& a)
{
    int g, m;
    cout << "grades  = ";
    if (!(in >> g))
        return in;
    cout << "minutes = ";
    if (!(in >> m))
        return in;
    try
    {
        Triangle::Angle::check(g, m);
    }
    catch (const invalid_argument&)
    {
        in.setstate(ios::failbit);
        return in;
    }
    a.grades = g;
    a.minutes = m;
    return in;
}

void Triangle::apply(double newA, double newB, double newC, int fixedIndex, int fixedMinutes)
{
    if (!isValidSide(newA) || !isValidSide(newB) || !isValidSide(newC))
        throw invalid_argument("sides must be finite numbers in range [1e-9; 1e9]");
    if (!(newA < newB + newC && newB < newA + newC && newC < newA + newB))
        throw invalid_argument("each side must be less than the sum of the other two");

    int minutes[3];
    if (!computeAngles(newA, newB, newC, fixedIndex, fixedMinutes, minutes))
        throw invalid_argument("triangle is degenerate: an angle is smaller than 1 minute");

    a = newA;
    b = newB;
    c = newC;
    A = Triangle::Angle(minutes[0] / 60, minutes[0] % 60);
    B = Triangle::Angle(minutes[1] / 60, minutes[1] % 60);
    C = Triangle::Angle(minutes[2] / 60, minutes[2] % 60);
}

Triangle::Triangle() : a(1), b(1), c(1)
{
    apply(1, 1, 1, -1, 0);
    counter++;
}

Triangle::Triangle(double a, double b, double c) : a(1), b(1), c(1)
{
    apply(a, b, c, -1, 0);
    counter++;
}

Triangle::Triangle(const Triangle& other)
    : a(other.a), b(other.b), c(other.c), A(other.A), B(other.B), C(other.C)
{
    counter++;
}

Triangle::~Triangle()
{
    counter--;
}

double Triangle::GetSideA() const { return a; }
double Triangle::GetSideB() const { return b; }
double Triangle::GetSideC() const { return c; }
Triangle::Angle  Triangle::GetAngleA() const { return A; }
Triangle::Angle  Triangle::GetAngleB() const { return B; }
Triangle::Angle  Triangle::GetAngleC() const { return C; }

void Triangle::SetSideA(double value) { apply(value, b, c, -1, 0); }
void Triangle::SetSideB(double value) { apply(a, value, c, -1, 0); }
void Triangle::SetSideC(double value) { apply(a, b, value, -1, 0); }

void Triangle::SetAngleA(const Triangle::Angle& value)
{
    int t = value.GetTotalMinutes();
    if (t < 1 || t > kStraightMinutes - 1)
        throw invalid_argument("angle must be strictly between 0 and 180 degrees");
    apply(oppositeSide(b, c, t), b, c, 0, t);
}

void Triangle::SetAngleB(const Triangle::Angle& value)
{
    int t = value.GetTotalMinutes();
    if (t < 1 || t > kStraightMinutes - 1)
        throw invalid_argument("angle must be strictly between 0 and 180 degrees");
    apply(a, oppositeSide(a, c, t), c, 1, t);
}

void Triangle::SetAngleC(const Triangle::Angle& value)
{
    int t = value.GetTotalMinutes();
    if (t < 1 || t > kStraightMinutes - 1)
        throw invalid_argument("angle must be strictly between 0 and 180 degrees");
    apply(a, b, oppositeSide(a, b, t), 2, t);
}

double Triangle::Area() const
{
    double s[3] = { a, b, c };
    sort(s, s + 3, greater<double>());
    double x = s[0], y = s[1], z = s[2];
    return 0.25 * sqrt((x + (y + z)) * (z - (x - y)) * (z + (x - y)) * (x + (y - z)));
}

double Triangle::Perimeter() const
{
    return a + b + c;
}

double Triangle::HeightA() const { return 2 * Area() / a; }
double Triangle::HeightB() const { return 2 * Area() / b; }
double Triangle::HeightC() const { return 2 * Area() / c; }

bool Triangle::IsEquilateral() const
{
    return almostEqual(a, b) && almostEqual(b, c);
}

bool Triangle::IsIsosceles() const
{
    return almostEqual(a, b) || almostEqual(b, c) || almostEqual(a, c);
}

bool Triangle::IsRight() const
{
    double s[3] = { a, b, c };
    sort(s, s + 3);
    double longest2 = s[2] * s[2];
    return fabs(longest2 - s[0] * s[0] - s[1] * s[1]) <= kRelativeTolerance * longest2;
}

string Triangle::TypeToString() const
{
    if (IsEquilateral())
        return "equilateral";
    if (IsIsosceles())
        return IsRight() ? "isosceles right" : "isosceles";
    return IsRight() ? "right" : "scalene";
}

Triangle& Triangle::operator = (const Triangle& other)
{
    a = other.a;
    b = other.b;
    c = other.c;
    A = other.A;
    B = other.B;
    C = other.C;
    return *this;
}

Triangle::operator string() const
{
    ostringstream out;
    out << "Triangle[a=" << a << ", b=" << b << ", c=" << c
        << ", A=" << string(A) << ", B=" << string(B) << ", C=" << string(C) << "]";
    return out.str();
}

Triangle& Triangle::operator ++ ()
{
    apply(a + 1, b + 1, c + 1, -1, 0);
    return *this;
}

Triangle& Triangle::operator -- ()
{
    apply(a - 1, b - 1, c - 1, -1, 0);
    return *this;
}

Triangle Triangle::operator ++ (int)
{
    Triangle old(*this);
    ++(*this);
    return old;
}

Triangle Triangle::operator -- (int)
{
    Triangle old(*this);
    --(*this);
    return old;
}

ostream& operator << (ostream& out, const Triangle& t)
{
    out << string(t);
    return out;
}

istream& operator >> (istream& in, Triangle& t)
{
    double x, y, z;
    cout << "a = ";
    if (!(in >> x))
        return in;
    cout << "b = ";
    if (!(in >> y))
        return in;
    cout << "c = ";
    if (!(in >> z))
        return in;
    try
    {
        Triangle candidate(x, y, z);
        t = candidate;
    }
    catch (const invalid_argument&)
    {
        in.setstate(ios::failbit);
    }
    return in;
}
