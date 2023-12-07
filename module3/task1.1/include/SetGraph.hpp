#ifndef CPP_TASKS_SETGRAPH_HPP
#define CPP_TASKS_SETGRAPH_HPP

#include "IGraph.hpp"
#include <set>


// хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска
struct SetGraph : public IGraph {
    SetGraph(size_t size);

    explicit SetGraph(const IGraph &);

    ~SetGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    int size;
    std::set<std::pair<int, int>> edges_set;
};


#endif //CPP_TASKS_SETGRAPH_HPP
