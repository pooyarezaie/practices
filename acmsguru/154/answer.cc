#include <iostream>

const int MAX_Q = 500 * 1000 * 1000 + 1000;

int lower_bound(int start, int end, int value);
int trailing_zeros(int n);

int main() {
  int q;
  std::cin >> q;

  int answer = lower_bound(1, MAX_Q, q);
  if (answer == -1) {
    std::cout << "No solution" << std::endl;
    return 0;
  }

  std::cout << answer << std::endl;
  return 0;
}

int lower_bound(int start, int end, int value) {
  if (end <= start) {
    return -1;
  }
  if (end <= (start + 2)) {
    for (int i = start; i < end; ++i) {
      if (trailing_zeros(i) == value) {
        return i;
      }
    }
    return -1;
  }

  int m = (start + end) / 2;
  int t = trailing_zeros(m);
  if (value > t) {
    return lower_bound(m + 1, end, value);
  } else {
    return lower_bound(start, m + 1, value);
  }
}

int trailing_zeros(int n) {
  int answer = 0;
  while (n > 0) {
    answer += n / 5;
    n /= 5;
  }
  return answer;
}