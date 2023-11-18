#include <cassert>
#include <iostream>

/*
ВНИМАНИЕ! Для решения данной задачи нужно использовать структуру данных стек!

Дана последовательность, состоящая из символов '(' и ')'. Требуется определить,
является ли данная последовательность правильной скобочной последовательностью.
Длина исходной последовательности ≤ 800000.

() -> YES
)( -> NO

*/

#define DEFAULT_STACK_SIZE 7

struct Stack {
  Stack()
      : _data(new char[DEFAULT_STACK_SIZE]),
        _capacity(DEFAULT_STACK_SIZE),
        _head(0) {}
  Stack(Stack& st) = delete;
  Stack& operator=(const Stack& st) = delete;

  ~Stack() { delete[] _data; }

  void push(char value) {
    if (++_head == _capacity) {
      resize();
    }
    _data[_head] = value;
  }

  char pop() {
    assert(_head > 0);
    return _data[_head--];
  }

  bool empty() const { return _head == 0; }

 private:
  void resize() {
    _capacity *= 2;
    char* new_data = new char[_capacity];

    for (int i = 0; i < _head; ++i) {
      new_data[i] = _data[i];
    }
    delete[] _data;
    _data = new_data;
  }

  char* _data;
  int _capacity;
  int _head;
};

int main() {
  Stack st;

  char sym = ' ';
  while (std::cin.peek() != '\n' && std::cin >> sym) {
    if (sym == ')') {
      if (st.empty()) {
        std::cout << "NO" << std::endl;
        return 0;
      }
      st.pop();
    } else {
      st.push(sym);
    }
  }

  std::cout << (st.empty() ? "YES" : "NO") << std::endl;
  return 0;
}
