#include <cassert>
#include <iostream>

/*
Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше
10^6. Отсортировать массив методом MSD по битам (бинарный QuickSort).

3
4 1000000 7
-> 4 7 1000000
*/

int partition(uint64_t *array, int array_size, int power) {
  uint64_t pivot = 1 << power;
  int l_index = 0;
  int r_index = 0;

  while (r_index < array_size) {
    if (array[r_index] & pivot) {
      ++r_index;
    } else {
      std::swap(array[l_index], array[r_index]);
      ++l_index;
      ++r_index;
    }
  }
  return l_index;
}

void MSD(uint64_t *array, int array_size, int power = 63) {
  if (power < 0 || array_size <= 0) {
    return;
  }

  int center_index = partition(array, array_size, power);

  MSD(array, center_index, power - 1);
  MSD(array + center_index, array_size - center_index, power - 1);
}

void test_partition() {
  {
    uint64_t array[] = {0b01, 0b00};
    partition(array, 2, 0);
    assert(array[0] == 0b00);
    assert(array[1] == 0b01);
  }
  {
    uint64_t array[] = {0b01, 0b11, 0b00};
    partition(array, 3, 1);
    partition(array, 2, 0);
    assert(array[0] == 0b00);
    assert(array[1] == 0b01);
    assert(array[2] == 0b11);
  }
  {
    uint64_t array[] = {0b01, 0b11, 0b10, 0b00};
    partition(array, 4, 1);
    partition(array, 2, 0);
    partition(array + 2, 2, 0);
    assert(array[0] == 0b00);
    assert(array[1] == 0b01);
    assert(array[2] == 0b10);
    assert(array[3] == 0b11);
  }

  std::cout << "The partition tests were successful." << std::endl;
}

void test_MSD() {
  {
    uint64_t array[] = {3, 2, 4, 5, 1};
    MSD(array, 5);
    assert(array[0] == 1);
    assert(array[1] == 2);
    assert(array[2] == 3);
    assert(array[3] == 4);
    assert(array[4] == 5);
  }
  {
    uint64_t array[] = {1 << 25, 1 << 13, 1 << 3, 1 << 6, 1 << 12};
    MSD(array, 5);
    assert(array[0] == 1 << 3);
    assert(array[1] == 1 << 6);
    assert(array[2] == 1 << 12);
    assert(array[3] == 1 << 13);
    assert(array[4] == 1 << 25);
  }
  {
    uint64_t array[] = {4, 1000000, 7};
    MSD(array, 3);
    assert(array[0] == 4);
    assert(array[1] == 7);
    assert(array[2] == 1000000);
  }
  {
    uint64_t array[] = {2, 1, 2, 0, 3, 1, 1};
    MSD(array, 7);
    assert(array[0] == 0);
    assert(array[1] == 1);
    assert(array[2] == 1);
    assert(array[3] == 1);
    assert(array[4] == 2);
    assert(array[5] == 2);
    assert(array[6] == 3);
  }
  {
    uint64_t array[] = {0b01, 0b11, 0b00};
    MSD(array, 3);
    assert(array[0] == 0);
    assert(array[1] == 1);
    assert(array[2] == 3);
  }

  std::cout << "The MSD tests were successful." << std::endl;
}

int main() {
  // Tests
  assert(sizeof(uint64_t) * 8 == 64);
  test_partition();
  test_MSD();

  // int N;
  // std::cin >> N;

  // uint64_t *array = new uint64_t[N];
  // for (int i = 0; i < N; ++i) {
  //   std::cin >> array[i];
  // }

  // // Sorting
  // MSD(array, N);

  // for (int i = 0; i < N; ++i) {
  //   std::cout << array[i] << ' ';
  // }

  // delete[] array;

  return 0;
}
