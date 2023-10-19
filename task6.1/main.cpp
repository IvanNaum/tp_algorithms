#include <cassert>
#include <functional>
#include <iostream>

/*
Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition
реализуйте методом прохода двумя итераторами от начала массива к концу.

10
1 2 3 4 5 6 7 8 9 10
-> 2 6 10
*/

template <class T, class Comparator = std::less<T> >
int partition(T *array, int array_size, int pivot_index,
              const Comparator &cmp = std::less<T>()) {
  T pivot = array[pivot_index];
  int l_index = 0;
  int r_index = 0;

  std::swap(array[array_size - 1], array[pivot_index]);
  while (r_index < array_size - 1) {
    if (cmp(pivot, array[r_index])) {
      ++r_index;
    } else {
      std::swap(array[l_index], array[r_index]);
      ++l_index;
      ++r_index;
    }
  }
  std::swap(array[array_size - 1], array[l_index]);
  return l_index;
}

template <class T, class Comparator = std::less<T> >
int get_pivot_by_three(T *array, int a_index, int b_index, int c_index,
                       const Comparator &cmp = std::less<T>()) {
  if (cmp(array[a_index], array[c_index])) {
    if (cmp(array[c_index], array[b_index])) {
      return c_index;  // acb
    }
    return cmp(array[a_index], array[b_index]) ? b_index
                                               : a_index;  // abc : bac
  } else {
    if (cmp(array[a_index], array[b_index])) {
      return a_index;  // cab
    }
    return cmp(array[c_index], array[b_index]) ? b_index
                                               : c_index;  // cba : bca
  }
}

template <class T, class Comparator = std::less<T> >
T order_statistic(T *array, int array_end, int K,
                  const Comparator &cmp = std::less<T>()) {
  assert(K >= 0 && K < array_end);

  // Copy array
  T *partiting_array = new T[array_end];
  for (int i = 0; i < array_end; ++i) {
    partiting_array[i] = array[i];
  }

  int array_start = 0;
  int pivot_index = get_pivot_by_three(
      partiting_array, array_start, (array_start + array_start) / 2, array_end);

  while (1) {
    pivot_index =
        partition(partiting_array + array_start, array_end - array_start,
                  pivot_index - array_start, cmp) +
        array_start;
    if (pivot_index == K) {
      break;
    } else if (pivot_index > K) {
      array_end = pivot_index;
    } else {
      array_start = pivot_index + 1;
    }
    pivot_index =
        get_pivot_by_three(partiting_array, array_start,
                           (array_start + array_start) / 2, array_end - 1);
  }
  T result = partiting_array[pivot_index];
  delete[] partiting_array;
  return result;
}

void test_partition() {
  {
    int array[] = {4, 3, 2, 5, 1};
    partition(array, 5, 1);
    assert(array[0] < 3);
    assert(array[1] < 3);
    assert(array[2] == 3);
    assert(array[3] > 3);
    assert(array[4] > 3);
  }
  {
    int array[] = {4, 3, 2, 5, 1};
    partition(array, 5, 1, [](int a, int b) { return a > b; });
    assert(array[0] > 3);
    assert(array[1] > 3);
    assert(array[2] == 3);
    assert(array[3] < 3);
    assert(array[4] < 3);
  }
  {
    int array[] = {4, 3, 2, 5, 1};
    partition(array, 5, 4);
    assert(array[0] == 1);
    assert(array[1] > 1);
    assert(array[2] > 1);
    assert(array[3] > 1);
    assert(array[4] > 1);
  }
  {
    int array[] = {4, 3, 2, 5, 1};
    partition(array, 5, 4, [](int a, int b) { return a > b; });
    assert(array[0] > 1);
    assert(array[1] > 1);
    assert(array[2] > 1);
    assert(array[3] > 1);
    assert(array[4] == 1);
  }
  std::cout << "The partition tests were successful." << std::endl;
}

void test_pivot() {
  int array[] = {1, 2, 3};
  assert(get_pivot_by_three(array, 0, 1, 2) == 1);
  assert(get_pivot_by_three(array, 0, 2, 1) == 1);
  assert(get_pivot_by_three(array, 1, 0, 2) == 1);
  assert(get_pivot_by_three(array, 1, 2, 0) == 1);
  assert(get_pivot_by_three(array, 2, 1, 0) == 1);
  assert(get_pivot_by_three(array, 2, 0, 1) == 1);

  std::cout << "The pivot tests were successful." << std::endl;
}

void test_order_stat() {
  {
    int array[] = {3, 1, 2};
    assert(order_statistic(array, 3, 0) == 1);
    assert(order_statistic(array, 3, 1) == 2);
    assert(order_statistic(array, 3, 2) == 3);
  }
  {
    int array[] = {3, 1, 2};
    assert(order_statistic(array, 3, 0, [](int a, int b) { return a > b; }) ==
           3);
    assert(order_statistic(array, 3, 1, [](int a, int b) { return a > b; }) ==
           2);
    assert(order_statistic(array, 3, 2, [](int a, int b) { return a > b; }) ==
           1);
  }
  {
    int array[] = {3, 4, 1, 5, 2};
    assert(order_statistic(array, 5, 0) == 1);
    assert(order_statistic(array, 5, 1) == 2);
    assert(order_statistic(array, 5, 2) == 3);
    assert(order_statistic(array, 5, 3) == 4);
    assert(order_statistic(array, 5, 4) == 5);
  }
  {
    int array[] = {3, 4, 1, 5, 2};
    assert(order_statistic(array, 5, 0) == 1);
    assert(order_statistic(array, 5, 1) == 2);
    assert(order_statistic(array, 5, 2) == 3);
    assert(order_statistic(array, 5, 3) == 4);
    assert(order_statistic(array, 5, 4) == 5);
  }
  std::cout << "The order statistic tests were successful." << std::endl;
}

int main() {
  // test_partition();
  // test_pivot();
  // test_order_stat();

  int N;
  std::cin >> N;

  int *array = new int[N];

  for (int i = 0; i < N; ++i) {
    std::cin >> array[i];
  }

  std::cout << order_statistic(array, N, N / 10) << std::endl;
  std::cout << order_statistic(array, N, N % 2 == 0 ? N / 2 : N / 2 + 1)
            << std::endl;
  std::cout << order_statistic(array, N, N * 90 / 100) << std::endl;

  delete[] array;

  return 0;
}
