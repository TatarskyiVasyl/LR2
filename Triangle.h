#pragma once
#include <iostream>
#include <string>

class Triangle
{
public:
    class Angle
    {
    private:
        int grades;
        int minutes;
        static int counter;

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

        static int GetCounter();
    };

private:
    double a, b, c;
    Angle  A, B, C;
    static int counter;

    void apply(double newA, double newB, double newC, int fixedIndex, int fixedMinutes);

public:
    static constexpr double MIN_SIDE = 1e-9;
    static constexpr double MAX_SIDE = 1e9;

    Triangle();
    Triangle(double a, double b, double c);
    Triangle(const Triangle& other);
    ~Triangle();

    double GetSideA() const;
    double GetSideB() const;
    double GetSideC() const;
    Angle  GetAngleA() const;
    Angle  GetAngleB() const;
    Angle  GetAngleC() const;
    void   SetSideA(double value);
    void   SetSideB(double value);
    void   SetSideC(double value);
    void   SetAngleA(const Angle& value);
    void   SetAngleB(const Angle& value);
    void   SetAngleC(const Angle& value);

    double Area() const;
    double Perimeter() const;
    double HeightA() const;
    double HeightB() const;
    double HeightC() const;

    bool        IsEquilateral() const;
    bool        IsIsosceles() const;
    bool        IsRight() const;
    std::string TypeToString() const;

    Triangle& operator = (const Triangle& other);
    operator std::string() const;

    Triangle& operator ++ ();
    Triangle& operator -- ();
    Triangle  operator ++ (int);
    Triangle  operator -- (int);

    friend std::ostream& operator << (std::ostream& out, const Triangle& t);
    friend std::istream& operator >> (std::istream& in, Triangle& t);

    static int GetCounter();
};
