#include <iostream>

int gcd(int a, int b);

int main() {
  int n;
  std::cin >> n;
  int answer = 0;
  for (int i = 1; i <= n; ++i) {
    if (gcd(n, i) == 1) {
      answer++;
    }
  }
  std::cout << answer << std::endl;
  return 0;
}

int gcd(int a, int b) { return (b == 0 ? a : gcd(b, a % b)); }
