// Дано множество целых чисел из [0..10^9] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
// 10%  перцентиль
// медиана
// 90%  перцентиль

// 6_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
// Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

#include <iostream>

template <class T>
class IsLessDefault {
   public:
    bool operator()(const T& l, const T& r) { return l < r; }
};

template <class T, class IsLess = IsLessDefault<T>>
int partition(T* arr, int l, int r, IsLess isLess = IsLess()) {
    if (l != r) {
        int pivot_idx = l + std::rand() % (r - l + 1);
        std::swap(arr[pivot_idx], arr[l]);
    }
    T pivot = arr[l];
    int i = r + 1;
    for (int j = r; j >= l; --j) {
        if (!isLess(arr[j], pivot)) {
            std::swap(arr[--i], arr[j]);
        }
    }
    return i;
}

template <class T, class IsLess = IsLessDefault<T>>
int find_stats(T* arr, int left, int right, int k_stat, IsLess isLess = IsLess()) {
    while (true) {
        int pivot_pos = partition(arr, left, right, isLess);

        if (pivot_pos == k_stat) {
            return arr[pivot_pos];
        }
        if (pivot_pos < k_stat) {
            left = pivot_pos + 1;
        } else {
            right = pivot_pos;
        }
    }
}

template <class T, class IsLess = IsLessDefault<T>>
void run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;
    T* arr = new T[n];

    for (int i = 0; i < n; ++i) {
        input >> arr[i];
    }
    output << find_stats<T>(arr, 0, n - 1, n / 10) << std::endl;
    output << find_stats<T>(arr, n * 0.1, n - 1, n / 2) << std::endl;
    output << find_stats<T>(arr, n * 0.5, n - 1, n * 0.9) << std::endl;
    delete[] arr;
}

int main() {
    run<int>(std::cin, std::cout);
    return 0;
}