#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <set>
#include <limits.h>


struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual int GetWeight(int from, int to) const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


struct MatrixGraph : public IGraph {
    explicit MatrixGraph(size_t size);

    explicit MatrixGraph(const IGraph &);

    ~MatrixGraph() override;

    void AddEdge(int from, int to, int weight) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

    int GetWeight(int from, int to) const override;

private:
    std::vector<std::vector<int>> adjacency_matrix;
};

MatrixGraph::MatrixGraph(size_t size) : adjacency_matrix(size, std::vector<int>(size, 0)) {};

MatrixGraph::MatrixGraph(const IGraph &graph) : MatrixGraph(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (int to: graph.GetNextVertices(from)) {
            AddEdge(from, to, graph.GetWeight(from, to));
        }
    }
}

MatrixGraph::~MatrixGraph() = default;

int MatrixGraph::GetWeight(int from, int to) const {
    return adjacency_matrix.at(from).at(to);
}

void MatrixGraph::AddEdge(int from, int to, int weight) {
    assert(0 <= from && from < adjacency_matrix.size());
    assert(0 <= to && to < adjacency_matrix.size());

    int exists_weight = GetWeight(from, to);
    if (exists_weight == 0 || exists_weight > weight) {
        adjacency_matrix.at(from).at(to) = weight;
        adjacency_matrix.at(to).at(from) = weight;
    }
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

int get_shortest_way(const IGraph &graph, int from, int to) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<int> prev(graph.VerticesCount(), -1);
    std::vector<int> dist(graph.VerticesCount(), INT_MAX);
    dist[from] = 0;

    auto cmp = [&dist](int v1, int v2) { return dist[v1] > dist[v2]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> que(cmp);

    que.push(from);
    while (!que.empty()) {
        int vertex = que.top();
        que.pop();

        if (visited[vertex]) {
            continue;
        }
        visited[vertex] = true;

        for (int v: graph.GetNextVertices(vertex)) {
            if (dist[v] > dist[vertex] + graph.GetWeight(vertex, v)) {
                dist[v] = dist[vertex] + graph.GetWeight(vertex, v);
                prev[v] = vertex;
                que.push(v);
            }
        }
    }
    return dist[to];
}

int main() {
    int N, M;
    std::cin >> N >> M;

    MatrixGraph graph(N);

    int from, to, weight;
    for (int i = 0; i < M; ++i) {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    std::cin >> from >> to;
    std::cout << get_shortest_way(graph, from, to) << std::endl;

    return 0;
}
