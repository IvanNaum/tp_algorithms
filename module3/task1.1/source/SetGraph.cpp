
#include <cassert>
#include "../include/SetGraph.hpp"

SetGraph::SetGraph(size_t size) : size(size) {};

SetGraph::SetGraph(const IGraph &graph) : SetGraph(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (int to: graph.GetNextVertices(from)) {
            AddEdge(from, to);
        }
    }
}

SetGraph::~SetGraph() = default;

void SetGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < size);
    assert(0 <= to && to < size);

    edges_set.insert({from, to});
}

int SetGraph::VerticesCount() const {
    return size;
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < size);

    std::vector<int> result;
    for (const auto &k: edges_set) {
        if (k.first == vertex) {
            result.push_back(k.second);
        }
    }
    return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < size);

    std::vector<int> result;
    for (const auto &k: edges_set) {
        if (k.second == vertex) {
            result.push_back(k.first);
        }
    }
    return result;
}
