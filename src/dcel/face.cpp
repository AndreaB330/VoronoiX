#include <vx/math/vec.hpp>
#include "face.hpp"


using namespace vx;

Face::Face(int id, const Vec2& center) : id(id), edge(nullptr), center(center) {

}

bool Face::cmp_x::operator()(const std::shared_ptr<Face>& lhs, const std::shared_ptr<Face>& rhs) {
    return lhs->center.x < rhs->center.x || (lhs->center.x == rhs->center.x && lhs->center.y < rhs->center.y);
}

bool Face::cmp_x::operator()(const Face& lhs, const Face& rhs) {
    return lhs.center.x < rhs.center.x || (lhs.center.x == rhs.center.x && lhs.center.y < rhs.center.y);
}

bool Face::cmp_y::operator()(const std::shared_ptr<Face>& lhs, const std::shared_ptr<Face>& rhs) {
    return lhs->center.y < rhs->center.y || (lhs->center.y == rhs->center.y && lhs->center.x < rhs->center.x);
}

bool Face::cmp_y::operator()(const Face& lhs, const Face& rhs) {
    return lhs.center.y < rhs.center.y || (lhs.center.y == rhs.center.y && lhs.center.x < rhs.center.x);
}