#ifndef VORONOIX_VORONOI_HPP
#define VORONOIX_VORONOI_HPP

#include <vector>
#include "dcel/edge.hpp"
#include <vx/math/vec.hpp>

namespace vx {

typedef std::vector<std::shared_ptr<Face>>::iterator FacesIterator;
typedef std::vector<std::shared_ptr<Face>>::const_iterator FacesConstIterator;

struct VoronoiNode {
    VoronoiNode();

    std::vector<Vec2> chain;
    //std::vector<Edge*> pruned;
    std::vector<std::shared_ptr<Face>> convex_hull;
    std::shared_ptr<Face> face;

    std::unique_ptr<VoronoiNode> left_node;
    std::unique_ptr<VoronoiNode> right_node;

    FacesIterator begin_it;
    FacesIterator end_it;

    FacesConstIterator begin() const;
    FacesConstIterator end() const;

    int axis; // 0 - vertical chain, 1 - horizontal chain

    bool is_leaf() const;

};

std::unique_ptr<VoronoiNode> build(FacesIterator first, FacesIterator last);

std::pair<int, int> get_supporting_line(
        const std::vector<std::shared_ptr<Face>>& a,
        const std::vector<std::shared_ptr<Face>>& b,
        bool cw = false);

bool update_supporting_line(
        std::shared_ptr<Face> a,
        const std::vector<std::shared_ptr<Face>>& b,
        int& i,
        bool cw = false);

bool proceed_chain(std::shared_ptr<Face>& a, std::shared_ptr<Face>& b,
                   std::shared_ptr<Vertex>& origin, std::shared_ptr<Edge>& e,
                   std::shared_ptr<Edge>& l_edge_in, std::shared_ptr<Edge>& r_edge_in,
                   std::shared_ptr<Edge>& l_edge_out, std::shared_ptr<Edge>& r_edge_out);

} // namespace vx



#endif //VORONOIX_VORONOI_HPP
