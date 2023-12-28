#include <iostream>
#include <cassert>
#include <queue>


struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


struct ListGraph : public IGraph {
    ListGraph(size_t size) : adjacency_list(size) {};

    ListGraph(const IGraph &graph) : ListGraph(graph.VerticesCount()) {
        for (int from = 0; from < graph.VerticesCount(); ++from) {
            for (int to: graph.GetNextVertices(from)) {
                AddEdge(from, to);
            }
        }
    }

    ~ListGraph() override = default;

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < adjacency_list.size());
        assert(0 <= to && to < adjacency_list.size());
        adjacency_list.at(from).push_back(to);
        adjacency_list.at(to).push_back(from);
    }

    int VerticesCount() const override {
        return adjacency_list.size();
    }


    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacency_list.size());

        return adjacency_list.at(vertex);
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
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

private:
    std::vector<std::vector<int>> adjacency_list;
};


int BFS(const IGraph &graph, int from, int to) {
    std::vector<size_t> distances(graph.VerticesCount(), ~0ull);
    std::vector<int> ways(graph.VerticesCount(), 0);
    std::vector<bool> visited(graph.VerticesCount(), false);

    visited[from] = true;

    distances[from] = 0;
    ways[from] = 1;

    std::queue<int> queue;
    queue.push(from);


    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int next: graph.GetNextVertices(current)) {
            if (distances[current] + 1 == distances[next]) {
                ways[next] += ways[current];
            }
            if (!visited[next]) {
                if (distances[current] + 1 < distances[next]) {
                    distances[next] = distances[current] + 1;
                    ways[next] = ways[current];
                }
                visited[next] = true;
                queue.push(next);
            }
        }
    }
    return ways[to];
}

void test() {
    ListGraph graph(4);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(2, 3);

    std::cout << BFS(graph, 0, 3);
}

int main() {
    size_t size;
    std::cin >> size;
    ListGraph graph(size);

    std::cin >> size;
    int from, to;
    for (size_t i = 0; i < size; ++i) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::cin >> from >> to;
    std::cout << BFS(graph, from, to) << '\n';

    return 0;
}
