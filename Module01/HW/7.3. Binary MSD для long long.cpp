// Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 1000000.
// Отсортировать массив методом MSD по битам (бинарный QuickSort).

#include <iostream>

bool digit_by_index(unsigned long long num, int idx) { return (num >> idx) & 1; }

void bin_quick_sort(unsigned long long* arr, int first, int last, int r) {
    if (last <= first || r < 0) {
        return;
    }
    int i = first, j = last;
    while (i < j) {
        for (; !digit_by_index(arr[i], r) && i < j; ++i) {
        }
        for (; digit_by_index(arr[j], r) && j > i; --j) {
        }
        if (i < j) {
            std::swap(arr[i], arr[j]);
        }
    }
    if (!digit_by_index(arr[last], r)) {
        ++j;
    }
    bin_quick_sort(arr, first, j - 1, r - 1);
    bin_quick_sort(arr, j, last, r - 1);
}

int main() {
    int len;
    std::cin >> len;
    unsigned long long* arr = new unsigned long long[len];
    for (int i = 0; i < len; ++i) {
        std::cin >> arr[i];
    }

    bin_quick_sort(arr, 0, len - 1, 63);
    for (int i = 0; i < len; ++i) {
        std::cout << arr[i] << ' ';
    }
    delete[] arr;
}