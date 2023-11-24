#include <iostream>
#include <queue>


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

    size_t get_min_height() {
        using pair = std::pair<BinTreeNode *, int>;
        std::queue<pair> queue_nodes; // <node, cur_height>
        queue_nodes.push(pair(_root, 1));

        size_t min_height = 0 - 1;

        BinTreeNode *node;
        int height;
        while (!queue_nodes.empty()) {
            pair pr = queue_nodes.front();
            queue_nodes.pop();
            node = pr.first;
            height = pr.second;

            if (node->right == nullptr && node->left == nullptr) {
                min_height = height < min_height ? height : min_height;
            }
            if (node->right != nullptr) {
                queue_nodes.push(pair(node->right, height + 1));
            }
            if (node->left != nullptr) {
                queue_nodes.push(pair(node->left, height + 1));
            }
        }
        return min_height;
    }

private:
    Comparator cmp;
    BinTreeNode *_root;
};


int main() {
    BinTree<std::less<int> > tree;
    int num = 0;
    while (std::cin >> num) {
        tree.add(BinTreeNode(num));
    }
    std::cout << tree.get_min_height() << '\n';

    return 0;
}
