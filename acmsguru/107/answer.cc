/*
for i in range(10**8, 10**9):
    if ((i**2) % (10**9) == 987654321):
        print(i)

111111111
119357639
380642361
388888889
611111111
619357639
880642361
888888889
*/

#include <iostream>

const int ANSWER_9 = 8;

int main() {
  int n;
  std::cin >> n;

  if (n <= 8) {
    std::cout << 0 << std::endl;
    return 0;
  }
  if (n == 9) {
    std::cout << 8 << std::endl;
  }
  if (n > 9) {
    std::cout << 72;
    for (int i = 0; i < n - 10; ++i) {
      std::cout << 0;
    }
    std::cout << std::endl;
  }
  return 0;
}