// Вернуть значение бита в числе N по его номеру K

#include <cassert>
#include <iostream>
#include <sstream>

int bit_value(int n, int k) {
    assert(k >= 0 && k <= 31);
    return (n >> k) & 1;
}

int main() {
    int n, k = 0;
    std::cin >> n >> k;
    std::cout << bit_value(n, k) << std::endl;
    return 0;
}