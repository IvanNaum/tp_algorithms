#include <cassert>
#include <iostream>

/*
Дан массив, который получен путем нескольких циклических сдвигов исходного
отсортированного массива. Нужно найти индекс элемента, с которого начинался
исходный массив.

[5, 5, 5, 1] -> 3
[12, 14, 16, 17, 1, 3, 5, 7] -> 4
*/

int bin_search(int *array, int array_size) {
  if (array[0] < array[array_size - 1]) {
    return 0;
  }

  int start = 0;
  int end = array_size - 1;

  while (start < end - 1) {
    int mid = (start + end) / 2;

    if (array[mid] < array[0]) {
      end = mid;
    } else {
      start = mid;
    }
  }

  return end;
}

void test_bs() {
  {
    int arr[] = {4, 5, 1, 2, 3};
    assert(bin_search(arr, 5) == 2);
  }
  {
    int arr[] = {1, 2, 3, 4, 5};
    assert(bin_search(arr, 5) == 0);
  }
  {
    int arr[] = {5, 5, 5, 1};
    assert(bin_search(arr, 4) == 3);
  }

  {
    int arr[] = {12, 14, 16, 17, 1, 3, 5, 7};
    assert(bin_search(arr, 8) == 4);
  }
}

int main() {
  // test_bs();

  int N = 0;
  std::cin >> N;

  int *array = new int[N];

  for (int i = 0; i < N; ++i) {
    std::cin >> array[i];
  }

  std::cout << bin_search(array, N) << std::endl;

  return 0;
}
