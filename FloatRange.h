#pragma once
#include <iostream>
#include <string>

class FloatRange
{
private:
    double first;
    double second;

    static void check(double first, double second);

public:
    FloatRange();
    FloatRange(double first, double second);
    FloatRange(const FloatRange& other);
    ~FloatRange();

    double GetFirst() const;
    double GetSecond() const;
    void   SetFirst(double value);
    void   SetSecond(double value);

    bool rangeCheck(double x) const;

    FloatRange& operator = (const FloatRange& other);
    operator std::string() const;

    FloatRange& operator ++ ();
    FloatRange& operator -- ();
    FloatRange  operator ++ (int);
    FloatRange  operator -- (int);

    friend std::ostream& operator << (std::ostream& out, const FloatRange& r);
    friend std::istream& operator >> (std::istream& in, FloatRange& r);
};
