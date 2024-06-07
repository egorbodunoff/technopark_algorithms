// Дано число N и N строк. Каждая строка содержащит команду добавления или удаления натуральных чисел,
// а также запрос на получение k-ой порядковой статистики. Команда добавления числа A задается
// положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
//  Запрос на получение k-ой порядковой статистики задается числом k.

#include <cassert>
#include <iostream>
#include <stack>

template <class T, class Compare>
class AvlTree {
   public:
    AvlTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}
    ~AvlTree();

    void insert(int key);
    void remove(int key);
    int getStatistic(const int& stat) const;

   private:
    struct node {
        int key;
        int elemNumber;
        unsigned char height;
        node* left;
        node* right;
        node(int k) {
            key = k;
            left = right = 0;
            height = 1;
            elemNumber = 1;
        }
    };

    unsigned char height(node* p);
    int bfactor(node* p);
    int getElemNumber(node* p) const;
    void fixheight(node* p);
    void fixElemNumber(node* node);
    node* rotateright(node* p);
    node* rotateleft(node* q);
    node* balance(node* p);
    std::pair<node*, node*> popMin(node* p);
    node* insertRecursive(node* p, int key);
    node* removeRecursive(node* p, int key);

    node* root;
    Compare cmp;
};

template <class T, class Compare>
AvlTree<T, Compare>::~AvlTree() {
    std::stack<node*> stack;
    node* current = root;

    while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }

        current = stack.top();
        stack.pop();

        node* right = current->right;

        delete current;

        current = right;
    }
}

template <class T, class Compare>
unsigned char AvlTree<T, Compare>::height(node* p) {
    return p != nullptr ? p->height : 0;
}

template <class T, class Compare>
int AvlTree<T, Compare>::bfactor(node* p) {
    return height(p->right) - height(p->left);
}

template <class T, class Compare>
int AvlTree<T, Compare>::getElemNumber(node* p) const {
    return p != nullptr ? p->elemNumber : 0;
}

template <class T, class Compare>
void AvlTree<T, Compare>::fixheight(node* p) {
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

template <class T, class Compare>
void AvlTree<T, Compare>::fixElemNumber(node* p) {
    p->elemNumber = getElemNumber(p->left) + getElemNumber(p->right) + 1;
}

template <class T, class Compare>
typename AvlTree<T, Compare>::node* AvlTree<T, Compare>::rotateright(node* p) {
    node* q = p->left;
    p->left = q->right;
    q->right = p;

    fixheight(p);
    fixElemNumber(p);
    fixheight(q);
    fixElemNumber(q);

    return q;
}

template <class T, class Compare>
typename AvlTree<T, Compare>::node* AvlTree<T, Compare>::rotateleft(node* q) {
    node* p = q->right;
    q->right = p->left;
    p->left = q;

    fixheight(q);
    fixElemNumber(q);
    fixheight(p);
    fixElemNumber(p);

    return p;
}

template <class T, class Compare>
typename AvlTree<T, Compare>::node* AvlTree<T, Compare>::balance(node* p) {
    fixheight(p);
    fixElemNumber(p);
    if (bfactor(p) == 2) {
        if (bfactor(p->right) < 0) p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if (bfactor(p) == -2) {
        if (bfactor(p->left) > 0) p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p;
}

template <class T, class Compare>
typename AvlTree<T, Compare>::node* AvlTree<T, Compare>::insertRecursive(node* p, int key) {
    if (!p) return new node(key);
    if (cmp(key, p->key))
        p->left = insertRecursive(p->left, key);
    else
        p->right = insertRecursive(p->right, key);
    return balance(p);
}

template <class T, class Compare>
std::pair<typename AvlTree<T, Compare>::node*, typename AvlTree<T, Compare>::node*>
AvlTree<T, Compare>::popMin(node* p) {
    if (p->left == nullptr) {
        return {p->right, p};
    }
    std::pair<node*, node*> removingMin = popMin(p->left);
    p->left = removingMin.first;
    return {balance(p), removingMin.second};
}

template <class T, class Compare>
typename AvlTree<T, Compare>::node* AvlTree<T, Compare>::removeRecursive(node* p, int key) {
    if (p == nullptr) {
        return nullptr;
    }
    if (cmp(key, p->key)) {
        p->left = removeRecursive(p->left, key);
    } else if (key == p->key) {
        node *left = p->left;
        node *right = p->right;
        delete p;
        if (right == nullptr) {
            return left;
        }
        std::pair<node*, node*> minPair = popMin(right);
        node *min = minPair.second;
        min->right = minPair.first;
        min->left = left;
        return balance(min);
    } else {
        p->right = removeRecursive(p->right, key);
    }
    return balance(p);
}

template <class T, class Compare>
void AvlTree<T, Compare>::insert(int key) {
    root = insertRecursive(root, key);
}

template <class T, class Compare>
void AvlTree<T, Compare>::remove(int key) {
    root = removeRecursive(root, key);
}

template <class T, class Compare>
int AvlTree<T, Compare>::getStatistic(const int &stat) const {
    int result = -1;
    if (stat < getElemNumber(root)) {
        node *current = root;
        int i = getElemNumber(root->left);
        while (i != stat) {
            if (stat > i) {
                current = current->right;
                i += getElemNumber(current->left) + 1;
            } else {
                current = current->left;
                i -= getElemNumber(current->right) + 1;
            }
        }
        result = current->key;
    } else if (getElemNumber(root) == stat) {
        return root->key;
    }
    return result;
}

struct IntCmp {
    bool operator()(int l, int r) { return l < r; }
};

int main() {
    IntCmp cmp;
    AvlTree<int, IntCmp> tree(cmp);
    size_t n;
    std::cin >> n;

    int key;
    int statisticNumber;
    while (n > 0) {
        std::cin >> key >> statisticNumber;
        if (key > 0) {
            tree.insert(key);
        } else {
            tree.remove(abs(key));
        }
        std::cout << tree.getStatistic(statisticNumber) << "\n";
        --n;
    }
    return 0;
}