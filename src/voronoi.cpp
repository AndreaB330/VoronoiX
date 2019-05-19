#include "voronoi.hpp"
#include <algorithm>
#include <dcel/vertex.hpp>
#include <dcel/face.hpp>
#include <iostream>

using namespace vx;

const double EPS = 1e-8;

bool VoronoiNode::is_leaf() const {
    return face != nullptr;
}

VoronoiNode::VoronoiNode()
        : face(nullptr), left_node(nullptr), right_node(nullptr), axis(0), convex_hull() {

}

int shift(int i, int dx, int n) {
    int res = i + dx;
    if (res < 0) return res + n;
    if (res >= n) return res - n;
    return res;
}

void connect(const std::shared_ptr<Edge>& first, const std::shared_ptr<Edge>& second) {
    first->next = second;
    second->previous = first;
}

FacesConstIterator VoronoiNode::begin() const {
    return begin_it;
}

FacesConstIterator VoronoiNode::end() const {
    return end_it;
}

void rotate(const std::shared_ptr<Face>& face) {
    std::shared_ptr<Edge> ptr = face->edge;
    std::shared_ptr<Edge> start = ptr;
    bool first = true;
    while (first || start != ptr) {
        if (ptr->origin == nullptr) {
            face->edge = ptr;
            break;
        }
        ptr = ptr->next;
        first = false;
    }
}

std::unique_ptr<VoronoiNode> vx::build(FacesIterator first, FacesIterator last) {
    std::unique_ptr<VoronoiNode> node = std::make_unique<VoronoiNode>();
    node->begin_it = first;
    node->end_it = last;
    if ((last - first) == 1) {
        node->face = *first;
        node->convex_hull.push_back(*first);
        (*first)->edge = nullptr;
        return std::move(node);
    }
    double maxX = (*std::max_element(first, last, Face::cmp_x()))->center.x;
    double minX = (*std::min_element(first, last, Face::cmp_x()))->center.x;
    double maxY = (*std::max_element(first, last, Face::cmp_y()))->center.y;
    double minY = (*std::min_element(first, last, Face::cmp_y()))->center.y;

    int half = (last - first) / 2;
    if (maxX - minX > maxY - minY) {
        node->axis = 0;
        std::nth_element(first, first + half, last, Face::cmp_x());
    } else {
        node->axis = 1;
        std::nth_element(first, first + half, last, Face::cmp_y());
    }

    node->left_node = build(first, first + half);
    node->right_node = build(first + half, last);

    auto& a = node->left_node->convex_hull;
    auto& b = node->right_node->convex_hull;
    std::pair<int, int> upper = get_supporting_line(a, b, false);
    std::pair<int, int> lower = get_supporting_line(a, b, true);

    if (upper == lower) {
        std::shared_ptr<Edge> e1 = std::make_shared<Edge>();
        std::shared_ptr<Edge> e2 = std::make_shared<Edge>();
        e1->incident_face = b[upper.second];
        e2->incident_face = a[upper.first];
        e1->twin = e2;
        e2->twin = e1;
        if (b[upper.second]->edge == nullptr)
            b[upper.second]->edge = e1;
        if (a[upper.first]->edge == nullptr)
            a[upper.first]->edge = e2;
        connect(e1, b[upper.second]->edge);
        connect(b[upper.second]->edge, e1);
        connect(e2, a[upper.first]->edge);
        connect(a[upper.first]->edge, e2);

        node->chain.push_back(e1->get_any_point() - e1->get_direction());
        node->chain.push_back(e1->get_any_point() + e1->get_direction());
    } else {
        std::shared_ptr<Face> left_face = a[upper.first];
        std::shared_ptr<Face> right_face = b[upper.second];
        std::shared_ptr<Vertex> origin = nullptr;
        std::shared_ptr<Edge> last_edge, l_edge_in, l_edge_out, r_edge_in, r_edge_out;
        std::vector<std::shared_ptr<Edge>> chain, l_edges_in, l_edges_out, r_edges_in, r_edges_out;

        while (proceed_chain(left_face, right_face, origin, last_edge, l_edge_in, r_edge_in, l_edge_out, r_edge_out)) {
            l_edges_in.push_back(l_edge_in);
            l_edges_out.push_back(l_edge_out);
            r_edges_in.push_back(r_edge_in);
            r_edges_out.push_back(r_edge_out);
            chain.push_back(last_edge);
        }
        chain.push_back(last_edge);

        if (upper.first == lower.first) {
            connect(chain[0]->twin, chain.back()->twin);
        } else {
            connect(chain[0]->twin, a[upper.first]->edge);
            if (chain.size() > 1)
                connect(a[lower.first]->edge->previous, chain.back()->twin);
        }
        if (upper.second == lower.second) {
            connect(chain.back(), chain[0]);
        } else {
            connect(b[upper.second]->edge->previous, chain[0]);
            if (chain.size() > 1)
                connect(chain.back(), b[lower.second]->edge);
        }

        for (int i = 1; i < chain.size(); i++) {
            if (l_edges_in[i - 1] && l_edges_out[i - 1]) {
                std::shared_ptr<Edge> ptr = l_edges_out[i - 1];
                while (true) {
                    ptr->origin = chain[i]->origin;
                    if (ptr->twin == l_edges_in[i - 1]) break;
                    ptr = ptr->twin->next;
                }
                connect(l_edges_in[i - 1], chain[i - 1]->twin);
                connect(chain[i]->twin, l_edges_out[i - 1]);
            } else {
                connect(chain[i]->twin, chain[i - 1]->twin);
            }
            if (r_edges_in[i - 1] && r_edges_out[i - 1]) {
                std::shared_ptr<Edge> ptr = r_edges_out[i - 1];
                while (true) {
                    ptr->origin = chain[i]->origin;
                    if (ptr->twin == r_edges_in[i - 1]) break;
                    ptr = ptr->twin->next;
                }
                connect(r_edges_in[i - 1], chain[i]);
                connect(chain[i - 1], r_edges_out[i - 1]);

            } else {
                connect(chain[i - 1], chain[i]);
            }
        }

        for (auto& e : chain) {
            e->incident_face->edge = e;
            e->twin->incident_face->edge = e->twin;
            rotate(e->incident_face);
            rotate(e->twin->incident_face);
            if (e->origin == nullptr) {
                node->chain.push_back(e->get_any_point() - e->get_direction());
            } else {
                node->chain.push_back(e->origin->point);
            }
            if (e->twin->origin == nullptr) {
                node->chain.push_back(e->get_any_point() + e->get_direction());
            }
        }
    }

    int n = a.size();
    int m = b.size();

    if (cross(a[upper.first]->center, b[upper.second]->center, a[shift(upper.first, +1, n)]->center) == 0) {
        upper.first = shift(upper.first, +1, n);
    }

    if (cross(a[upper.first]->center, b[upper.second]->center, b[shift(upper.second, -1, m)]->center) == 0) {
        upper.second = shift(upper.second, -1, m);
    }

    if (cross(a[lower.first]->center, b[lower.second]->center, a[shift(lower.first, -1, n)]->center) == 0) {
        lower.first = shift(lower.first, -1, n);
    }

    if (cross(a[lower.first]->center, b[lower.second]->center, b[shift(lower.second, +1, m)]->center) == 0) {
        lower.second = shift(lower.second, +1, m);
    }

    std::vector<std::shared_ptr<Face>> convex;
    if (upper.first != lower.first) {
        convex.push_back(a[upper.first]);
        for (int i = shift(upper.first, +1, n); i != lower.first; i = shift(i, +1, n)) {
            convex.push_back(a[i]);
        }
    }
    convex.push_back(a[lower.first]);

    if (upper.second != lower.second) {
        convex.push_back(b[lower.second]);
        for (int i = shift(lower.second, +1, m); i != upper.second; i = shift(i, +1, m)) {
            convex.push_back(b[i]);
        }
    }
    convex.push_back(b[upper.second]);

    node->convex_hull = std::move(convex);

    return std::move(node);
}

std::pair<int, int> vx::get_supporting_line(
        const std::vector<std::shared_ptr<Face>>& a,
        const std::vector<std::shared_ptr<Face>>& b,
        bool cw) {
    int i = 0, j = 0;
    while (update_supporting_line(a[i], b, j, !cw) || update_supporting_line(b[j], a, i, cw)) {}
    return {i, j};
}

bool vx::update_supporting_line(
        std::shared_ptr<Face> a,
        const std::vector<std::shared_ptr<Face>>& b,
        int& i, bool cw) {
    int start = i;
    if (b.size() == 1) return false;
    if (b.size() == 2 && cross(a->center, b[0]->center, b[1]->center) == 0) {
        if (b[i]->center.dist_square(a->center) < b[i ^ 1]->center.dist_square(a->center))
            return false;
        i ^= 1;
        return true;
    }
    while (true) {
        int next = shift(i, +1, b.size());
        int prev = shift(i, -1, b.size());
        double cross_next = cross(a->center, b[next]->center, b[i]->center);
        double cross_prev = cross(a->center, b[prev]->center, b[i]->center);
        if (cw) {
            if (cross_next > 0 && cross_prev >= 0) {
                break;
            }
            if (cross_next >= 0 && cross_prev < 0) {
                i = prev;
            } else {
                i = next;
            }
        } else {
            if (cross_next <= 0 && cross_prev < 0)
                break;
            if (cross_next > 0 && cross_prev <= 0) {
                i = next;
            } else {
                i = prev;
            }
        }
    }
    return start != i;
}

double find_hit(
        const std::shared_ptr<Face>& face,
        Vec2 ray_origin,
        Vec2 ray_direction,
        std::shared_ptr<Edge>& hit_edge,
        std::shared_ptr<Vertex>& hit_vertex
) {
    hit_edge = nullptr;
    hit_vertex = nullptr;
    std::shared_ptr<Edge> ptr = face->edge;
    if (ptr == nullptr) {
        return std::numeric_limits<double>::max();
    }
    do {
        Vec2 ptr_dir = ptr->get_direction();
        if (cross(ray_direction, ptr_dir) > EPS) {
            Vec2 ptr_point = ptr->get_any_point();
            if (ptr->next->origin && std::abs(cross(ray_direction, ptr->next->origin->point - ray_origin)) < EPS) {
                hit_edge = ptr->next;
                hit_vertex = ptr->next->origin;
                return dot(ray_direction, hit_vertex->point - ray_origin) / dot(ray_direction, ray_direction);
            }
            if (ptr->origin && std::abs(cross(ray_direction, ptr->origin->point - ray_origin)) < EPS) {
                hit_edge = ptr;
                hit_vertex = ptr->origin;
                return dot(ray_direction, hit_vertex->point - ray_origin) / dot(ray_direction, ray_direction);
            }
            if ((!ptr->origin || cross(ptr->origin->point - ray_origin, ray_direction) > 0) &&
                (!ptr->next->origin || cross(ray_direction, ptr->next->origin->point - ray_origin) > 0)) {
                hit_edge = ptr;
                return dot(ptr_point - ray_origin, ptr_dir.rot()) / dot(ray_direction, ptr_dir.rot());
            }
        }
        ptr = ptr->next;
    } while (ptr != face->edge);
    return std::numeric_limits<double>::max();
}

bool vx::proceed_chain(
        std::shared_ptr<Face>& a, std::shared_ptr<Face>& b,
        std::shared_ptr<Vertex>& origin, std::shared_ptr<Edge>& e,
        std::shared_ptr<Edge>& l_edge_in, std::shared_ptr<Edge>& r_edge_in,
        std::shared_ptr<Edge>& l_edge_out, std::shared_ptr<Edge>& r_edge_out) {

    std::shared_ptr<Edge> e1 = std::make_shared<Edge>();
    std::shared_ptr<Edge> e2 = std::make_shared<Edge>();

    e = e1;

    e1->incident_face = b;
    e2->incident_face = a;

    e1->twin = e2;
    e2->twin = e1;

    e1->origin = origin;

    Vec2 ray_direction = e1->get_direction();
    Vec2 ray_origin = e1->get_any_point();

    std::shared_ptr<Edge> ea = nullptr;
    std::shared_ptr<Vertex> va = nullptr;
    double depth_a = find_hit(a, ray_origin, ray_direction, ea, va);

    std::shared_ptr<Edge> eb;
    std::shared_ptr<Vertex> vb;
    double depth_b = find_hit(b, ray_origin, ray_direction, eb, vb);

    if (!ea && !eb) return false;

    if (ea && eb && std::abs(depth_a - depth_b) < EPS) {
        std::shared_ptr<Vertex> hit = std::make_shared<Vertex>(ray_origin + ray_direction * depth_a);
        e2->origin = hit;
        hit->incident_edge = e2;
        if (va) {
            l_edge_in = ea->previous;
            l_edge_out = ea->twin->next;
            hit->point = va->point;
        } else {
            l_edge_in = ea;
            l_edge_out = ea->twin;
        }
        if (vb) {
            r_edge_in = eb->previous->twin->previous;
            r_edge_out = eb;
            hit->point = vb->point;
        } else {
            r_edge_in = eb->twin;
            r_edge_out = eb;
        }

        b = r_edge_in->incident_face;
        a = l_edge_out->incident_face;
        origin = hit;
        return true;
    }
    std::shared_ptr<Vertex> hit = std::make_shared<Vertex>(ray_origin + ray_direction * std::min(depth_a, depth_b));
    if (depth_a < depth_b && ea) {
        e2->origin = hit;
        hit->incident_edge = e2;
        if (va) {
            hit->point = va->point;
            l_edge_in = ea->previous;
            l_edge_out = ea->twin->next;
        } else {
            l_edge_in = ea;
            l_edge_out = ea->twin;
        }

        r_edge_in = nullptr;
        r_edge_out = nullptr;

        a = l_edge_out->incident_face;
    }
    if (depth_a > depth_b && eb) {
        e2->origin = hit;
        hit->incident_edge = e2;
        if (vb) {
            hit->point = vb->point;
            r_edge_in = eb->previous->twin->previous;
            r_edge_out = eb;
        } else {
            r_edge_in = eb->twin;
            r_edge_out = eb;
        }

        l_edge_in = nullptr;
        l_edge_out = nullptr;

        b = r_edge_in->incident_face;
    }
    origin = hit;
    return true;
}
