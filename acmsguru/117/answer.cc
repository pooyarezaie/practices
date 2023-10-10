#include <iostream>
#include <map>
#include <vector>

typedef std::map<int, int> Factorization;

const int MAX_N = 10000 + 10;

bool is_prime[MAX_N];
std::vector<int> primes;
int numbers[MAX_N];

void find_primes();
Factorization factorize(int number);

int main() {
  int n, m, k;
  std::cin >> n >> m >> k;
  for (int i = 0; i < n; ++i) {
    std::cin >> numbers[i];
  }

  find_primes();

  auto k_factorization = factorize(k);

  int answer = 0;
  for (int i = 0; i < n; ++i) {
    auto number_i = factorize(numbers[i]);
    bool is_divisible = true;
    for (auto f : k_factorization) {
      if (number_i[f.first] * m < f.second) {
        is_divisible = false;
        break;
      }
    }
    if (is_divisible) {
      answer++;
    }
  }
  std::cout << answer << std::endl;
  return 0;
}

Factorization factorize(int number) {
  Factorization f;
  for (auto p : primes) {
    while (number % p == 0) {
      f[p] += 1;
      number /= p;
    }
  }
  return f;
}

void find_primes() {
  std::fill(is_prime, is_prime + MAX_N, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 2; i < MAX_N; ++i) {
    if (is_prime[i]) {
      primes.push_back(i);
      for (int k = 2; k * i < MAX_N; ++k) {
        is_prime[k * i] = false;
      }
    }
  }
}
