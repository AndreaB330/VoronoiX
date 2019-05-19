
#include <vx/model/graph.hpp>

#include "vx/model/graph.hpp"

using namespace vx;


SimpleGraph::SimpleGraph(int graph_size) {
    adjacency_list.resize(static_cast<unsigned int>(graph_size));
}

SimpleGraph::SimpleGraph(const std::vector<std::vector<int>> & adjacency_list):adjacency_list(adjacency_list) {

}

void SimpleGraph::add_edge(int first, int second) {
    adjacency_list[first].push_back(second);
    adjacency_list[second].push_back(first);
}

const std::vector<int>& SimpleGraph::get_adjacent(int vertex) const {
    return adjacency_list[vertex];
}

int SimpleGraph::size() const {
    return adjacency_list.size();
}
