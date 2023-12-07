#ifndef CPP_TASKS_ARCGRAPH_HPP
#define CPP_TASKS_ARCGRAPH_HPP

#include "IGraph.hpp"


// хранящий граф в виде одного массива пар {from, to}
struct ArcGraph : public IGraph {
    ArcGraph(size_t size);

    explicit ArcGraph(const IGraph &);

    ~ArcGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    int size;
    std::vector<std::pair<int, int>> edges;
};

#endif //CPP_TASKS_ARCGRAPH_HPP
