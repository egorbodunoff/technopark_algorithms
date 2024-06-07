// Реализовать очередь с динамическим зацикленным буфером.
// Обрабатывать команды push back и pop front.

#include <cassert>
#include <iostream>
#include <sstream>

// Очередь целых чисел, реализованная с помощью динамического массива.
class Queue {
   private:
    int* buffer;     // Буфер.
    int bufferSize;  // Размер буфера.
    int head;        // Указывает на первый элемент очереди.
    int tail;        // Указывает на следующий после последнего.
    int size;

    void grow();

   public:
    Queue() : buffer(0), bufferSize(0), head(0), tail(0){};
    ~Queue() { delete[] buffer; }
    Queue(const Queue& q);
    Queue& operator=(const Queue& q);

    // Добавление и извлечение элемента из очереди.
    void Enqueue(int a);
    int Dequeue();

    // Проверка на пустоту.
    bool IsEmpty() const { return head == tail; }
};

Queue::Queue(const Queue& q) {
    bufferSize = q.bufferSize;
    head = q.head;
    tail = q.tail;
    size = q.size;

    buffer = new int[bufferSize];
    for (int i = head; i < tail; ++i) {
        buffer[i] = q.buffer[i];
    }
}

Queue& Queue::operator=(const Queue& q) {
    if (&q != this) {
        bufferSize = q.bufferSize;
        head = q.head;
        tail = q.tail;
        size = q.size;

        delete[] buffer;
        buffer = new int[bufferSize];
        for (int i = head; i < tail; ++i) {
            buffer[i] = q.buffer[i];
        }
    }
    return *this;
}
// Увеличение буфера.
void Queue::grow() {
    int newBufferSize = std::max(bufferSize * 2, 2);
    int* newBuffer = new int[newBufferSize];

    int idx = 0;
    while (head != tail) {
        newBuffer[idx++] = buffer[head];
        head = (head + 1) % bufferSize;
    };

    delete[] buffer;
    buffer = newBuffer;
    bufferSize = newBufferSize;

    head = 0;
    tail = idx;
}

void Queue::Enqueue(int a) {
    if ((bufferSize == 0) || ((tail + 1) % bufferSize == head)) {
        grow();
    }
    buffer[tail] = a;
    tail = (tail + 1) % bufferSize;
    ++size;
}

int Queue::Dequeue() {
    assert(size > 0);

    --size;
    int result = buffer[head];
    head = (head + 1) % bufferSize;

    return result;
}

void run(std::istream& input, std::ostream& output) {
    Queue intQueue;
    int n = 0;
    input >> n;
    bool result = true;

    for (int i = 0; i < n; ++i) {
        int command = 0;
        int data = 0;
        input >> command >> data;

        switch (command) {
            case 2:
                if (intQueue.IsEmpty()) {
                    result = result && data == -1;
                } else {
                    result = result && intQueue.Dequeue() == data;
                }
                break;
            case 3:
                intQueue.Enqueue(data);
                break;
            default:
                assert(false);
        }
    }
    output << (result ? "YES" : "NO") << std::endl;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}
