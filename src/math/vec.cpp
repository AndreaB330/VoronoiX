#include "vx/math/vec.hpp"
#include <cmath>
#include <vx/math/vec.hpp>


Vec2::Vec2() : x(0), y(0) {

}

Vec2::Vec2(double x, double y) : x(x), y(y) {

}

double Vec2::length_square() const {
    return x * x + y * y;
}

double Vec2::length() const {
    return sqrt(length_square());
}

Vec2 Vec2::normalized() const {
    if (x == 0.0 && y == 0.0) return *this;
    return *this / length();
}

double Vec2::dist_square(const Vec2& a) const {
    return (*this - a).length_square();
}

double Vec2::dist(const Vec2& a) const {
    return (*this - a).length();
}

Vec2 Vec2::rot() const {
    return Vec2(-y, x);
}


Vec2 operator-(const Vec2& a) {
    return Vec2(-a.x, -a.y);
}

Vec2& operator+=(Vec2& a, const Vec2& b) {
    a.x += b.x, a.y += b.y;
    return a;
}

Vec2& operator-=(Vec2& a, const Vec2& b) {
    a.x -= b.x, a.y -= b.y;
    return a;
}

Vec2& operator*=(Vec2& a, double k) {
    a.x *= k, a.y *= k;
    return a;
}

Vec2& operator/=(Vec2& a, double k) {
    return a *= (1 / k);
}

Vec2 operator+(Vec2 a, const Vec2& b) {
    return a += b;
}

Vec2 operator-(Vec2 a, const Vec2& b) {
    return a -= b;
}

Vec2 operator*(Vec2 a, double k) {
    return a *= k;
}

Vec2 operator*(double k, Vec2 b) {
    return b *= k;
}

Vec2 operator/(Vec2 a, double k) {
    return a /= k;
}

double dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

double cross(const Vec2& a, const Vec2& b) {
    return a.x * b.y - a.y * b.x;
}

bool operator==(const Vec2& a, const Vec2& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const Vec2& a, const Vec2& b) {
    return !(a == b);
}

double cross(const Vec2& o, const Vec2& a, const Vec2& b) {
    return cross(a - o, b - o);
}

bool operator<(const Vec2& a, const Vec2& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

std::ostream& operator<<(std::ostream& out, const Vec2& v) {
    return out << '(' << v.x << ',' << ' ' << v.y << ')';
}
