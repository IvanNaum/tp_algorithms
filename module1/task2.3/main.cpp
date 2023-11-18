#include <cassert>
#include <iostream>

/*
2_3. Даны два массива неповторяющихся целых чисел, упорядоченные по
возрастанию. A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1]
в массиве A.. В процессе поиска очередного элемента B[i] в массиве A
пользуйтесь результатом поиска элемента B[i-1]. Внимание! В этой задаче
для каждого B[i] сначала нужно определить диапазон для бинарного поиска
размером порядка k с помощью экспоненциального поиска, а потом уже в
нем делать бинарный поиск.

5
3
1 2 3 4 5
1 3 5

-> 1 3 5
*/

int bin_search(int start, int end, int target, int array[]) {
  assert(start < end);

  while (start <= end) {
    int mid = (start + end) / 2;
    if (array[mid] < target) {
      start = mid + 1;
    } else if (array[mid] > target) {
      end = mid - 1;
    } else {
      return mid;
    }
  }
  return -1;
}

int exp_search(int start, int end, int target, int array[]) {
  assert(start < end);
  if (array[start] == target) {
    return start;
  }
  int index = 1;
  while (start + index < end && array[start + index] < target) {
    index *= 2;
  }

  int ind =
      bin_search(start + index / 2, start + index > end ? end : start + index,
                 target, array);
  // Если в array нет target, возвращает: start + 2 ^ i < target
  return ind != -1 ? ind : start + index / 2;
}

void test_bin_search() {
  {
    int array[] = {1, 2, 3, 4, 5};
    assert(bin_search(0, 5, 1, array) == 0);
    assert(bin_search(0, 5, 2, array) == 1);
    assert(bin_search(0, 5, 3, array) == 2);
    assert(bin_search(0, 5, 4, array) == 3);
    assert(bin_search(0, 5, 5, array) == 4);

    assert(bin_search(0, 5, -1, array) == -1);
    assert(bin_search(0, 5, 6, array) == -1);
  }
  {
    int array[] = {1, 4, 6, 8};
    assert(bin_search(0, 4, 1, array) == 0);
    assert(bin_search(0, 4, 4, array) == 1);
    assert(bin_search(0, 4, 6, array) == 2);
    assert(bin_search(0, 4, 8, array) == 3);

    assert(bin_search(0, 4, 0, array) == -1);
    assert(bin_search(0, 4, 9, array) == -1);
    assert(bin_search(0, 4, 2, array) == -1);
    assert(bin_search(0, 4, 3, array) == -1);
    assert(bin_search(0, 4, 7, array) == -1);
  }
}

void test_exp_search() {
  {
    int array[] = {1, 2, 3, 4, 5};
    assert(exp_search(0, 5, 1, array) == 0);
    assert(exp_search(0, 5, 2, array) == 1);
    assert(exp_search(0, 5, 3, array) == 2);
    assert(exp_search(0, 5, 4, array) == 3);
    assert(exp_search(0, 5, 5, array) == 4);
  }
  {
    int array[] = {1, 3, 5, 7, 9};
    assert(exp_search(0, 5, 2, array) == 0);
    assert(exp_search(0, 5, 4, array) == 1);
    assert(exp_search(0, 5, 6, array) == 2);
    assert(exp_search(0, 5, 8, array) == 2);
    assert(exp_search(0, 5, 10, array) == 4);
  }
}

int main() {
  test_bin_search();
  test_exp_search();

  int N, M;
  std::cin >> N >> M;  // (N >> M))

  int *a_array = new int[N];
  int *b_array = new int[M];

  for (int i = 0; i < N; ++i) std::cin >> a_array[i];
  for (int i = 0; i < M; ++i) std::cin >> b_array[i];

  int last_k = 0;
  for (int i = 0; i < M; ++i) {
    last_k = exp_search(last_k, N, b_array[i], a_array);
    if (a_array[last_k] == b_array[i]) {
      std::cout << b_array[i] << ' ';
    }
  }
  std::cout << std::endl;
  
  delete[] a_array;
  delete[] b_array;

  return 0;
}
