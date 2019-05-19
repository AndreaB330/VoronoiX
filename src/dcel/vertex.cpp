#include "vertex.hpp"


using namespace vx;

int Vertex::id_generator = 0;

Vertex::Vertex(const Vec2& point)
        : id(id_generator++), point(point), incident_edge(nullptr) {

}
