
#include <cassert>
#include "../include/MatrixGraph.hpp"

MatrixGraph::MatrixGraph(size_t size) : adjacency_matrix(size, std::vector<int>(size, 0)) {};

MatrixGraph::MatrixGraph(const IGraph &graph) : MatrixGraph(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (int to: graph.GetNextVertices(from)) {
            AddEdge(from, to);
        }
    }
}

MatrixGraph::~MatrixGraph() = default;

void MatrixGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacency_matrix.size());
    assert(0 <= to && to < adjacency_matrix.size());

    adjacency_matrix.at(from).at(to) = 1;
}

int MatrixGraph::VerticesCount() const {
    return adjacency_matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacency_matrix.size());

    std::vector<int> result;

    for (int i = 0; i < adjacency_matrix.at(vertex).size(); ++i) {
        if (adjacency_matrix.at(vertex).at(i) != 0) {
            result.push_back(i);
        }
    }

    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacency_matrix.size());

    std::vector<int> result;

    for (size_t i = 0; i < adjacency_matrix.size(); ++i) {
        if (adjacency_matrix.at(i).at(vertex) != 0) {
            result.push_back(i);
        }
    }

    return result;
}
