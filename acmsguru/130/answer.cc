#include <iostream>

const int MAX_K = 30 + 5;
long long int f[MAX_K];

int main() {
  int k;
  std::cin >> k;

  f[0] = f[1] = 1;
  for (int i = 2; i <= k; ++i) {
    for (int j = 0; j <= (i - 1); ++j) {
      f[i] += f[j] * f[i - 1 - j];
    }
  }
  std::cout << f[k] << " " << k + 1 << std::endl;
  return 0;
}
