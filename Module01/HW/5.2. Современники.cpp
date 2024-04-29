// Группа людей называется современниками если был такой момент, когда они могли собраться вместе.
// Для этого в этот момент каждому из них должно было  уже исполниться 18 лет,
// но ещё не исполниться 80 лет. Дан список Жизни Великих Людей. Необходимо получить
// максимальное количество современников. В день 18летия человек уже может принимать
// участие в собраниях, а в день 80летия и в день смерти уже не может.

#include <cassert>
#include <iostream>
#include <sstream>

struct Date {
    int day;
    int month;
    int year;
    int board;
    Date() : day(0), month(0), year(0), board(0) {}
    Date(int day, int month, int year, int board) : day(day), month(month), year(year), board(board) {}

    bool operator<(const Date& r) const {
        if (year != r.year)
            return year < r.year;
        else if (month != r.month)
            return month < r.month;
        else if (day != r.day)
            return day < r.day;
        else if (board != r.board)
            return board > r.board;
        else
            return false;
    }
};

template <class T>
class IsLessDefault {
   public:
    bool operator()(const T& l, const T& r) { return l < r; }
};

template <class T, class IsLess = IsLessDefault<T>>
void mmerge(T* arr, size_t first, size_t middle, size_t last, IsLess isLess) {
    T* result = new T[last - first];
    size_t j = 0, i = 0;

    while (first + i < middle && middle + j < last) {
        if (isLess(arr[first + i], arr[middle + j])) {
            result[i + j] = arr[first + i];
            ++i;
        } else {
            result[i + j] = arr[middle + j];
            ++j;
        }
    }
    while (first + i < middle) {
        result[i + j] = arr[first + i];
        ++i;
    }
    while (middle + j < last) {
        result[i + j] = arr[middle + j];
        ++j;
    }
    for (size_t k = 0; k < i + j; ++k) {
        arr[first + k] = result[k];
    }
    delete[] result;
}

template <class T, class IsLess = IsLessDefault<T>>
void MergeSort(T* arr, size_t arr_size, IsLess isLess = IsLess()) {
    if (arr_size > 0) {
        for (size_t i = 1; i < arr_size; i *= 2) {
            for (size_t j = 0; j < arr_size - i; j += 2 * i) {
                mmerge<T, IsLess>(arr, j, i + j, std::min(2 * i + j, arr_size), isLess);
            }
        }
    }
}

int operate(Date* arr, size_t arrSize) {
    int current = 0, result = 0;

    for (size_t i = 0; i < arrSize; ++i) {
        if (arr[i].board == 0) {
            ++current;
        } else {
            --current;
        }
        if (current > result) {
            result = current;
        }
    }
    return result;
}

int run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;
    size_t size = 0;
    Date* arr = new Date[2 * n];

    for (size_t i = 0; i < 2 * n; i += 2) {
        Date date_l, date_r;

        input >> date_l.day >> date_l.month >> date_l.year >> date_r.day >> date_r.month >> date_r.year;
        date_l.board = 0, date_r.board = 1;

        date_l.year += 80;
        if (date_l < date_r) {
            date_r.day = date_l.day;
            date_r.month = date_l.month;
            date_r.year = date_l.year;
        }
        date_l.year -= 80;

        date_l.year += 18;
        if (date_l < date_r) {
            arr[size++] = date_l;
            arr[size++] = date_r;
        }
    }
    MergeSort<Date>(arr, size);
    int res = operate(arr, size);
    delete[] arr;
    return res;
}

int main() {
    int res = run(std::cin, std::cout);
    std::cout << res;
}