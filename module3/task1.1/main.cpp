#include <iostream>

#include "include/ArcGraph.hpp"
#include "include/ListGraph.hpp"
#include "include/MatrixGraph.hpp"
#include "include/SetGraph.hpp"

#include <cassert>


#define TEST_SIZE 5
std::vector<std::pair<int, int>> test_edges = {
        {0, 1},
        {0, 2},
        {1, 2},
        {1, 3},
        {1, 4},
        {2, 4},
        {2, 1},
        {3, 2},
};

void add_test_edges(IGraph &graph) {
    for (auto &k: test_edges) {
        graph.AddEdge(k.first, k.second);
    }
}

void test_graph(IGraph &graph) {
    add_test_edges(graph);

    assert(graph.VerticesCount() == TEST_SIZE);

    for (int i = 0; i < TEST_SIZE; ++i) {
        {
            auto next = graph.GetNextVertices(i);
            for (int k: next) {
                assert(std::find(test_edges.begin(), test_edges.end(),
                                 std::pair<int, int>{i, k}) != test_edges.end());
            }
        }

        {
            auto prev = graph.GetPrevVertices(i);
            for (int k: prev) {
                assert(std::find(test_edges.begin(), test_edges.end(),
                                 std::pair<int, int>{k, i}) != test_edges.end());
            }
        }
    }
}

void test() {
    {
        ArcGraph graph(TEST_SIZE);
        test_graph(graph);
    }
    {
        ListGraph graph(TEST_SIZE);
        test_graph(graph);
    }
    {
        MatrixGraph graph(TEST_SIZE);
        test_graph(graph);
    }
    {
        SetGraph graph(TEST_SIZE);
        test_graph(graph);
    }

    {
        ListGraph graph(TEST_SIZE);
        add_test_edges(graph);
        test_graph(graph);

        ArcGraph arc_gr(graph);
        test_graph(arc_gr);
    }

    {
        ArcGraph graph(TEST_SIZE);
        add_test_edges(graph);
        test_graph(graph);

        ListGraph list_gr(graph);
        test_graph(list_gr);

        MatrixGraph matrix_gr(graph);
        test_graph(matrix_gr);

        SetGraph set_gr(graph);
        test_graph(set_gr);
    }

    std::cout << std::endl << "Success." << std::endl;
}

int main() {
    test();

    return 0;
}
