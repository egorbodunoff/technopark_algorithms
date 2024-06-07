// Постройте B-дерево минимального порядка t и выведите его по слоям.
// В качестве ключа используются числа, лежащие в диапазоне [0..232-1]
// Требования:
// B-дерево должно быть реализовано в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.

#include <iostream>
#include <vector>
#include <queue>

template <class T>
class BTree {
   public:
    BTree(size_t _t) : root(nullptr), t(_t) {}
    ~BTree() { delete root; }

    void Insert(const T& key);
    void PrintByLevels(void (*visit)(const std::vector<T>& v)) const;

   private:
    struct Node {
        bool IsLeaf;
        std::vector<T> Keys;
        std::vector<Node*> Children;

        Node(bool isLeaf) : IsLeaf(isLeaf) {}
        ~Node() {
            for (Node* child : Children) {
                delete child;
            }
        }
    };
    Node* root;
    size_t t;

    bool isNodeFull(Node* node) { return node->Keys.size() == 2 * t - 1; }
    void splitChild(Node* node, int pos);
    void insertNonFull(Node* node, const T& key);
};

template <class T>
void BTree<T>::Insert(const T& key) {
    if (root == nullptr) {
        root = new Node(true);
    }

    if (isNodeFull(root)) {
        Node* newRoot = new Node(false);
        newRoot->Children.push_back(root);
        root = newRoot;
        splitChild(root, 0);
    }

    insertNonFull(root, key);
}

template <class T>
void BTree<T>::insertNonFull(Node* node, const T& key) {
    int pos = node->Keys.size() - 1;
    if (node->IsLeaf) {
        node->Keys.resize(node->Keys.size() + 1);

        while (pos >= 0 && key < node->Keys[pos]) {
            node->Keys[pos + 1] = node->Keys[pos];
            --pos;
        }
        node->Keys[pos + 1] = key;
    } else {
        int pos = node->Keys.size() - 1;
        while (pos >= 0 && key < node->Keys[pos]) {
            --pos;
        }
        if (isNodeFull(node->Children[pos + 1])) {
            splitChild(node, pos + 1);
            if (key >= node->Keys[pos + 1]) {
                ++pos;
            }
        }
        insertNonFull(node->Children[pos + 1], key);
    }
}

template <class T>
void BTree<T>::splitChild(Node* node, int pos) {
    Node* oldChild = node->Children[pos]; 
    Node* newChild = new Node(oldChild->IsLeaf);

    for (std::size_t i = 0; i < t - 1; ++i) { 
        newChild->Keys.push_back(oldChild->Keys[i + t]); 
    } 
    if (!oldChild->IsLeaf) { 
        for (std::size_t i = 0; i < t; ++i) { 
            newChild->Children.push_back(oldChild->Children[i + t]);
        } 
    } 

    T oldKey = oldChild->Keys[t - 1]; 
    oldChild->Keys.resize(t - 1); 
    oldChild->Children.resize(t); 
    node->Children.resize(node->Children.size() + 1); 

    for (std::size_t i = node->Children.size()- 1; i > pos + 1; --i) { 
        node->Children[i] = node->Children[i - 1]; 
    } 

    node->Children[pos + 1] = newChild; 
    node->Keys.resize(node->Keys.size() + 1); 

    for (std::size_t i = node->Keys.size() - 1; i > pos; --i) { 
        node->Keys[i] = node->Keys[i - 1]; 
    } 

    node->Keys[pos] = oldKey;
}

template <class T>
void BTree<T>::PrintByLevels(void (*visit)(const std::vector<T>& v)) const {
    if (root == nullptr) {
        return;
    }

    std::queue<Node*> levelQueue;
    levelQueue.push(root);

    while (!levelQueue.empty()) {
        size_t levelSize = levelQueue.size();
        std::vector<T> row;
        
        for (size_t i = 0; i < levelSize; ++i) {
            Node* currentNode = levelQueue.front();
            levelQueue.pop();
            
            for (const auto& key : currentNode->Keys) {
                row.push_back(key);
            }
            
            if (!currentNode->IsLeaf) {
                for (Node* child : currentNode->Children) {
                    levelQueue.push(child);
                }
            }
        }
        visit(row);
    }
}

int main() {
    int t, k = 0;
    std::cin >> t;

    BTree<int> tree(t);
    while(std::cin >> k) {
        tree.Insert(k);
    }

    auto printVector = [](const std::vector<int>& v) {
        for (const auto& element : v) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    };
    tree.PrintByLevels(printVector);

    return 0;
}