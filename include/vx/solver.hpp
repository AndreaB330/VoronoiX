#ifndef VORONOIX_SOLVER_HPP
#define VORONOIX_SOLVER_HPP

#include <memory>
#include <vector>
#include <vx/math/vec.hpp>
#include <vx/core.hpp>

namespace vx {

class VXSolver {
    VXSolver();

    void fit(const std::vector<Vec2>& data);

    //void add(const Vec2& point);

    //std::vector<size_t> get_convex_hull();

    //size_t get_any_neares_point(); // to point, to segment, to sphere

    //void get_delauney_graph();

    /*void get_nearest_connections();
    void get_nearest_pair();
    void get_max_circle();
    void get_euclidean_mst();*/
private:
    std::unique_ptr<vx::core> core;
};

} // namespace vx

#endif //VORONOIX_SOLVER_HPP
