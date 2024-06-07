// Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е.,
// при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Выведите элементы в порядке in-order (слева направо).

#include <cassert>
#include <iostream>
#include <stack>

template <class T, class Compare>
class BinaryTree {
   public:
    BinaryTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}
    ~BinaryTree();
    BinaryTree(const BinaryTree& tree);
    BinaryTree& operator=(const BinaryTree& tree);

    void Add(const T& key);
    bool Has(const T& key) const;

    void Dfs(void (*visit)(const T& key)) const;

   private:
    struct Node {
        T Key;
        Node* left;
        Node* right;

        Node(const T& key) : Key(key), left(nullptr), right(nullptr) {}
    };

    Node* root;
    Compare cmp;
};

template <class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    std::stack<Node*> stack;
    Node* current = root;

    while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }
        
        current = stack.top();
        stack.pop();

        Node* right = current->right;

        delete current;

        current = right;
    }
}

template <class T, class Compare>
void BinaryTree<T, Compare>::Add(const T& key) {
    Node* newNode = new Node(key);
    if (root == nullptr) {
        root = newNode;
        return;
    }

    Node* current = root;
    while (true) {
        if (cmp(key, current->Key)) {
            if (current->left == nullptr) {
                current->left = newNode;
                break;
            } else {
                current = current->left;
            }
        } else {
            if (current->right == nullptr) {
                current->right = newNode;
                break;
            } else {
                current = current->right;
            }
        }
    }
}

template <class T, class Compare>
bool BinaryTree<T, Compare>::Has(const T& key) const {
    Node* current = root;
    while (current != nullptr) {
        if (cmp(key, current->Key)) {
            current = current->left;
        } else if (cmp(current->Key, key)) {
            current = current->right;
        } else {
            return true;
        }
    }
    return false;
}

template <class T, class Compare>
void BinaryTree<T, Compare>::Dfs(void (*visit)(const T& key)) const {
    std::stack<Node*> nodesStack;
    Node* current = root;

    while (current != nullptr || !nodesStack.empty()) {
        while (current != nullptr) {
            nodesStack.push(current);
            current = current->left;
        }

        current = nodesStack.top();
        nodesStack.pop();
        visit(current->Key);

        current = current->right;
    }
}

struct IntCmp {
    bool operator()(int l, int r) { return l < r; }
};

int main() {
    IntCmp cmp;
    BinaryTree<int, IntCmp> tree(cmp);

    size_t count = 0;
    std::cin >> count;

    int elem = 0;
    for (size_t i = 0; i < count; i++) {
        std::cin >> elem;
        tree.Add(elem);
    }
    tree.Dfs([](const int& key) { std::cout << key << std::endl; });
    return 0;
}