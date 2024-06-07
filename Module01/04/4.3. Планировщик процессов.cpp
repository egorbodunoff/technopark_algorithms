// В операционной системе Technux есть планировщик процессов.
// Каждый процесс характеризуется:
// приоритетом P
// временем, которое он уже отработал t
// временем, которое необходимо для завершения работы процесса T
// Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
// выполняет его время P и кладет обратно в очередь процессов.
// Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
// Требуется посчитать кол-во переключений процессора.

#include <cassert>
#include <iostream>
#include <sstream>

struct Process {
    int P;
    int t;
    int T;
    Process() : P(0), t(0), T(0) {}
    Process(int P, int t, int T) : P(P), t(t), T(T) {}
};

class IsLessByP {
   public:
    IsLessByP(){};
    bool operator()(const Process& p1, const Process& p2) { return p1.P * (p1.t + 1) < p2.P * (p2.t + 1); }
};

template <class T, class IsLess>
class Heap {
   public:
    Heap(IsLess isLess) : buffer(0), bufferSize(0), size(0) {}
    Heap(T* array, int arrSize, IsLess isLess);
    Heap(const Heap& h);
    Heap& operator=(const Heap& h);
    ~Heap() { delete[] buffer; }

    void Insert(const T& element);
    void DeleteLast() { --size; }
    T ExtractMax();
    int Size() const { return size; }
    const T& PeekMax() const { return buffer[0]; }
    bool IsEmpty() const { return size == 0; }

   private:
    IsLess isLess;
    T* buffer;
    int bufferSize;
    int size;

    void buildHeap();
    void grow();
    void siftDown(int i);
    void siftUp(int i);
};

template <class T, class IsLess>
Heap<T, IsLess>::Heap(T* array, int arrSize, IsLess isLess) {
    bufferSize = arrSize;
    T* newBuffer = new T[bufferSize];

    for (int i = 0; i < arrSize; ++i) {
        newBuffer[size++] = array[i];
    }
    delete[] buffer;
    buffer = newBuffer;
    buildHeap();
}

template <class T, class IsLess>
Heap<T, IsLess>::Heap(const Heap& h) {
    bufferSize = h.bufferSize;
    size = h.size;
    isLess = h.isLess;

    buffer = new T[bufferSize];
    for (int i = 0; i < bufferSize; ++i) {
        buffer[i] = h.buffer[i];
    }
}

template <class T, class IsLess>
Heap<T, IsLess>& Heap<T, IsLess>::operator=(const Heap& h){
    if (&h != this) {
        bufferSize = h.bufferSize;
        size = h.size;
        isLess = h.isLess;

        delete[] buffer;
        buffer = new T[bufferSize];
        for (int i = 0; i < size; ++i) {
            buffer[i] = h.buffer[i];
        }
    }
    return *this;
}

template <class T, class IsLess>
void Heap<T, IsLess>::siftDown(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = -1;

    while (largest != i) {
        largest = i;

        if (left < size && isLess(buffer[i], buffer[left])) largest = left;
        if (right < size && isLess(buffer[largest], buffer[right])) largest = right;

        if (largest != i) {
            std::swap(buffer[i], buffer[largest]);
        }
    }
}

template <class T, class IsLess>
void Heap<T, IsLess>::grow() {
    int newBufferSize = std::max(bufferSize * 2, 1);
    T* newBuffer = new T[newBufferSize];

    for (int i = 0; i < size; ++i) {
        newBuffer[i] = buffer[i];
    }
    delete[] buffer;
    buffer = newBuffer;
    bufferSize = newBufferSize;
}

template <class T, class IsLess>
void Heap<T, IsLess>::buildHeap() {
    for (int i = size / 2 - 1; i >= 0; --i) {
        siftDown(i);
    }
}

template <class T, class IsLess>
void Heap<T, IsLess>::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (isLess(buffer[index], buffer[parent])) return;

        std::swap(buffer[index], buffer[parent]);
        index = parent;
    }
}

template <class T, class IsLess>
void Heap<T, IsLess>::Insert(const T& element) {
    if (size == bufferSize) grow();

    buffer[size++] = element;
    siftUp(size - 1);
}

template <class T, class IsLess>
T Heap<T, IsLess>::ExtractMax() {
    assert(size != 0);

    T result = buffer[0];

    std::swap(buffer[0], buffer[size - 1]);
    DeleteLast();

    if (size != 0) {
        siftDown(0);
    }
    return result;
}

int run(std::istream& input, std::ostream& output) {
    IsLessByP isLess;
    Heap<Process, IsLessByP> prHeap(isLess);
    int n = 0;
    input >> n;
    int result = 0;

    for (int i = 0; i < n; ++i) {
        Process pr;
        input >> pr.P >> pr.T;
        pr.t = 0;
        prHeap.Insert(pr);
    }

    while (!prHeap.IsEmpty()) {
        Process run_procces = prHeap.ExtractMax();
        if (run_procces.t < run_procces.T) {
            result++;
            run_procces.t += run_procces.P;

            prHeap.Insert(run_procces);
        }
    } 
    return result; 
}

int main() {
    int res = run(std::cin, std::cout);
    std::cout << res;
    return 0;
}