#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <unordered_map>
#include <climits>

struct PairHasher {
    size_t operator()(const std::pair<int, int> &pr) const {
        return std::hash<int>{}(pr.first + pr.second);
    }
};

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual int GetWeight(int from, int to) const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

struct MapGraph : public IGraph {
    MapGraph(size_t size);

    explicit MapGraph(const IGraph &);

    ~MapGraph() override;

    void AddEdge(int from, int to, int weight) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

    int GetWeight(int from, int to) const override;

private:
    int size;
    std::unordered_map<std::pair<int, int>, int, PairHasher> edges_map;
};

std::pair<int, int> get_pair(int from, int to) {
    if (from < to) {
        return {from, to};
    } else {
        return {to, from};
    };
}

MapGraph::MapGraph(size_t size) : size(size) {};

MapGraph::MapGraph(const IGraph &graph) : MapGraph(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (int to: graph.GetNextVertices(from)) {
            AddEdge(from, to, graph.GetWeight(from, to));
        }
    }
}

MapGraph::~MapGraph() = default;

void MapGraph::AddEdge(int from, int to, int weight) {
    assert(0 <= from && from < size);
    assert(0 <= to && to < size);

    std::pair<int, int> pr = get_pair(from, to);

    if (GetWeight(from, to) > weight) {
        edges_map[pr] = weight;
    }
}

int MapGraph::VerticesCount() const {
    return size;
}

std::vector<int> MapGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < size);

    return GetPrevVertices(vertex);
}

std::vector<int> MapGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < size);

    std::vector<int> result;
    for (const auto &[k, w]: edges_map) {
        if (k.second == vertex) {
            result.push_back(k.first);
        }
        if (k.first == vertex) {
            result.push_back(k.second);
        }
    }
    return result;
}

int MapGraph::GetWeight(int from, int to) const {
    assert(0 <= from && from < size);
    assert(0 <= to && to < size);

    std::pair<int, int> pr = get_pair(from, to);

    if (edges_map.find(pr) == edges_map.end()) {
        return INT_MAX;
    }
    return edges_map.at(pr);
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

    MapGraph graph(N);

    int from, to, weight;
    for (int i = 0; i < M; ++i) {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    std::cin >> from >> to;
    std::cout << get_shortest_way(graph, from, to) << std::endl;

    return 0;
}
