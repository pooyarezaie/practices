#include <algorithm>
#include <iostream>

const int MAX_N = 10 * 1000 + 10;

int color[MAX_N];

int count(int n, int d) {
  int answer = 0;
  while (n % d == 0) {
    answer += 1;
    n /= d;
  }
  return answer;
}

int main() {
  int n;
  std::cin >> n;

  for (int i = 2; i <= n; ++i) {
    if (color[i] > 0) {
      continue;
    }
    for (int k = 1; k * i <= n; ++k) {
      color[k * i] += count(k * i, i);
    }
  }
  std::cout << *std::max_element(color, color + MAX_N) + 1 << std::endl;
  for (int i = 1; i <= n; ++i) {
    std::cout << color[i] + 1 << " ";
  }
  std::cout << std::endl;
  return 0;
}