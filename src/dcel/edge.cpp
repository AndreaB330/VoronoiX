#include "edge.hpp"
#include "face.hpp"
#include "vertex.hpp"

using namespace vx;

int Edge::id_generator = 0;

Edge::Edge()
        : id(id_generator++),
          origin(nullptr),
          twin(nullptr),
          next(nullptr),
          previous(nullptr),
          incident_face(nullptr) {

}

int Edge::get_side(const Vec2& point) {
    if (origin == nullptr || next == nullptr || next->origin == nullptr) {
        double dist_sqr_left = incident_face->center.dist_square(point);
        double dist_sqr_right = twin->incident_face->center.dist_square(point);
        return (dist_sqr_left < dist_sqr_right ? +1 : (dist_sqr_left > dist_sqr_right ? -1 : 0));
    }
    double c = cross(next->origin->point - origin->point, point - origin->point);
    return (c > 0 ? +1 : (c < 0 ? -1 : 0));
}

Vec2 Edge::get_direction() {
    if (origin && next && next->origin) {
        return next->origin->point - origin->point;
    }
    return (twin->incident_face->center - incident_face->center).rot();
}

Vec2 Edge::get_any_point() {
    if (origin)
        return origin->point;
    if (twin->origin)
        return twin->origin->point;
    return (twin->incident_face->center + incident_face->center) * 0.5;
}
