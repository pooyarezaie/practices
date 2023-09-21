#include <algorithm>
#include <iostream>
#include <set>

const int MAX_N = 100 + 5;
const int MAX_LOG_A = 60 + 5;

long long int a[MAX_N];

int bit_width(long long int input);

int main() {
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
  }

  auto max_w = bit_width(*std::max_element(a, a + n));

  std::set<int> answer_indexs;
  for (int w = max_w - 1; w >= 0; --w) {
    for (int i = 0; i < n; ++i) {
      if (answer_indexs.find(i) != answer_indexs.end()) {
        continue;
      }

      if (a[i] & (1ll << w)) {
        for (int j = 0; j < n; ++j) {
          if (j != i && (a[j] & (1ll << w))) {
            a[j] ^= a[i];
          }
        }
        answer_indexs.insert(i);
        break;
      }
    }
  }

  long long int answer = 0;
  for (int i : answer_indexs) {
    answer ^= a[i];
  }
  std::cout << answer << std::endl;
  return 0;
}

int bit_width(long long int input) {
  int width = 1;
  while (input >>= 1) {
    ++width;
  }
  return width;
}