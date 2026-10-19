#pragma once
#include <iostream>
#include <string>

class Vector3D
{
private:
    double x;
    double y;
    double z;

    static void check(double x, double y, double z);

public:
    Vector3D();
    Vector3D(double x, double y, double z);
    Vector3D(const Vector3D& other);
    ~Vector3D();

    double GetX() const;
    double GetY() const;
    double GetZ() const;
    void   SetX(double value);
    void   SetY(double value);
    void   SetZ(double value);

    Vector3D& operator = (const Vector3D& other);
    operator std::string() const;

    Vector3D& operator += (const Vector3D& other);
    Vector3D& operator -= (const Vector3D& other);

    Vector3D& operator ++ ();
    Vector3D& operator -- ();
    Vector3D  operator ++ (int);
    Vector3D  operator -- (int);

    friend Vector3D operator + (const Vector3D& a, const Vector3D& b);
    friend Vector3D operator - (const Vector3D& a, const Vector3D& b);
    friend double   operator * (const Vector3D& a, const Vector3D& b);

    friend std::ostream& operator << (std::ostream& out, const Vector3D& v);
    friend std::istream& operator >> (std::istream& in, Vector3D& v);
};
