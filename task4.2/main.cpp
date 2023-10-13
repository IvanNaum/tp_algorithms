#include <cassert>
#include <functional>
#include <iostream>

/*
Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор
пользователя, посещаемость сайта). Напишите программу, которая выбирает K
пользователей, которые чаще других заходили на сайт, и выводит их в порядке
возрастания посещаемости. Количество заходов и идентификаторы пользователей не
повторяются.

O(N * logK)

3 3
100 36
80 3
1 5
-> 80 1 100
*/

#define DEFAULT_ARRAY_SIZE_HEAP 7  // Для дерева высоты 2: 1 + 2 + 4

template <class T, class Comparator = std::less<T> >
struct Heap {
  Heap()
      : max_size(0),
        _data(new T[DEFAULT_ARRAY_SIZE_HEAP]),
        _size(0),
        _capacity(DEFAULT_ARRAY_SIZE_HEAP){};
  Heap(size_t max_size)
      : max_size(max_size),
        _data(new T[DEFAULT_ARRAY_SIZE_HEAP]),
        _size(0),
        _capacity(DEFAULT_ARRAY_SIZE_HEAP){};
  Heap(const Heap& heap) = delete;
  Heap& operator=(const Heap& heap) = delete;
  ~Heap() {
    _size = 0;
    _capacity = 0;
    delete[] _data;
  }

  // Добавить элемент в кучу за
  void insert(T element) {
    if (max_size != 0 && _size == max_size) {
      if (cmp(_data[_size], element)) {
        _data[_size] = element;
      }
      sift_up(_size);
    } else {
      if (_size + 1 == _capacity) {
        replace();
      }
      _data[_size] = element;
      ++_size;
      sift_up(_size - 1);
    }
  }

  // Извлечь максимум из кучи за
  T extract_max() {
    T res = _data[0];
    _data[0] = _data[_size - 1];
    --_size;
    sift_down(0);
    return res;
  }

  // Посмотреть значение максимума в куче за
  T peek_max() const { return _data[0]; };

  size_t size() const { return _size; }

  bool is_full() const { return max_size != 0 && _size == max_size; }

  bool is_empty() const { return _size == 0; }

  void print() {
    for (int i = 0; i < _size; ++i) {
      std::cout << _data[i] << ' ';
    }
    std::cout << std::endl;
  }

 private:
  void replace() {
    _capacity *= 2;
    if (max_size != 0 && _capacity > max_size) {
      _capacity = max_size + 1;
    }
    T* new_data = new T[_capacity];
    for (size_t i = 0; i < _size; ++i) {
      new_data[i] = _data[i];
    }

    delete[] _data;
    _data = new_data;
  }

  // void buildHeap();
  void sift_down(size_t index) {
    size_t r_child = index * 2 + 1;
    size_t l_child = index * 2 + 2;

    size_t largest = index;

    if (r_child < _size && cmp(_data[index], _data[r_child])) {
      largest = r_child;
    }
    if (l_child < _size && cmp(_data[largest], _data[l_child])) {
      largest = l_child;
    }

    if (largest != index) {
      // swap
      T temp = _data[index];
      _data[index] = _data[largest];
      _data[largest] = temp;

      // O(log n) -> recursive is possible
      sift_down(largest);
    }
  }

  void sift_up(size_t index) {
    if (index == 0) {
      return;
    }

    size_t parent = (index - 1) / 2;

    if (cmp(_data[parent], _data[index])) {
      T temp = _data[index];
      _data[index] = _data[parent];
      _data[parent] = temp;

      // O(log n) -> recursive is possible
      sift_up(parent);
    }
  }

  T* _data;
  size_t _capacity;
  size_t _size;
  size_t max_size;
  Comparator cmp;
};

struct UserLog {
  UserLog() : _id(-1), _count(-1){};
  UserLog(int id, int count) : _id(id), _count(count){};
  UserLog(const UserLog& us_log) {
    _id = us_log._id;
    _count = us_log._count;
  }
  ~UserLog(){};

  int id() const { return _id; }
  int count() const { return _count; }

  struct more {
    bool operator()(const UserLog& user_1, const UserLog& user_2) {
      if (user_1.count() > user_2.count()) {
        return true;
      }
      if (user_1.count() == user_2.count() && user_1.id() > user_2.id()) {
        return true;
      }
      return false;
    };
  };

 private:
  int _id;
  int _count;
};

std::ostream& operator<<(std::ostream& os, const UserLog& usr_log) {
  return os << usr_log.id();
}

void test_heap() {
  {  // Base test
    Heap<int> hp;
    hp.insert(1);
    hp.insert(2);
    hp.insert(3);
    hp.insert(4);
    hp.insert(5);

    assert(hp.size() == 5);

    assert(hp.extract_max() == 5);
    assert(hp.extract_max() == 4);
    assert(hp.extract_max() == 3);
    assert(hp.extract_max() == 2);
    assert(hp.extract_max() == 1);
    assert(hp.is_empty());
  }

  {  // Test replace
    Heap<int> hp;
    for (int i = 1; i <= 100; ++i) {
      hp.insert(i);
    }
    for (int i = 100; i >= 1; --i) {
      assert(hp.extract_max() == i);
    }
  }

  {  // Test heap with fixed size
    Heap<int> hp(3);
    hp.insert(1);
    hp.insert(2);
    hp.insert(3);
    hp.insert(4);
    hp.insert(5);

    assert(hp.size() == 3);
  }

  {  // Comparator test
    struct TestComparator {
      bool operator()(int a, int b) { return -a < -b; }
    };
    Heap<int, TestComparator> hp;
    hp.insert(1);
    hp.insert(2);
    hp.insert(3);
    hp.insert(4);
    hp.insert(5);

    assert(hp.size() == 5);

    assert(hp.extract_max() == 1);
    assert(hp.extract_max() == 2);
    assert(hp.extract_max() == 3);
    assert(hp.extract_max() == 4);
    assert(hp.extract_max() == 5);
    assert(hp.is_empty());
  }
}

void test_userlog_heap() {
  {
    Heap<UserLog, UserLog::more> heap(3);
    heap.insert(UserLog(1, 1));
    heap.insert(UserLog(2, 2));
    heap.insert(UserLog(3, 3));

    assert(heap.extract_max().id() == 1);
    assert(heap.extract_max().id() == 2);
    assert(heap.extract_max().id() == 3);
  }
  {
    Heap<UserLog, UserLog::more> heap(3);

    UserLog test_users[] = {
        UserLog(1, 1), UserLog(2, 2), UserLog(3, 3),
        UserLog(4, 4), UserLog(5, 5),
    };
    for (int i = 0; i < 5; ++i) {
      UserLog us_log = test_users[i];
      if (heap.is_full()) {
        if (UserLog::more()(us_log, heap.peek_max())) {
          heap.extract_max();
          heap.insert(us_log);
        }
      } else {
        heap.insert(us_log);
      }
    }

    assert(heap.extract_max().id() == 3);
    assert(heap.extract_max().id() == 4);
    assert(heap.extract_max().id() == 5);
  }
}

int main() {
  // test_heap();
  // test_userlog_heap();

  int N, K;
  std::cin >> N >> K;

  Heap<UserLog, UserLog::more> heap(K);
  int id, count;
  for (int i = 0; i < N; ++i) {
    std::cin >> id >> count;
    UserLog us_log = UserLog(id, count);
    if (heap.is_full()) {
      if (UserLog::more()(us_log, heap.peek_max())) {
        heap.extract_max();
        heap.insert(us_log);
      }
    } else {
      heap.insert(us_log);
    }
  }
  for (int i = 0; i < K; ++i) {
    std::cout << heap.extract_max() << " ";
  }
  return 0;
}
