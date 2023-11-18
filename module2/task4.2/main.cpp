#include <cassert>
#include <iostream>

/*
4_2. Порядковые статистики. Дано число N и N строк. Каждая строка содержит команду добавления или удаления
натуральных чисел, а также запрос на получение k-ой порядковой статистики. Команда добавления числа A
задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
Запрос на получение k-ой порядковой статистики задается числом k.

Требования: скорость выполнения запроса - O(log n).

 */


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

    int get_order_stat(int k_index) const {
        return _get_order_stat(_root, k_index);
    }

    Node *get_head() {
        return _root;
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
        if (cmp(value, node->value)) {
            _insert(node->left, value);
        } else {
            _insert(node->right, value);
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
            _exclude(node->left, value);
        } else if (!cmp(value, node->value) && node->value != value) {
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

void avl_tree_test() {
    { // deleting nodes test
        AVLTree tree;
        tree.insert(2);
        tree.insert(1);
        tree.insert(3);

        tree.exclude(1);
        assert(tree.get_head()->left == nullptr);
        assert(tree.get_head()->value == 2);
        assert(tree.get_head()->right->value == 3);

        tree.exclude(3);
        assert(tree.get_head()->left == nullptr);
        assert(tree.get_head()->value == 2);
        assert(tree.get_head()->right == nullptr);

        tree.exclude(2);
        assert(tree.get_head() == nullptr);
    }
    {
        AVLTree tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        tree.insert(20);
        tree.insert(7);
        tree.insert(8);
        tree.insert(3);

        tree.exclude(5);

        AVLTree<std::less<int>>::Node *root = tree.get_head();
        assert(root->value == 10);
        assert(root->left->value == 7);
        assert(root->left->left->value == 3);
        assert(root->left->right->value == 8);

        assert(root->right->value == 15);
        assert(root->right->right->value == 20);
    }
    { //  right rotate
        AVLTree tree;
        tree.insert(5);
        tree.insert(3);
        tree.insert(1);

        assert(tree.get_head()->left->value == 1);
        assert(tree.get_head()->value == 3);
        assert(tree.get_head()->right->value == 5);
    }
    { // large right rotate
        AVLTree tree;
        tree.insert(5);
        tree.insert(3);
        tree.insert(4);

        assert(tree.get_head()->left->value == 3);
        assert(tree.get_head()->value == 4);
        assert(tree.get_head()->right->value == 5);
    }
    { //  left rotate
        AVLTree tree;
        tree.insert(5);
        tree.insert(7);
        tree.insert(9);

        assert(tree.get_head()->left->value == 5);
        assert(tree.get_head()->value == 7);
        assert(tree.get_head()->right->value == 9);
    }
    { // large left rotate
        AVLTree tree;
        tree.insert(5);
        tree.insert(7);
        tree.insert(6);

        assert(tree.get_head()->left->value == 5);
        assert(tree.get_head()->value == 6);
        assert(tree.get_head()->right->value == 7);
    }
    { // deleting nodes test with rotate
        AVLTree tree;
        tree.insert(5);
        tree.insert(7);
        tree.insert(3);
        tree.insert(1);

        tree.exclude(7);
        assert(tree.get_head()->left->value == 1);
        assert(tree.get_head()->value == 3);
        assert(tree.get_head()->right->value == 5);
    }
    {
        AVLTree tree;
        for (int i = 0; i < 10; ++i) {
            tree.insert(i);
            assert(tree.get_head()->count == i + 1);
        }
        for (int i = 0; i < 10; ++i) {
            assert(tree.get_order_stat(i) == i);
        }
    }
    {
        AVLTree tree;
        for (int i = 0; i < 10; ++i) {
            tree.insert(i);
            assert(tree.get_head()->count == i + 1);
        }
        for (int i = 0; i < 10; ++i) {
            assert(tree.get_order_stat(i) == i);
        }
    }
    {
        AVLTree tree;

        tree.insert(40);
        assert(tree.get_order_stat(0) == 40);
        tree.insert(10);
        assert(tree.get_order_stat(1) == 40);
        tree.insert(4);
        assert(tree.get_order_stat(1) == 10);
        tree.exclude(10);
        assert(tree.get_order_stat(0) == 4);
        tree.insert(50);
        assert(tree.get_order_stat(2) == 50);
    }
    {
        AVLTree tree;
        tree.insert(5);
        tree.insert(1);
        tree.insert(5);
        tree.insert(5);

        assert(tree.get_order_stat(0) == 1);
        assert(tree.get_order_stat(1) == 5);
        assert(tree.get_order_stat(2) == 5);
        assert(tree.get_order_stat(3) == 5);

        tree.exclude(5);
        tree.exclude(5);

        assert(tree.get_order_stat(0) == 1);
        assert(tree.get_order_stat(1) == 5);
    }
    std::cout << "Success tests!" << std::endl;
}

int main() {
//    avl_tree_test();

    int N;
    std::cin >> N;

    AVLTree avl_tree;

    int number, k_index;
    for (int i = 0; i < N; ++i) {
        std::cin >> number >> k_index;

        if (number < 0) {
            avl_tree.exclude(-number);
        } else {
            avl_tree.insert(number);
        }

        std::cout << avl_tree.get_order_stat(k_index) << std::endl;
    }
    return 0;
}
