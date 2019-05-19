#include "core_impl.hpp"
#include "dcel/face.hpp"
#include "dcel/vertex.hpp"
#include <vx/model/polygon.hpp>

using namespace vx;

core_impl::core_impl() = default;

void core_impl::fit(const std::vector<Vec2>& points) {
    faces_tree_order.clear();
    faces_tree_order.reserve(points.size());
    for (int i = 0; i < points.size(); i++) {
        faces_tree_order.push_back(std::make_shared<vx::Face>(i, points[i]));
    }
    face_by_id = faces_tree_order;
    root = build(faces_tree_order.begin(), faces_tree_order.end());
}

vx::SimpleGraph core_impl::get_delaunay_graph() const {
    vx::SimpleGraph graph = vx::SimpleGraph(face_by_id.size());
    for (const auto& face : face_by_id) {
        std::shared_ptr<vx::Edge> ptr = face->edge;
        do {
            if (ptr->twin->incident_face->id < face->id)
                graph.add_edge(ptr->twin->incident_face->id, face->id);
            ptr = ptr->next;
        } while (ptr != face->edge);
    }
    return graph;
}

vx::Polygon core_impl::get_cell(int face_id) const {
    vx::Polygon cell;
    std::shared_ptr<vx::Edge> ptr = face_by_id[face_id]->edge;
    if (ptr == nullptr)
        return cell;
    do {
        if (ptr->origin) {
            cell.add(ptr->origin->point);
        } else {
            cell.add(ptr->get_any_point() - ptr->get_direction() * 1e6); // todo: infty? crop?
        }
        if (!ptr->twin->origin) {
            cell.add(ptr->get_any_point() + ptr->get_direction() * 1e6);
        }
        ptr = ptr->next;
    } while (ptr != face_by_id[face_id]->edge);
    if (crop_poly.size())
        return intersect(cell, crop_poly);
    return cell;
}

Vec2 core_impl::get_center(int face_id) const {
    return face_by_id[face_id]->center;
}

void core_impl::lloyd_relaxation(double step) {
    for (auto& face : face_by_id) {
        if (get_cell(face->id).size())
            face->center += (get_cell(face->id).centroid() - face->center) * step;
    }
    root = build(faces_tree_order.begin(), faces_tree_order.end());
}

void core_impl::add_crop(const Polygon& poly) {
    crop_poly = poly;
}

void core_impl::remove_crop() {
    crop_poly = Polygon();
}

void core_impl::add_crop(double min_x, double max_x, double min_y, double max_y) {
    Polygon p;
    p.add(Vec2(min_x, min_y));
    p.add(Vec2(max_x, min_y));
    p.add(Vec2(max_x, max_y));
    p.add(Vec2(min_x, max_y));
    add_crop(p);
}

int core_impl::add(const Vec2& point) {
    face_by_id.push_back(std::make_shared<Face>(face_by_id.size(), point));
    faces_tree_order.push_back(face_by_id.back());
    root = build(faces_tree_order.begin(), faces_tree_order.end());
    return face_by_id.back()->id;
}

int core_impl::size() const {
    return face_by_id.size();
}

std::vector<int> core_impl::get_convex_hull() const {
    std::vector<int> convex_hull;
    for (const std::shared_ptr<Face>& face : root->convex_hull) {
        convex_hull.push_back(face->id);
    }
    return convex_hull;
}

std::vector<int> core_impl::get_nearest_points() const {
    std::vector<int> nearest(static_cast<unsigned int>(size()), -1);
    for (int i = 0; i < size(); i++) {
        std::shared_ptr<Edge> ptr = face_by_id[i]->edge;
        nearest[i] = ptr->twin->incident_face->id;
        double nearest_distance = get_center(i).dist_square(get_center(nearest[i]));
        do {
            double distance = get_center(i).dist_square(get_center(ptr->twin->incident_face->id));
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest[i] = ptr->twin->incident_face->id;
            }
            ptr = ptr->next;
        } while (ptr != face_by_id[i]->edge);

    }
    return nearest;
}

int core_impl::get_nearest_face(const Vec2& point) const {
    VoronoiNode* node = root.get();
    while (!node->is_leaf()) {
        int l = 0;
        int r = node->chain.size() - 1;
        while (l < r - 1) {
            int m = (l + r) / 2;
            if ((node->axis == 0 && node->chain[m].y >= point.y)
                ||
                (node->axis == 1 && node->chain[m].x <= point.x)) {
                l = m;
            } else {
                r = m;
            }
        }
        double cr = cross(node->chain[l], node->chain[r], point);
        int side = (cr > 0 ? +1 : (cr < 0 ? -1 : 0));
        if (side >= 0) node = node->right_node.get(); else node = node->left_node.get();
    }
    return node->face->id;
}

int core_impl::get_nearest_face(const Vec2& point, std::vector<std::vector<int>>& faces,
                                std::vector<std::vector<Vec2>>& chain) const {
    faces.clear();
    chain.clear();
    VoronoiNode* node = root.get();
    while (!node->is_leaf()) {
        int l = 0;
        int r = node->chain.size() - 1;
        while (l < r - 1) {
            int m = (l + r) / 2;
            if ((node->axis == 0 && node->chain[m].y >= point.y)
                ||
                (node->axis == 1 && node->chain[m].x <= point.x)) {
                l = m;
            } else {
                r = m;
            }
        }
        double cr = cross(node->chain[l], node->chain[r], point);
        int side = (cr > 0 ? +1 : (cr < 0 ? -1 : 0));
        if (side >= 0) node = node->right_node.get(); else node = node->left_node.get();
        std::vector<int> f;
        std::vector<Vec2> c;
        for (const auto& face : *node) {
            f.push_back(face->id);
        }
        for (const auto& v : node->chain) {
            c.push_back(v);
        }
        chain.push_back(c);
        faces.push_back(f);
    }
    return node->face->id;
}
