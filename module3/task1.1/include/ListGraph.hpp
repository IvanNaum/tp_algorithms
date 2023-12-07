#ifndef CPP_TASKS_LISTGRAPH_HPP
#define CPP_TASKS_LISTGRAPH_HPP

#include "IGraph.hpp"
#include <vector>


// хранящий граф в виде массива списков смежности
struct ListGraph : public IGraph {
    ListGraph(size_t size);

    ListGraph(const IGraph &);

    ~ListGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacency_list;
};

#endif //CPP_TASKS_LISTGRAPH_HPP
