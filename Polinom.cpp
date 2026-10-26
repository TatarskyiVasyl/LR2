#include "Polinom.h"
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
using namespace std;

namespace
{
    bool isDigit(char c) { return c >= '0' && c <= '9'; }

    void skipSpaces(const string& s, size_t& pos)
    {
        while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t'))
            pos++;
    }

    double scanNumber(const string& s, size_t& pos)
    {
        size_t start = pos;
        while (pos < s.size() && isDigit(s[pos]))
            pos++;
        if (pos < s.size() && s[pos] == '.')
        {
            pos++;
            while (pos < s.size() && isDigit(s[pos]))
                pos++;
        }
        size_t digits = pos - start - ((s.substr(start, pos - start).find('.') != string::npos) ? 1 : 0);
        if (digits == 0)
            throw invalid_argument("number expected in polynomial text");
        if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E'))
        {
            size_t p = pos + 1;
            if (p < s.size() && (s[p] == '+' || s[p] == '-'))
                p++;
            if (p < s.size() && isDigit(s[p]))
            {
                while (p < s.size() && isDigit(s[p]))
                    p++;
                pos = p;
            }
        }
        double value = strtod(s.substr(start, pos - start).c_str(), nullptr);
        if (!isfinite(value))
            throw invalid_argument("number in polynomial text is out of range");
        return value;
    }
}

Polinom::Item::Item(Polinom& owner, int index) : owner(owner), index(index)
{
}

Polinom::Item::Item(const Item& other) : owner(other.owner), index(other.index)
{
}

Polinom::Item& Polinom::Item::operator = (double value)
{
    owner.SetCoefficient(index, value);
    return *this;
}

Polinom::Item& Polinom::Item::operator = (const Item& other)
{
    return *this = static_cast<double>(other);
}

Polinom::Item& Polinom::Item::operator += (double value) { return *this = owner.coef[index] + value; }
Polinom::Item& Polinom::Item::operator -= (double value) { return *this = owner.coef[index] - value; }
Polinom::Item& Polinom::Item::operator *= (double value) { return *this = owner.coef[index] * value; }
Polinom::Item& Polinom::Item::operator /= (double value) { return *this = owner.coef[index] / value; }

Polinom::Item::operator double() const
{
    return owner.coef[index];
}

void Polinom::checkSize(int size)
{
    if (size < 1 || size > MAX_SIZE)
        throw invalid_argument("polynomial size must be in range [1; 100]");
}

void Polinom::checkValue(double value)
{
    if (!isfinite(value))
        throw invalid_argument("coefficient must be a finite number");
}

void Polinom::checkIndex(int index) const
{
    if (index < 0 || index >= size)
        throw out_of_range("coefficient index is out of range");
}

void Polinom::recount()
{
    count = 1;
    for (int i = size - 1; i > 0; i--)
        if (coef[i] != 0)
        {
            count = i + 1;
            break;
        }
}

void Polinom::checkResult() const
{
    for (int i = 0; i < size; i++)
        if (!isfinite(coef[i]))
            throw overflow_error("coefficient is out of range of double");
}

bool Polinom::isZero() const
{
    return count == 1 && coef[0] == 0;
}

int Polinom::compare(const Polinom& a, const Polinom& b)
{
    int top = a.count > b.count ? a.count : b.count;
    for (int i = top - 1; i >= 0; i--)
    {
        double x = i < a.count ? a.coef[i] : 0;
        double y = i < b.count ? b.coef[i] : 0;
        if (x < y) return -1;
        if (x > y) return 1;
    }
    return 0;
}

void Polinom::divide(const Polinom& a, const Polinom& b, Polinom& quotient, Polinom& remainder)
{
    if (b.isZero())
        throw invalid_argument("division by zero polynomial");

    double work[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++)
        work[i] = i < a.size ? a.coef[i] : 0;

    Polinom q(a.size);
    int shiftMax = a.count - b.count;
    double lead = b.coef[b.count - 1];
    for (int shift = shiftMax; shift >= 0; shift--)
    {
        double factor = work[shift + b.count - 1] / lead;
        q.coef[shift] = factor;
        for (int k = 0; k < b.count; k++)
            work[shift + k] -= factor * b.coef[k];
        work[shift + b.count - 1] = 0;
    }

    Polinom r(b.size);
    int remainderLength = b.count - 1 < a.count ? b.count - 1 : a.count;
    for (int i = 0; i < remainderLength; i++)
        r.coef[i] = work[i];

    q.recount();
    r.recount();
    q.checkResult();
    r.checkResult();
    quotient = q;
    remainder = r;
}

Polinom::Polinom() : size(MAX_SIZE), count(1)
{
    for (int i = 0; i < MAX_SIZE; i++)
        coef[i] = 0;
}

Polinom::Polinom(int size)
{
    checkSize(size);
    this->size = size;
    count = 1;
    for (int i = 0; i < MAX_SIZE; i++)
        coef[i] = 0;
}

Polinom::Polinom(int size, initializer_list<double> coefficients)
{
    checkSize(size);
    if (static_cast<int>(coefficients.size()) > size)
        throw invalid_argument("too many coefficients for the given size");
    for (double c : coefficients)
        checkValue(c);
    this->size = size;
    int i = 0;
    for (double c : coefficients)
        coef[i++] = c;
    for (; i < MAX_SIZE; i++)
        coef[i] = 0;
    recount();
}

Polinom::Polinom(const string& text, int size)
{
    checkSize(size);
    double sum[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++)
        sum[i] = 0;

    size_t pos = 0;
    bool anyTerm = false;
    skipSpaces(text, pos);
    while (pos < text.size())
    {
        int sign = 1;
        bool hasSign = false;
        while (pos < text.size() && (text[pos] == '+' || text[pos] == '-'))
        {
            if (text[pos] == '-')
                sign = -sign;
            hasSign = true;
            pos++;
            skipSpaces(text, pos);
        }
        if (anyTerm && !hasSign)
            throw invalid_argument("sign + or - expected between terms");

        double c = 1;
        bool hasNumber = false;
        if (pos < text.size() && (isDigit(text[pos]) || text[pos] == '.'))
        {
            c = scanNumber(text, pos);
            hasNumber = true;
            skipSpaces(text, pos);
        }

        int degree = 0;
        if (pos < text.size() && text[pos] == 'x')
        {
            pos++;
            degree = 1;
            skipSpaces(text, pos);
            if (pos < text.size() && text[pos] == '^')
            {
                pos++;
                skipSpaces(text, pos);
                size_t start = pos;
                degree = 0;
                while (pos < text.size() && isDigit(text[pos]))
                {
                    degree = degree * 10 + (text[pos] - '0');
                    pos++;
                    if (pos - start > 4)
                        throw invalid_argument("degree in polynomial text is too large");
                }
                if (pos == start)
                    throw invalid_argument("degree expected after ^");
            }
        }
        else if (!hasNumber)
            throw invalid_argument("wrong polynomial text");

        if (degree >= size)
            throw invalid_argument("degree of a term is not less than polynomial size");
        sum[degree] += sign * c;
        anyTerm = true;
        skipSpaces(text, pos);
    }
    if (!anyTerm)
        throw invalid_argument("empty polynomial text");

    for (int i = 0; i < MAX_SIZE; i++)
        if (!isfinite(sum[i]))
            throw invalid_argument("coefficient is out of range of double");
    this->size = size;
    for (int i = 0; i < MAX_SIZE; i++)
        coef[i] = i < size ? sum[i] : 0;
    recount();
}

Polinom::Polinom(const Polinom& other) : size(other.size), count(other.count)
{
    for (int i = 0; i < MAX_SIZE; i++)
        coef[i] = other.coef[i];
}

Polinom::~Polinom()
{
}

int Polinom::Size() const { return size; }
int Polinom::Count() const { return count; }
int Polinom::Degree() const { return count - 1; }

double Polinom::GetCoefficient(int index) const
{
    checkIndex(index);
    return coef[index];
}

void Polinom::SetCoefficient(int index, double value)
{
    checkIndex(index);
    checkValue(value);
    coef[index] = value;
    recount();
}

Polinom::Item Polinom::operator [] (int index)
{
    checkIndex(index);
    return Item(*this, index);
}

double Polinom::operator [] (int index) const
{
    checkIndex(index);
    return coef[index];
}

Polinom& Polinom::operator = (const Polinom& other)
{
    size = other.size;
    count = other.count;
    for (int i = 0; i < MAX_SIZE; i++)
        coef[i] = other.coef[i];
    return *this;
}

Polinom Polinom::operator - () const
{
    Polinom r(size);
    for (int i = 0; i < size; i++)
        r.coef[i] = -coef[i];
    r.recount();
    return r;
}

Polinom& Polinom::operator += (const Polinom& other) { return *this = *this + other; }
Polinom& Polinom::operator -= (const Polinom& other) { return *this = *this - other; }
Polinom& Polinom::operator *= (const Polinom& other) { return *this = *this * other; }
Polinom& Polinom::operator /= (const Polinom& other) { return *this = *this / other; }
Polinom& Polinom::operator %= (const Polinom& other) { return *this = *this % other; }

Polinom::operator string() const
{
    ostringstream out;
    out.precision(15);
    bool first = true;
    for (int i = count - 1; i >= 0; i--)
    {
        double c = coef[i];
        if (c == 0)
            continue;
        if (first)
            out << (c < 0 ? "-" : "");
        else
            out << (c < 0 ? " - " : " + ");
        double a = fabs(c);
        if (i == 0 || a != 1)
            out << a;
        if (i >= 1)
            out << "x";
        if (i >= 2)
            out << "^" << i;
        first = false;
    }
    if (first)
        out << "0";
    return out.str();
}

double Polinom::Value(double x) const
{
    if (!isfinite(x))
        throw invalid_argument("x must be a finite number");
    double result = 0;
    for (int i = count - 1; i >= 0; i--)
        result = result * x + coef[i];
    if (!isfinite(result))
        throw overflow_error("value of the polynomial is out of range of double");
    return result;
}

double Polinom::operator () (double x) const
{
    return Value(x);
}

Polinom Polinom::Derivative() const
{
    Polinom r(size);
    for (int i = 1; i < count; i++)
        r.coef[i - 1] = i * coef[i];
    r.recount();
    r.checkResult();
    return r;
}

Polinom Polinom::Integral(double constant) const
{
    checkValue(constant);
    if (count + 1 > MAX_SIZE)
        throw overflow_error("integral does not fit into the array");
    Polinom r(size < MAX_SIZE ? size + 1 : MAX_SIZE);
    r.coef[0] = constant;
    for (int i = 0; i < count; i++)
        r.coef[i + 1] = coef[i] / (i + 1);
    r.recount();
    r.checkResult();
    return r;
}

double Polinom::Integral(double a, double b) const
{
    if (!isfinite(a) || !isfinite(b))
        throw invalid_argument("limits of integration must be finite numbers");
    double sum = 0, powA = a, powB = b;
    for (int i = 0; i < count; i++)
    {
        sum += coef[i] * (powB - powA) / (i + 1);
        powA *= a;
        powB *= b;
    }
    if (!isfinite(sum))
        throw overflow_error("integral is out of range of double");
    return sum;
}

Polinom operator + (const Polinom& a, const Polinom& b)
{
    Polinom r(a.size > b.size ? a.size : b.size);
    for (int i = 0; i < r.size; i++)
        r.coef[i] = (i < a.size ? a.coef[i] : 0) + (i < b.size ? b.coef[i] : 0);
    r.recount();
    r.checkResult();
    return r;
}

Polinom operator - (const Polinom& a, const Polinom& b)
{
    Polinom r(a.size > b.size ? a.size : b.size);
    for (int i = 0; i < r.size; i++)
        r.coef[i] = (i < a.size ? a.coef[i] : 0) - (i < b.size ? b.coef[i] : 0);
    r.recount();
    r.checkResult();
    return r;
}

Polinom operator * (const Polinom& a, const Polinom& b)
{
    int needed = a.count + b.count - 1;
    int size = a.size + b.size - 1;
    if (size > Polinom::MAX_SIZE)
        size = Polinom::MAX_SIZE;
    if (needed > size)
        throw overflow_error("degree of the product does not fit into the array");
    Polinom r(size);
    for (int i = 0; i < a.count; i++)
        for (int j = 0; j < b.count; j++)
            r.coef[i + j] += a.coef[i] * b.coef[j];
    r.recount();
    r.checkResult();
    return r;
}

Polinom operator / (const Polinom& a, const Polinom& b)
{
    Polinom quotient, remainder;
    Polinom::divide(a, b, quotient, remainder);
    return quotient;
}

Polinom operator % (const Polinom& a, const Polinom& b)
{
    Polinom quotient, remainder;
    Polinom::divide(a, b, quotient, remainder);
    return remainder;
}

bool operator == (const Polinom& a, const Polinom& b) { return Polinom::compare(a, b) == 0; }
bool operator != (const Polinom& a, const Polinom& b) { return Polinom::compare(a, b) != 0; }
bool operator <  (const Polinom& a, const Polinom& b) { return Polinom::compare(a, b) < 0; }
bool operator >  (const Polinom& a, const Polinom& b) { return Polinom::compare(a, b) > 0; }
bool operator <= (const Polinom& a, const Polinom& b) { return Polinom::compare(a, b) <= 0; }
bool operator >= (const Polinom& a, const Polinom& b) { return Polinom::compare(a, b) >= 0; }

ostream& operator << (ostream& out, const Polinom& p)
{
    out << string(p);
    return out;
}

istream& operator >> (istream& in, Polinom& p)
{
    int n;
    cout << "size = ";
    if (!(in >> n))
        return in;
    if (n < 1 || n > Polinom::MAX_SIZE)
    {
        in.setstate(ios::failbit);
        return in;
    }
    Polinom t(n);
    for (int i = 0; i < n; i++)
    {
        double c;
        cout << "a[" << i << "] = ";
        if (!(in >> c))
            return in;
        if (!isfinite(c))
        {
            in.setstate(ios::failbit);
            return in;
        }
        t.coef[i] = c;
    }
    t.recount();
    p = t;
    return in;
}
