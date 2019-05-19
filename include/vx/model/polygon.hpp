#ifndef VORONOIX_POLYGON_HPP
#define VORONOIX_POLYGON_HPP
#include <algorithm>
#include <vx/math/vec.hpp>
#include <vector>

namespace vx {

class Polygon {
public:
    Polygon();

    explicit Polygon(std::vector<Vec2>& points);

    size_t size() const;

    const Vec2& operator[](size_t i) const;

    Vec2& operator[](size_t i);

    void add(const Vec2& point);

    std::vector<Vec2>::const_iterator begin() const;

    std::vector<Vec2>::const_iterator end() const;

    Vec2 centroid();

    Polygon squeezed(double factor = 0.05);

    friend Polygon intersect(const Polygon & p1, const Polygon & p2);

private:
    std::vector<Vec2> points;
};

} // namespace vx

#endif //VORONOIX_POLYGON_HPP
