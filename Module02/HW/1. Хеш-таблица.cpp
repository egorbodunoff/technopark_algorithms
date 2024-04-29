#include <cassert>
#include <iostream>
#include <string>
#include <vector>

template <class T, class H, class H2>
class HashTable {
   public:
    HashTable(int initialSize, const H &_hasher, const H2 &_hasher2)
        : hasher(_hasher),
          hasher2(_hasher2),
          table(initialSize, HashTableCell()),
          keysDeleted(0),
          keysCount(0) {}
    ~HashTable();
    HashTable(const HashTable& h);
    HashTable& operator=(const HashTable& h);

    bool Has(const T &key);
    bool Add(const T &key);
    bool Delete(const T &key);

   private:
    H hasher;
    H2 hasher2;
    enum CellState { Empty, Key, Deleted };
    struct HashTableCell {
        T Key;
        unsigned int Hash;
        CellState State;
        HashTableCell() : Hash(0), State(Empty) {}
    };

    std::vector<HashTableCell> table;
    unsigned int keysCount;
    unsigned int keysDeleted;

    void growTable();
};

template <class T, class H, class H2>
HashTable<T, H, H2>::~HashTable() {
    table.clear();
}

template <class T, class H, class H2>
HashTable<T, H, H2>::HashTable(const HashTable& h) {
    keysCount = h.keysCount;
    keysDeleted = h.keysDeleted;
    table = std::move(h.table);
    hasher = h.hasher;
    hasher2 = h.hasher2;
};

template <class T, class H, class H2>
HashTable<T, H, H2>&HashTable<T, H, H2>::operator=(const HashTable& h) {
    if (&h != this) {
        keysCount = h.keysCount;
        keysDeleted = h.keysDeleted;
        table = std::move(h.table);
        hasher = h.hasher;
        hasher2 = h.hasher2;
    }
    return *this;
}

template <class T, class H, class H2>
bool HashTable<T, H, H2>::Has(const T &key) {
    unsigned int hash = hasher(key) % table.size();
    if (table[hash].State == Empty) {
        return false;
    }

    unsigned int hash2 = hasher2(key) % table.size();
    while (true) {
        if (table[hash].State == Key && table[hash].Key == key) {
            return true;
        }
        if (table[hash].State == Empty) {
            return false;
        }
        hash = (hash + hash2) % table.size();
    }
}

template <class T, class H, class H2>
bool HashTable<T, H, H2>::Add(const T &key) {
    if (keysCount > 0.6 * table.size() || keysDeleted > 0.3 * table.size()) {
        growTable();
    }
    unsigned int hash = hasher(key) % table.size();
    int firstDeleted = -1;
    while (true) {
        if (table[hash].State == Key && table[hash].Key == key) {
            return false;
        }
        if (table[hash].State == Empty) {
            if (firstDeleted < 0) {
                firstDeleted = hash;
            }
            table[firstDeleted].Key = key, table[firstDeleted].State = Key,
            table[firstDeleted].Hash = firstDeleted;
            ++keysCount;
            return true;
        }
        if (table[hash].State == Deleted && firstDeleted < 0) {
            firstDeleted = hash;
            --keysDeleted;
        }
        if (table[hash].State == Key && table[hash].Key == key) {
            return false;
        }
        hash = (hash + (hasher2(key) % table.size())) % table.size();
    }
}

template <class T, class H, class H2>
bool HashTable<T, H, H2>::Delete(const T &key) {
    unsigned int hash = hasher(key) % table.size();
    if (table[hash].State == Empty) {
        return false;
    }

    unsigned int hash2 = hasher2(key) % table.size();
    while (true) {
        if (table[hash].State == Key && table[hash].Key == key) {
            table[hash].State = Deleted;
            --keysCount;
            ++keysDeleted;
            return true;
        }
        if (table[hash].State == Empty) {
            return false;
        }
          if (table[hash].State == Deleted && table[hash].Key == key) {
            return false;
        }
        hash = (hash + hash2) % table.size();
    }
}

template <class T, class H, class H2>
void HashTable<T, H, H2>::growTable() {
    std::vector<HashTableCell> newTable(table.size() * 2, HashTableCell());
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].State == Deleted || table[i].State == Empty) {
            continue;
        }
        unsigned int listIndex = hasher(table[i].Key) % newTable.size();
        while (true) {
            if (newTable[listIndex].State == Empty) {
                newTable[listIndex].State = Key;
                newTable[listIndex].Key = table[i].Key;
                newTable[listIndex].Hash = listIndex;
                break;
            }
            listIndex = (listIndex + (hasher2(table[i].Key) % newTable.size())) % newTable.size();
        }
    }
    keysDeleted = 0;
    table = std::move(newTable);
}

struct StringHasher {
    unsigned int operator()(const std::string &key) const {
        unsigned int hash = 0;
        for (unsigned int i = 0; i < key.size(); ++i) {
            hash = hash * 137 + key[i];
        }
        return hash;
    }
};

struct StringHasher2 {
    unsigned int operator()(const std::string &key) const {
        unsigned int hash = 5;
        for (unsigned int i = 0; i < key.size(); ++i) {
            hash += key[i];
        }
        return 2 * hash + 1;
    }
};

int main() {
    StringHasher hasher;
    StringHasher2 hasher2;
    HashTable<std::string, StringHasher, StringHasher2> table(2, hasher, hasher2);
    char operation = 0;
    std::string word;
    while (std::cin >> operation >> word) {
        switch (operation) {
            case '+':
                std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);
        }
    }
    return 0;
}