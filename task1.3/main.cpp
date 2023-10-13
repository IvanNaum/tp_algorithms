#include <iostream>

/*
1.3 Если в числе содержится только один бит со значением 1, записать
в выходной поток OK. Иначе записать FAIL

32 -> OK
34 -> FAIL
 */

bool is_power_of_2(unsigned int x) { return x && !(x & (x - 1)); }

int main() {
  // Tests
  // assert(!is_power_of_2(0));
  // assert(is_power_of_2(1));
  // assert(is_power_of_2(2));
  // assert(!is_power_of_2(3));
  // for (int i = 4; i <= 1024; i *= 2)
  // {
  //     assert(is_power_of_2(i));
  // }

  unsigned int x;
  std::cin >> x;
  std::cout << (is_power_of_2(x) ? "OK" : "FAIL") << std::endl;
  return 0;
}
