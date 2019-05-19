#ifndef VORONOIX_VEC_H
#define VORONOIX_VEC_H
#include <iostream>

class Vec2 {
public:
    double x, y;

    Vec2();
    Vec2(double x, double y);

    double length_square() const;
    double length() const;

    Vec2 normalized() const;
    Vec2 rot() const;

    double dist_square(const Vec2 & a) const;
    double dist(const Vec2 & a) const;
};

Vec2 operator-(const Vec2 & a);

Vec2& operator+=(Vec2 & a, const Vec2 & b);
Vec2& operator-=(Vec2 & a, const Vec2 & b);
Vec2& operator*=(Vec2 & a, double k);
Vec2& operator/=(Vec2 & a, double k);

Vec2 operator+(Vec2 a, const Vec2 & b);
Vec2 operator-(Vec2 a, const Vec2 & b);
Vec2 operator*(Vec2 a, double k);
Vec2 operator*(double k, Vec2 b);
Vec2 operator/(Vec2 a, double k);

double dot(const Vec2 & a, const Vec2 & b);
double cross(const Vec2 & a, const Vec2 & b);
double cross(const Vec2 & o, const Vec2 & a, const Vec2 & b);

bool operator<(const Vec2 & a, const Vec2 & b);
bool operator==(const Vec2 & a, const Vec2 & b);
bool operator!=(const Vec2 & a, const Vec2 & b);

std::ostream& operator<<(std::ostream& out, const Vec2 & v);

#endif //VORONOIX_VEC_H
