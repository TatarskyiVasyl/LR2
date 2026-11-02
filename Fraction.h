#pragma once
#include <iostream>
#include <string>

class Fraction
{
public:
    class Pair
    {
    private:
        double first;
        double second;
        static int counter;

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

        static int GetCounter();
    };

private:
    Pair pair;
    static int counter;

    static void checkWhole(long long whole);
    static void checkFrac(double frac);
    static void checkPair(const Pair& p);

public:
    Fraction();
    Fraction(long long whole, double frac);
    explicit Fraction(const Pair& pair);
    explicit Fraction(double value);
    Fraction(const Fraction& other);
    ~Fraction();

    Pair      GetPair() const;
    long long GetWhole() const;
    double    GetFrac() const;
    void      SetPair(const Pair& pair);
    void      SetWhole(long long whole);
    void      SetFrac(double frac);

    double ToDouble() const;

    Fraction& operator = (const Fraction& other);
    operator std::string() const;

    Fraction& operator ++ ();
    Fraction& operator -- ();
    Fraction  operator ++ (int);
    Fraction  operator -- (int);

    friend bool operator == (const Fraction& a, const Fraction& b);
    friend bool operator != (const Fraction& a, const Fraction& b);
    friend bool operator <  (const Fraction& a, const Fraction& b);
    friend bool operator >  (const Fraction& a, const Fraction& b);
    friend bool operator <= (const Fraction& a, const Fraction& b);
    friend bool operator >= (const Fraction& a, const Fraction& b);

    friend std::ostream& operator << (std::ostream& out, const Fraction& f);
    friend std::istream& operator >> (std::istream& in, Fraction& f);

    static int GetCounter();
};