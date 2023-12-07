#ifndef CPP_TASKS_MATRIXGRAPH_HPP
#define CPP_TASKS_MATRIXGRAPH_HPP

#include "IGraph.hpp"


// хранящий граф в виде матрицы смежности
struct MatrixGraph : public IGraph {
    MatrixGraph(size_t size);

    MatrixGraph(const IGraph &);

    ~MatrixGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacency_matrix;
};

#endif //CPP_TASKS_MATRIXGRAPH_HPP
