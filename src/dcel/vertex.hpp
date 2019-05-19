#ifndef VORONOIX_VERTEX_H
#define VORONOIX_VERTEX_H

#include <vx/math/vec.hpp>
#include <memory>

namespace vx {

class Edge;

class Vertex {
public:
    explicit Vertex(const Vec2& point);

    int id;

    Vec2 point;
    std::shared_ptr<Edge> incident_edge;

private:
    static int id_generator;
};

} // namespace vx

#endif //VORONOIX_VERTEX_H
