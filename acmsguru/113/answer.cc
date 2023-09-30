#include <iostream>
#include <string>
#include <vector>

bool is_prime(int number);
bool is_near_prime(int number);

int main() {
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    int number;
    std::cin >> number;
    if (is_near_prime(number)) {
      std::cout << "Yes" << std::endl;
    } else {
      std::cout << "No" << std::endl;
    }
  }
  return 0;
}

bool is_prime(int number) {
  if ((number <= 1)) {
    return false;
  }
  for (int i = 2; i * i <= number; ++i) {
    if ((number % i) == 0) {
      return false;
    }
  }
  return true;
}

bool is_near_prime(int number) {
  if (number <= 3) {
    return false;
  }
  for (int i = 2; i * i <= number; ++i) {
    if ((number % i) == 0) {
      if (is_prime(i) && is_prime(number / i)) {
        return true;
      } else {
        return false;
      }
    }
  }
  return false;
}