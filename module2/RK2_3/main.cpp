#include <iostream>
#include <unordered_set>
#include <functional>
#include <string>


struct Node {

    std::string product_name;
    int color;
    int size;

};

struct NodeEqual {
    bool operator()(Node &n1, Node &n2) const {
        return n1.product_name == n2.product_name &&
               n1.color == n2.color &&
               n1.size == n2.size;
    }
};

struct NodeHash {
    std::size_t operator()(Node &node) const {
        std::size_t h1 = std::hash<std::string>{}(node.product_name);
        std::size_t h2 = std::hash<int>{}(node.color);
        std::size_t h3 = std::hash<int>{}(node.size);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

int main() {
    std::unordered_set<Node, NodeHash, NodeEqual> customSet;

    char operation;
    Node node;

    while (std::cin >> operation >> node.product_name >> node.color >> node.size) {
        switch (operation) {
            case '+':
                std::cout << (customSet.insert(node).second ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (customSet.erase(node) > 0 ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (customSet.find(node) != customSet.end() ? "OK" : "FAIL") << std::endl;
                break;
        }
    }

    return 0;

}