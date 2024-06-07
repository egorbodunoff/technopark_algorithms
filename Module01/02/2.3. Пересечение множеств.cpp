// Даны два массива неубывающих целых чисел, упорядоченные по возрастанию. A[0..n-1] и B[0..m-1]. n >> m.
// Найдите их пересечение.

#include <cassert>
#include <iostream>
#include <sstream>

// Класс «Динамический массив».
class CArray {
   public:
    CArray() : buffer(0), bufferSize(0), realSize(0) {}
    ~CArray() { delete[] buffer; }

    // Размер.
    int Size() const { return realSize; }

    // Доступ по индексу.
    double GetAt(int index) const;
    double operator[](int index) const { return GetAt(index); }

    // Добавление нового элемента.
    void Add(double element);
    void swap(int el1, int el2) {std::swap(buffer[el1], buffer[el2]);}

   private:
    double *buffer;  // Буфер.
    int bufferSize;  // Размер буфера.
    int realSize;    // Количество элементов в массиве.

    void grow();
};

// Доступ к элементу.
double CArray::GetAt(int index) const {
    assert(index >= 0 && index < realSize && buffer != 0);
    return buffer[index];
}

// Увеличение буфера.
void CArray::grow() {
    int newBufferSize = std::max(bufferSize * 2, 1);
    double *newBuffer = new double[newBufferSize];

    std::copy(buffer, buffer + realSize, newBuffer);

    delete[] buffer;
    buffer = newBuffer;
    bufferSize = newBufferSize;
}
// Добавление элемента.
void CArray::Add(double element) {
    if (realSize == bufferSize) grow();

    assert(realSize < bufferSize && buffer != 0);
    buffer[realSize++] = element;
}

// Бинарный поиск без рекурсии.
int bin_search(int* arr, int left_bound, int right_bound, int element) {
    while (left_bound < right_bound) {
        int mid = (left_bound + right_bound) / 2;

        if (arr[mid] < element) {
            left_bound = mid + 1;
        } else {
            right_bound = mid;
        }
    }

    if (arr[left_bound] == element) {
        return left_bound;  // Возвращает позицию вставки элемента на отрезке [first, last).
    } else {
        return -1;          // -1 если элемент не найден.           
    }
}

CArray intersection_sets(int *A, int *B, int A_len, int B_len) {
    CArray result_arr;
    int left_bound = 0;
    int right_bound = 1;

    for (size_t i = 0; i < B_len; ++i) {
        // Ищем границы для бинарного поиска.
        while (A[right_bound] <= B[i] && right_bound < A_len) {
            left_bound = right_bound;
            right_bound = (right_bound * 2 > A_len) ? A_len : right_bound * 2;
        }

        int idx_found_el = bin_search(A, left_bound, right_bound, B[i]);

        if (idx_found_el != -1) {
            result_arr.Add(A[idx_found_el]);
        }
    }
    return result_arr;
}

int main() {
    size_t A_len = 0;
    std::cin >> A_len;

    size_t B_len = 0;
    std::cin >> B_len;

    int *A = new int[A_len];
    for (size_t i = 0; i < A_len; ++i) {
        std::cin >> A[i];
    };

    int *B = new int[B_len];
    for (size_t i = 0; i < B_len; ++i) {
        std::cin >> B[i];
    };

    CArray arr = intersection_sets(A, B, A_len, B_len);
    for (int i = 0; i < arr.Size(); ++i) {
        std::cout << arr[i];

        if (i != arr.Size() - 1) {
            std::cout << ' ';
        } else {
            std::cout << std::endl;
        }
    }
    delete[] A;
    delete[] B;
}