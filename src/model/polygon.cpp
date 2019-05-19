#include "vx/model/polygon.hpp"

using namespace vx;

Polygon::Polygon() {

}

Polygon::Polygon(std::vector<Vec2>& points) : points(points) {

}

size_t Polygon::size() const {
    return points.size();
}

const Vec2& Polygon::operator[](size_t i) const {
    if (i >= size()) return points[i % size()];
    return points[i];
}

Vec2& Polygon::operator[](size_t i) {
    if (i >= size()) return points[i % size()];
    return points[i];
}

void Polygon::add(const Vec2& point) {
    points.push_back(point);
}

std::vector<Vec2>::const_iterator Polygon::begin() const {
    return points.begin();
}

std::vector<Vec2>::const_iterator Polygon::end() const {
    return points.end();
}

Vec2 Polygon::centroid() {
    Vec2 sum = Vec2();
    for (const Vec2& p : points) sum += p;
    return sum / size();
}

Polygon Polygon::squeezed(double factor) {
    Polygon result = *this;
    for (int i = 0; i < size(); i++) {
        Vec2 v1 = (*this)[i];
        Vec2 v2 = (*this)[i + 1];
        Vec2 e_dir = (v2-v1).rot().normalized();
        v1 += e_dir * factor;
        v2 += e_dir * factor;
        Polygon c = Polygon();
        for (int j = 0; j < result.size(); j++) {
            Vec2 v3 = result[j];
            Vec2 v4 = result[j+1];
            Vec2 rot = (v3-v4).rot();
            Vec2 dir = (v2-v1);
            Vec2 x = v1 + dir * dot(v3-v1, rot) / dot(dir, rot);
            if (cross(v1, v2, v3) > 0 && cross(v1, v2, v4) < 0) {
                c.add(v3);
                c.add(x);
            } else if (cross(v1, v2, v3) < 0 && cross(v1, v2, v4) < 0) {

            } else if (cross(v1, v2, v3) < 0 && cross(v1, v2, v4) > 0) {
                c.add(x);
            } else {
                c.add(v3);
            }
        }
        result = c;
    }
    return result;
}

Polygon vx::intersect(const Polygon& p1, const Polygon& p2) {
    Polygon result = p2;
    for (int i = 0; i < p1.size(); i++) {
        Vec2 v1 = p1[i];
        Vec2 v2 = p1[i + 1];
        Polygon c = Polygon();
        for (int j = 0; j < result.size(); j++) {
            Vec2 v3 = result[j];
            Vec2 v4 = result[j+1];
            Vec2 rot = (v3-v4).rot();
            Vec2 dir = (v2-v1);
            Vec2 x = v1 + dir * dot(v3-v1, rot) / dot(dir, rot);
            if (cross(v1, v2, v3) > 0 && cross(v1, v2, v4) < 0) {
                c.add(v3);
                c.add(x);
            } else if (cross(v1, v2, v3) < 0 && cross(v1, v2, v4) < 0) {

            } else if (cross(v1, v2, v3) < 0 && cross(v1, v2, v4) > 0) {
                c.add(x);
            } else {
                c.add(v3);
            }
        }
        result = c;
    }
    return result;
}
