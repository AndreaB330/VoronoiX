
#ifndef VORONOIX_CORE_HPP
#define VORONOIX_CORE_HPP
#include <vector>
#include <vx/math/vec.hpp>
#include <vx/model/polygon.hpp>
#include <vx/model/graph.hpp>

namespace vx {

class core {
public:
    virtual void fit(const std::vector<Vec2>& points) = 0;

    virtual vx::Polygon get_cell(int face_id) const = 0;

    virtual Vec2 get_center(int face_id) const = 0;

    virtual vx::SimpleGraph get_delaunay_graph() const = 0;

    virtual std::vector<int> get_convex_hull() const = 0;
};

} // namespace vx

#endif //VORONOIX_CORE_HPP
