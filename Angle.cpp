#include "Angle.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
using namespace std;

namespace
{
    const double kAnglePi = 3.14159265358979323846;
    const int    kMinutesInCircle = 360 * 60;
}

void Angle::check(int grades, int minutes)
{
    if (grades < -MAX_GRADES || grades > MAX_GRADES)
        throw invalid_argument("grades must be in range [-100000; 100000]");
    if (minutes < 0 || minutes > 59)
        throw invalid_argument("minutes must be in range [0; 59]");
}

void Angle::setTotal(int total)
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

Angle::Angle() : grades(0), minutes(0)
{
}

Angle::Angle(int grades, int minutes)
{
    check(grades, minutes);
    this->grades = grades;
    this->minutes = minutes;
}

Angle::Angle(const Angle& other) : grades(other.grades), minutes(other.minutes)
{
}

Angle::~Angle()
{
}

int Angle::GetGrades() const
{
    return grades;
}

int Angle::GetMinutes() const
{
    return minutes;
}

int Angle::GetTotalMinutes() const
{
    return grades * 60 + minutes;
}

void Angle::SetGrades(int value)
{
    check(value, minutes);
    grades = value;
}

void Angle::SetMinutes(int value)
{
    check(grades, value);
    minutes = value;
}

double Angle::ToRadians() const
{
    return GetTotalMinutes() * (kAnglePi / (180.0 * 60.0));
}

double Angle::ToDegrees() const
{
    return GetTotalMinutes() / 60.0;
}

void Angle::Normalize()
{
    int r = GetTotalMinutes() % kMinutesInCircle;
    if (r < 0)
        r += kMinutesInCircle;
    setTotal(r);
}

double Angle::Sin() const
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

Angle& Angle::operator = (const Angle& other)
{
    grades = other.grades;
    minutes = other.minutes;
    return *this;
}

Angle& Angle::operator += (const Angle& other)
{
    setTotal(GetTotalMinutes() + other.GetTotalMinutes());
    return *this;
}

Angle& Angle::operator -= (const Angle& other)
{
    setTotal(GetTotalMinutes() - other.GetTotalMinutes());
    return *this;
}

Angle::operator string() const
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

Angle& Angle::operator ++ ()
{
    setTotal(GetTotalMinutes() + 60);
    return *this;
}

Angle& Angle::operator -- ()
{
    setTotal(GetTotalMinutes() - 60);
    return *this;
}

Angle Angle::operator ++ (int)
{
    Angle old(*this);
    setTotal(GetTotalMinutes() + 1);
    return old;
}

Angle Angle::operator -- (int)
{
    Angle old(*this);
    setTotal(GetTotalMinutes() - 1);
    return old;
}

Angle operator + (const Angle& a, const Angle& b)
{
    Angle result(a);
    result += b;
    return result;
}

Angle operator - (const Angle& a, const Angle& b)
{
    Angle result(a);
    result -= b;
    return result;
}

bool operator == (const Angle& a, const Angle& b) { return a.GetTotalMinutes() == b.GetTotalMinutes(); }
bool operator != (const Angle& a, const Angle& b) { return a.GetTotalMinutes() != b.GetTotalMinutes(); }
bool operator <  (const Angle& a, const Angle& b) { return a.GetTotalMinutes() < b.GetTotalMinutes(); }
bool operator >  (const Angle& a, const Angle& b) { return a.GetTotalMinutes() > b.GetTotalMinutes(); }
bool operator <= (const Angle& a, const Angle& b) { return a.GetTotalMinutes() <= b.GetTotalMinutes(); }
bool operator >= (const Angle& a, const Angle& b) { return a.GetTotalMinutes() >= b.GetTotalMinutes(); }

ostream& operator << (ostream& out, const Angle& a)
{
    out << string(a);
    return out;
}

istream& operator >> (istream& in, Angle& a)
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
        Angle::check(g, m);
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
