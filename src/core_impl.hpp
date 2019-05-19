#ifndef VORONOIX_CORE_IMPL_HPP
#define VORONOIX_CORE_IMPL_HPP

#include <memory>
#include "voronoi.hpp"
#include "vx/core.hpp"

namespace vx {

class core_impl : public core {
public:
    core_impl();

    void fit(const std::vector<Vec2>& points) override;

    int add(const Vec2 & point);

    vx::Polygon get_cell(int face_id) const override;

    Vec2 get_center(int face_id) const override;

    vx::SimpleGraph get_delaunay_graph() const override;

    void lloyd_relaxation(double step);

    void add_crop(const Polygon & poly);

    void add_crop(double min_x, double max_x, double min_y, double max_y);

    void remove_crop();

    int size() const;

    std::vector<int> get_convex_hull() const override;

    std::vector<int> get_nearest_points() const;

    int get_nearest_face(const Vec2 & point) const;

    int get_nearest_face(const Vec2 & point,
            std::vector<std::vector<int>> & faces,
            std::vector<std::vector<Vec2>> & chain) const;

private:

    std::unique_ptr<vx::VoronoiNode> root;
    std::vector<std::shared_ptr<vx::Face>> face_by_id;
    std::vector<std::shared_ptr<vx::Face>> faces_tree_order;

    vx::Polygon crop_poly;
};

} // namesapce vx

#endif //VORONOIX_CORE_IMPL_HPP
