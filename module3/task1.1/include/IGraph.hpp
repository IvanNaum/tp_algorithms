#ifndef CPP_TASKS_IGRAPH_HPP
#define CPP_TASKS_IGRAPH_HPP

#include <vector>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

#endif //CPP_TASKS_IGRAPH_HPP
