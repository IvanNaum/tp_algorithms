#include <cassert>
#include <functional>
#include <iostream>
#include <utility>

/*
5_3. Закраска прямой 1.
На числовой прямой окрасили N отрезков. Известны координаты левого и правого
концов каждого отрезка (Li и Ri). Найти длину окрашенной части числовой прямой.

3
1 4
7 8
2 5
-> 5
*/

typedef std::pair<int, int> pair;

template <class T, class Compare = std::less<T> >
void merge_sort(T* array, size_t size, const Compare& cmp = std::less<T>()) {
  if (size <= 1) {
    return;
  }
  size_t l_size = size / 2;
  size_t r_size = size - l_size;

  merge_sort(array, l_size, cmp);
  merge_sort(array + l_size, r_size, cmp);

  size_t l_it = 0;
  size_t r_it = 0;

  T* temp = new T[size];

  while (l_it < l_size && r_it < r_size) {
    if (cmp(array[l_it], array[l_size + r_it])) {
      temp[l_it + r_it] = array[l_it];
      ++l_it;
    } else {
      temp[l_it + r_it] = array[l_size + r_it];
      ++r_it;
    }
  }

  while (r_it < r_size) {
    temp[l_it + r_it] = array[l_size + r_it];
    ++r_it;
  }

  while (l_it < l_size) {
    temp[l_it + r_it] = array[l_it];
    ++l_it;
  }

  for (size_t i = 0; i < size; ++i) {
    array[i] = temp[i];
  }

  delete[] temp;
  return;
}

int get_length_painted_line(pair* array, int arr_size) {
  // not painted
  int prev = -1, cur = 0, count = 0;
  for (int i = 0; i < arr_size; ++i) {
    pair pr = array[i];
    if (prev != -1 && cur > 0) {
      count += pr.first - prev;
    }
    prev = pr.first;
    cur += pr.second;
  }
  return count;
}

void test_sort() {
  {
    int arr[] = {1};
    merge_sort(arr, 1);
    assert(arr[0] == 1);
  }
  {
    int arr[] = {2, 1};
    merge_sort(arr, 2);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
  }
  {
    int arr[] = {2, 3, 1};
    merge_sort(arr, 3);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr[2] == 3);
  }
  {
    int arr[] = {0, 5, 7, 3, 9, 1, 6, 2, 4, 8};
    merge_sort(arr, 10);
    for (int i = 0; i < 10; ++i) {
      assert(arr[i] == i);
    }
  }
  {
    int arr[] = {2, 1, 3, 1, 2, 1};
    merge_sort(arr, 6);
    assert(arr[0] == 1);
    assert(arr[1] == 1);
    assert(arr[2] == 1);
    assert(arr[3] == 2);
    assert(arr[4] == 2);
    assert(arr[5] == 3);
  }
  {
    int arr[] = {1, 2};
    merge_sort(arr, 2, [](int a, int b) { return a > b; });
    assert(arr[0] == 2);
    assert(arr[1] == 1);
  }
  {
    int arr[] = {3, 1, 2};
    merge_sort(arr, 3, [](int a, int b) { return a > b; });
    assert(arr[0] == 3);
    assert(arr[1] == 2);
    assert(arr[2] == 1);
  }
  {
    int arr[] = {0, 5, 7, 3, 9, 1, 6, 2, 4, 8};
    merge_sort(arr, 10, [](int a, int b) { return a > b; });
    for (int i = 0; i < 9; ++i) {
      assert(arr[i] == 9 - i);
    }
  }
}

int main() {
  // test_sort();

  int N;
  std::cin >> N;

  std::pair<int, int>* array = new pair[N * 2];

  int start_line = 0;
  int end_line = 0;
  for (int i = 0; i < N * 2; i += 2) {
    std::cin >> start_line >> end_line;
    array[i] = {start_line, +1};
    array[i + 1] = {end_line, -1};
  }

  merge_sort(array, N * 2,
             [](const pair& a, const pair& b) { return a.first < b.first; });

  std::cout << get_length_painted_line(array, N * 2) << std::endl;

  // std::cout << std::endl << "Success." << std::endl;
  return 0;
}
