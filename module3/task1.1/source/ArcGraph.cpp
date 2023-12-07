#include <cassert>
#include "../include/ArcGraph.hpp"

ArcGraph::ArcGraph(size_t size) : size(size) {};

ArcGraph::ArcGraph(const IGraph &graph) : ArcGraph(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (int to: graph.GetNextVertices(from)) {
            AddEdge(from, to);
        }
    }
}

ArcGraph::~ArcGraph() = default;

void ArcGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < size);
    assert(0 <= to && to < size);

    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return size;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < size);
    std::vector<int> result;

    for (auto k: edges) {
        if (k.first == vertex) {
            result.push_back(k.second);
        }
    }

    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < size);
    std::vector<int> result;

    for (auto k: edges) {
        if (k.second == vertex) {
            result.push_back(k.first);
        }
    }

    return result;
}


