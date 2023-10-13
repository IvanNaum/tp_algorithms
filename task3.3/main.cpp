#include <cassert>
#include <iostream>

/*
3_3. Реализовать очередь с помощью двух стеков.
Требования: Очередь должна быть реализована в виде класса. Стек тоже должен быть
реализован в виде класса.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back

3
3 44
3 50
2 44
-> YES

2
3 44
2 66
-> NO
 */

#define DEFAULT_STACK_SIZE 4

struct Stack {
  Stack()
      : _data(new int[DEFAULT_STACK_SIZE]),
        _capacity(DEFAULT_STACK_SIZE),
        _head(0) {}
  Stack(Stack& st) = delete;
  Stack& operator=(const Stack& st) = delete;

  ~Stack() { delete[] _data; }

  void push(int value) {
    if (++_head == _capacity) {
      resize();
    }
    _data[_head] = value;
  }

  int pop() {
    assert(_head > 0);
    return _data[_head--];
  }

  int size() const { return _head; }
  int capacity() const { return _capacity; }
  bool is_empty() const { return _head == 0; }

 private:
  void resize() {
    _capacity *= 2;
    int* new_data = new int[_capacity];

    for (int i = 0; i < _head; ++i) {
      new_data[i] = _data[i];
    }
    delete[] _data;
    _data = new_data;
  }

  int* _data;
  int _capacity;
  int _head;
};

struct Queue {
  Queue() : in_stack(), out_stack() {}
  ~Queue() = default;

  Queue(const Queue& qu) = delete;
  Queue& operator=(const Queue& qu) = delete;

  bool is_empty() const { return out_stack.is_empty() && in_stack.is_empty(); }
  int size() const { return in_stack.size(); }

  void push(int value) { in_stack.push(value); }

  int pop() {
    if (is_empty()) {
      return -1;
    }
    if (out_stack.is_empty()) {
      while (!in_stack.is_empty()) out_stack.push(in_stack.pop());
    }
    return out_stack.pop();
  }

 private:
  Stack in_stack;
  Stack out_stack;
};

void test_stack() {
  Stack stack;

  assert(stack.is_empty());
  assert(stack.size() == 0);
  assert(stack.capacity() == DEFAULT_STACK_SIZE);
  stack.push(1);
  stack.push(2);
  stack.push(3);
  assert(!stack.is_empty());
  assert(stack.size() == 3);
  assert(stack.pop() == 3);
  assert(stack.pop() == 2);
  assert(stack.pop() == 1);

  assert(stack.is_empty());

  for (int i = 0; i < 100; i++) stack.push(i + 1);
  assert(stack.size() == 100);
  for (int i = 100; i > 0; i--) assert(stack.pop() == i);
  assert(stack.is_empty());
}

void test_queue() {
  Queue queue;

  assert(queue.is_empty());

  // Test 1
  queue.push(1);
  queue.push(2);
  queue.push(3);
  assert(queue.size() == 3);
  assert(queue.pop() == 1);
  assert(queue.pop() == 2);
  assert(queue.pop() == 3);
  assert(queue.is_empty());

  // Test 2
  for (int i = 1; i <= 10; i++) queue.push(i);
  assert(queue.size() == 10);
  for (int i = 1; i <= 10; i++) assert(queue.pop() == i);
  assert(queue.is_empty());

  // Test 3
  assert(queue.pop() == -1);

  // Test 4
  queue.push(44);
  queue.push(50);
  assert(queue.pop() == 44);
  assert(queue.pop() == 50);
  assert(queue.pop() == -1);
}

int main() {
  test_stack();
  test_queue();

  int n = 0;
  std::cin >> n;

  Queue queue;

  int command = 0, value = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> command >> value;

    if (command == 2) {
      if (queue.pop() != value) {
        std::cout << "NO" << std::endl;
        return 0;
      }
    } else if (command == 3) {
      queue.push(value);
    }
  }

  std::cout << "YES" << std::endl;
  return 0;
}
