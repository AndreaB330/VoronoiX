#ifndef VORONOIX_EDGE_H
#define VORONOIX_EDGE_H
#include <memory>

#include <vx/math/vec.hpp>

namespace vx {

class Vertex;

class Face;

class Edge {
public:
    Edge();

    int id;

    std::shared_ptr<Vertex> origin;

    std::shared_ptr<Edge> twin;
    std::shared_ptr<Edge> next;
    std::shared_ptr<Edge> previous;

    std::shared_ptr<Face> incident_face;

    int get_side(const Vec2& point);

    Vec2 get_direction();

    Vec2 get_any_point();

private:
    static int id_generator;
};

} // namespace vx


#endif //VORONOIX_EDGE_H
