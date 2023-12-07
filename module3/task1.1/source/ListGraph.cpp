
#include <cassert>
#include "../include/ListGraph.hpp"

ListGraph::ListGraph(size_t size) : adjacency_list(size) {};

ListGraph::ListGraph(const IGraph &graph) : ListGraph(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (int to: graph.GetNextVertices(from)) {
            AddEdge(from, to);
        }
    }
};

ListGraph::~ListGraph() = default;

void ListGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacency_list.size());
    assert(0 <= to && to < adjacency_list.size());
    adjacency_list.at(from).push_back(to);
}

int ListGraph::VerticesCount() const {
    return adjacency_list.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacency_list.size());

    return adjacency_list.at(vertex);
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacency_list.size());

    auto result = std::vector<int>();
    for (size_t from = 0; from < adjacency_list.size(); ++from) {
        for (int to: adjacency_list.at(from)) {
            if (to == vertex) {
                result.push_back(from);
            }
        }
    }

    return result;
}


