#include <iostream>
#include <optional>
#include <cassert>

template<class Comp = std::less<int> >
struct AVLTree {
    struct Node {
        explicit Node(int val) : value(val), height(1), count(1),
                                 left(nullptr), right(nullptr) {}

        Node(Node &) = delete;

        Node &operator=(Node &) = delete;

        ~Node() {
            delete left;
            delete right;
        }

        int value;
        int height;
        int count;
        Node *left;
        Node *right;
    };

    AVLTree() : _root(nullptr) {}

    AVLTree(AVLTree &) = delete;

    AVLTree &operator=(AVLTree &) = delete;

    ~AVLTree() {
        delete _root;
    }

    void insert(int value) {
        _insert(_root, value);
        fix_count(_root);
    }

    void exclude(int value) {
        _exclude(_root, value);
        fix_count(_root);
    }

    bool exists(int value) {
        if (_root == nullptr) {
            return false;
        }
        Node *node = _root;
        while (node->value != value) {
            if (cmp(value, node->value)) {
                if (node->left == nullptr) {
                    return false;
                }
                node = node->left;
            } else {
                if (node->right == nullptr) {
                    return false;
                }
                node = node->right;
            }
        }
        return true;
    }

    std::optional<int> next(int value) {
        std::optional<int> result = std::nullopt;
        if (_root == nullptr) {
            return result;
        }
        Node *node = _root;
        while (true) {
            if (cmp(value, node->value) && value != node->value) {
                result = node->value;
                if (node->left == nullptr) {
                    return result;
                }
                node = node->left;
            } else if (!cmp(value, node->value) && value != node->value) {
                if (node->right == nullptr) {
                    return result;
                }
                node = node->right;
            } else {
                if (node->right == nullptr) {
                    return result;
                }
                node = node->right;
            }
        }
    }

    std::optional<int> prev(int value) {
        std::optional<int> result = std::nullopt;
        if (_root == nullptr) {
            return result;
        }
        Node *node = _root;
        while (true) {
            if (cmp(node->value, value) && value != node->value) {
                result = node->value;
                if (node->right == nullptr) {
                    return result;
                }
                node = node->right;
            } else if (!cmp(value, node->value) && value != node->value) {
                if (node->left == nullptr) {
                    return result;
                }
                node = node->left;
            } else {
                if (node->left == nullptr) {
                    return result;
                }
                node = node->left;
            }
        }
    }

private:
    int count(Node *node) {
        return (node == nullptr) ? 0 : count(node->left) + count(node->right);
    }

    static void right_rotate(Node *&node) {
        assert(node->left != nullptr);
        Node *A_node = node;
        Node *B_node = node->left;
        A_node->left = B_node->right;
        B_node->right = A_node;
        node = B_node;
    }

    static void left_rotate(Node *&node) {
        assert(node->right != nullptr);
        Node *A_node = node;
        Node *B_node = node->right;
        A_node->right = B_node->left;
        B_node->left = A_node;
        node = B_node;
    }

    static void large_right_rotate(Node *&node) {
        left_rotate(node->left);
        right_rotate(node);
    }

    static void large_left_rotate(Node *&node) {
        right_rotate(node->right);
        left_rotate(node);
    }

    void _insert(Node *&node, int value) {
        if (node == nullptr) {
            node = new Node(value);
            return;
        }
        if (cmp(value, node->value) && value != node->value) {
            _insert(node->left, value);
        } else if (value != node->value) {
            _insert(node->right, value);
        } else {
            return;
        }

        balance(node);
    }

    int get_height(Node *node) const {
        return node == nullptr ? 0 : node->height;
    }

    int get_count(Node *node) const {
        return node == nullptr ? 0 : node->count;
    }

    int fix_count(Node *node) {
        if (node == nullptr) {
            return 0;
        }
        node->count = fix_count(node->left) + fix_count(node->right) + 1;
        return node->count;
    }

    int get_balance(Node *node) {
        return get_height(node->left) - get_height(node->right);
    }

    void balance(Node *&node) {
        node->height = std::max(get_height(node->left), get_height(node->right)) + 1;

        switch (get_balance(node)) {
            case -2:
                if (get_balance(node->right) > 0) {
                    large_left_rotate(node);
                } else {
                    left_rotate(node);
                }
                break;
            case 2:
                if (get_balance(node->left) < 0) {
                    large_right_rotate(node);
                } else {
                    right_rotate(node);
                }
                break;
        }
    }

    Node *pop_minimal(Node *&node) {
        if (node->left != nullptr) {
            return pop_minimal(node->left);
        }

        Node *min_node = node;
        node = node->right;
        min_node->right = nullptr;
        return min_node;
    }

    void _exclude(Node *&node, int value) {
        if (cmp(value, node->value)) {
            if (node->left == nullptr) {
                return;
            }
            _exclude(node->left, value);
        } else if (!cmp(value, node->value) && node->value != value) {
            if (node->right == nullptr) {
                return;
            }
            _exclude(node->right, value);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            } else if (node->left == nullptr) {
                Node *tmp = node;
                node = node->right;
                tmp->right = nullptr;
                delete tmp;
            } else if (node->right == nullptr) {
                Node *tmp = node;
                node = node->left;
                tmp->left = nullptr;
                delete tmp;
            } else {
                Node *min_node = pop_minimal(node->right);
                min_node->left = node->left;
                min_node->right = node->right;

                Node *deleting_node = node;
                node = min_node;

                deleting_node->right = nullptr;
                deleting_node->left = nullptr;
                delete deleting_node;
            }
            return;
        }

        balance(node);
    }

    int _get_order_stat(Node *node, int k_index) const {
        int left_count = get_count(node->left);
        if (k_index <= left_count - 1) {
            return _get_order_stat(node->left, k_index);
        } else if (k_index == left_count) {
            return node->value;
        } else {
            return _get_order_stat(node->right, k_index - get_count(node->left) - 1);
        }
    }

    Node *_root;
    Comp cmp;
};


int main() {
    std::string command;
    int number;

    AVLTree<std::less<int> > tree;
    while (std::cin >> command >> number) {
        if (command == "insert") {
            tree.insert(number);
        } else if (command == "delete") {
            tree.exclude(number);
        } else if (command == "exists") {
            // если ключ x есть в дереве, вывести «true», иначе «false»
            std::cout << (tree.exists(number) ? "true" : "false") << std::endl;
        } else if (command == "next") {
            // минимальный элемент в дереве, больший x, или «none», если такого нет
            std::optional<int> result = tree.next(number);
            if (result == std::nullopt) {
                std::cout << "none" << std::endl;
            } else {
                std::cout << result.value() << std::endl;
            }
        } else if (command == "prev") {
            // максимальный элемент в дереве, меньший x, или «none», если такого нет.
            std::optional<int> result = tree.prev(number);
            if (result == std::nullopt) {
                std::cout << "none" << std::endl;
            } else {
                std::cout << result.value() << std::endl;
            }
        }
    }

    return 0;
}
