#pragma once
#include <iostream>
#include <string>

class Point
{
private:
    double x;
    double y;

    static void check(double x, double y);

public:
    Point();
    Point(double x, double y);
    Point(const Point& other);
    ~Point();

    double GetX() const;
    double GetY() const;
    void   SetX(double value);
    void   SetY(double value);

    Point& operator = (const Point& other);
    operator std::string() const;

    Point operator + (const Point& other) const;
    Point operator + (double number) const;

    friend Point operator - (const Point& a, const Point& b);
    friend Point operator - (const Point& a, double number);

    friend bool operator == (const Point& a, const Point& b);
    friend bool operator != (const Point& a, const Point& b);

    friend std::ostream& operator << (std::ostream& out, const Point& p);
    friend std::istream& operator >> (std::istream& in, Point& p);
};
