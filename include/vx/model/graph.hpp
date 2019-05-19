#ifndef VORONOIX_GRAPH_HPP
#define VORONOIX_GRAPH_HPP

#include <vector>

namespace vx {

class SimpleGraph {
public:
    explicit SimpleGraph(int graph_size = 0);

    explicit SimpleGraph(const std::vector<std::vector<int>> & adjacency_list);

    void add_edge(int first, int second);

    const std::vector<int>& get_adjacent(int vertex) const;

    int size() const;

private:
    std::vector<std::vector<int>> adjacency_list;
};

} // namespace vx

#endif //VORONOIX_GRAPH_HPP
