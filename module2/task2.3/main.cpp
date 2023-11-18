#include <iostream>
#include <queue>
#include <stack>

struct BinTreeNode {
    explicit BinTreeNode(int val) : val(val), left(nullptr), right(nullptr) {};

    BinTreeNode(const BinTreeNode &node) : val(node.val),
                                           left(node.left),
                                           right(node.right) {};

    ~BinTreeNode() = default;

    int val;
    BinTreeNode *left;
    BinTreeNode *right;


};

template<class Comparator = std::less<int> >
struct BinTree {
    BinTree() : _root(nullptr) {};

    ~BinTree() {
        if (_root == nullptr) {
            return;
        }
        std::queue<BinTreeNode *> que;

        que.push(_root);
        while (!que.empty()) {
            BinTreeNode *node = que.front();
            que.pop();
            if (node->right != nullptr) {
                que.push(node->right);
            }
            if (node->left != nullptr) {
                que.push(node->left);
            }
            delete node;
        }
    }

    void add(BinTreeNode &&node) {
        if (_root == nullptr) {
            _root = new BinTreeNode(node);
            return;
        }
        BinTreeNode *cur_node = _root;
        while (true) {
            if (cmp(node.val, cur_node->val)) {
                if (cur_node->left == nullptr) {
                    cur_node->left = new BinTreeNode(node);
                    return;
                } else {
                    cur_node = cur_node->left;
                }
            } else {
                if (cur_node->right == nullptr) {
                    cur_node->right = new BinTreeNode(node);
                    return;
                } else {
                    cur_node = cur_node->right;
                }
            }
        }
    }

    void post_order_print() {
        BinTreeNode *rt = _root;
        std::stack<BinTreeNode *> st_nodes;
        while (true) {
            while (rt) {
                st_nodes.push(rt);
                st_nodes.push(rt);
                rt = rt->left;
            }
            if (st_nodes.empty()) { return; }
            rt = st_nodes.top();
            st_nodes.pop();
            if (!st_nodes.empty() && st_nodes.top() == rt)
                rt = rt->right;
            else {
                std::cout << rt->val << ' ';
                rt = nullptr;
            }
        }
    }

private:
    Comparator cmp;
    BinTreeNode *_root;
};

int main() {
    int N;
    std::cin >> N;

    BinTree bt;

    int item = 0;
    for (int i = 0; i < N; ++i) {
        std::cin >> item;
        bt.add(BinTreeNode(item));
    }
    bt.post_order_print();

    return 0;
}
