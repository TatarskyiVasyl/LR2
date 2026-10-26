#pragma once
#include <initializer_list>
#include <iostream>
#include <string>

class Polinom
{
public:
    static const int MAX_SIZE = 100;

    class Item
    {
        Polinom& owner;
        int      index;
    public:
        Item(Polinom& owner, int index);
        Item(const Item& other);
        Item& operator = (double value);
        Item& operator = (const Item& other);
        Item& operator += (double value);
        Item& operator -= (double value);
        Item& operator *= (double value);
        Item& operator /= (double value);
        operator double() const;
    };

private:
    int    size;
    int    count;
    double coef[MAX_SIZE];

    static void checkSize(int size);
    static void checkValue(double value);
    void        checkIndex(int index) const;
    void        recount();
    void        checkResult() const;
    bool        isZero() const;
    static int  compare(const Polinom& a, const Polinom& b);
    static void divide(const Polinom& a, const Polinom& b, Polinom& quotient, Polinom& remainder);

public:
    Polinom();
    explicit Polinom(int size);
    Polinom(int size, std::initializer_list<double> coefficients);
    explicit Polinom(const std::string& text, int size = MAX_SIZE);
    Polinom(const Polinom& other);
    ~Polinom();

    int    Size() const;
    int    Count() const;
    int    Degree() const;
    double GetCoefficient(int index) const;
    void   SetCoefficient(int index, double value);

    Item   operator [] (int index);
    double operator [] (int index) const;

    Polinom& operator = (const Polinom& other);
    Polinom  operator - () const;
    Polinom& operator += (const Polinom& other);
    Polinom& operator -= (const Polinom& other);
    Polinom& operator *= (const Polinom& other);
    Polinom& operator /= (const Polinom& other);
    Polinom& operator %= (const Polinom& other);
    operator std::string() const;

    double  Value(double x) const;
    double  operator () (double x) const;
    Polinom Derivative() const;
    Polinom Integral(double constant = 0) const;
    double  Integral(double a, double b) const;

    friend Polinom operator + (const Polinom& a, const Polinom& b);
    friend Polinom operator - (const Polinom& a, const Polinom& b);
    friend Polinom operator * (const Polinom& a, const Polinom& b);
    friend Polinom operator / (const Polinom& a, const Polinom& b);
    friend Polinom operator % (const Polinom& a, const Polinom& b);

    friend bool operator == (const Polinom& a, const Polinom& b);
    friend bool operator != (const Polinom& a, const Polinom& b);
    friend bool operator <  (const Polinom& a, const Polinom& b);
    friend bool operator >  (const Polinom& a, const Polinom& b);
    friend bool operator <= (const Polinom& a, const Polinom& b);
    friend bool operator >= (const Polinom& a, const Polinom& b);

    friend std::ostream& operator << (std::ostream& out, const Polinom& p);
    friend std::istream& operator >> (std::istream& in, Polinom& p);
};
