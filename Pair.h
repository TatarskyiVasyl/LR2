#pragma once
#include <iostream>
#include <string>

class Pair
{
private:
    double first;
    double second;

    static void check(double first, double second);

public:
    Pair();
    Pair(double first, double second);
    Pair(const Pair& other);
    ~Pair();

    double GetFirst() const;
    double GetSecond() const;
    void   SetFirst(double value);
    void   SetSecond(double value);

    Pair& operator = (const Pair& other);
    operator std::string() const;

    Pair& operator ++ ();
    Pair& operator -- ();
    Pair  operator ++ (int);
    Pair  operator -- (int);

    friend bool operator == (const Pair& a, const Pair& b);
    friend bool operator != (const Pair& a, const Pair& b);
    friend bool operator <  (const Pair& a, const Pair& b);
    friend bool operator >  (const Pair& a, const Pair& b);
    friend bool operator <= (const Pair& a, const Pair& b);
    friend bool operator >= (const Pair& a, const Pair& b);

    friend std::ostream& operator << (std::ostream& out, const Pair& p);
    friend std::istream& operator >> (std::istream& in, Pair& p);
};
