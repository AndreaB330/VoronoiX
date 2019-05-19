#ifndef VORONOIX_FACE_H
#define VORONOIX_FACE_H

#include <vx/math/vec.hpp>
#include <memory>

namespace vx {

class Edge;


class Face {
public:
    explicit Face(int id = 0, const Vec2& center = Vec2());

    int id;

    std::shared_ptr<Edge> edge;
    Vec2 center;

    struct cmp_x {
        bool operator()(const std::shared_ptr<Face>& lhs, const std::shared_ptr<Face>& rhs);

        bool operator()(const Face& lhs, const Face& rhs);
    };

    struct cmp_y {
        bool operator()(const std::shared_ptr<Face>& lhs, const std::shared_ptr<Face>& rhs);

        bool operator()(const Face& lhs, const Face& rhs);
    };
};

} // namespace vx


#endif //VORONOIX_FACE_H
