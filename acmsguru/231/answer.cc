#include <iostream>
#include <utility>
#include <vector>

const int MAX_N = 1000 * 1000 + 100;

bool is_prime[MAX_N];

void find_primes();

int main() {
  int n;
  std::cin >> n;

  find_primes();

  std::vector<std::pair<int, int>> pairs;

  for (int i = 2; i < (n - 1); ++i) {
    if (is_prime[i] && is_prime[2 + i]) {
      pairs.push_back(std::make_pair(2, i));
    }
  }

  std::cout << pairs.size() << std::endl;
  for (auto i : pairs) {
    std::cout << i.first << " " << i.second << std::endl;
  }

  return 0;
}

void find_primes() {
  std::fill(is_prime, is_prime + MAX_N, true);
  is_prime[0] = is_prime[1] = false;

  for (int i = 2; i <= MAX_N; ++i) {
    if (is_prime[i]) {
      for (int k = 2; i * k <= MAX_N; ++k) {
        is_prime[k * i] = false;
      }
    }
  }
}