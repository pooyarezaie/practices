#include <iostream>

const int MAX_N = 1000 * 1000 + 10;

bool appeared_divisor[MAX_N];

int main() {
  int n;
  std::cin >> n;
  int answer = 0;
  for (int i = 0; i < n; ++i) {
    int friendship;
    std::cin >> friendship;
    for (int j = 1; j * j <= friendship; ++j) {
      if (friendship % j != 0) {
        continue;
      }
      if (appeared_divisor[j]) {
        answer = std::max(answer, j);
      }
      appeared_divisor[j] = true;
      if (appeared_divisor[friendship / j] && friendship / j != j) {
        answer = std::max(answer, friendship / j);
      }
      appeared_divisor[friendship / j] = true;
    }
  }
  std::cout << answer << std::endl;
  return 0;
}