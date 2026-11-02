#pragma once
#include <iostream>
#include <string>

class Angle
{
private:
    int grades;
    int minutes;

    static void check(int grades, int minutes);
    void        setTotal(int total);

public:
    static const int MAX_GRADES = 100000;

    Angle();
    explicit Angle(int grades, int minutes = 0);
    Angle(const Angle& other);
    ~Angle();

    int  GetGrades() const;
    int  GetMinutes() const;
    int  GetTotalMinutes() const;
    void SetGrades(int value);
    void SetMinutes(int value);

    double ToRadians() const;
    double ToDegrees() const;
    void   Normalize();
    double Sin() const;

    Angle& operator = (const Angle& other);
    Angle& operator += (const Angle& other);
    Angle& operator -= (const Angle& other);
    operator std::string() const;

    Angle& operator ++ ();
    Angle& operator -- ();
    Angle  operator ++ (int);
    Angle  operator -- (int);

    friend Angle operator + (const Angle& a, const Angle& b);
    friend Angle operator - (const Angle& a, const Angle& b);

    friend bool operator == (const Angle& a, const Angle& b);
    friend bool operator != (const Angle& a, const Angle& b);
    friend bool operator <  (const Angle& a, const Angle& b);
    friend bool operator >  (const Angle& a, const Angle& b);
    friend bool operator <= (const Angle& a, const Angle& b);
    friend bool operator >= (const Angle& a, const Angle& b);

    friend std::ostream& operator << (std::ostream& out, const Angle& a);
    friend std::istream& operator >> (std::istream& in, Angle& a);
};
