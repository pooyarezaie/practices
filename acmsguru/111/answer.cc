#include <algorithm>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

const int BASE = 1000, BASE_LEN = 3;
const std::size_t MAX_DIGITS = (1000 / BASE_LEN) + 100;

struct BigInt {
  unsigned long long int digit[MAX_DIGITS];
  int len;

  BigInt() {
    for (int i = 0; i < MAX_DIGITS; ++i) {
      digit[i] = 0;
    }
    len = 0;
  }
  BigInt(int n) : BigInt() {
    digit[0] = n;
    relax();
  }
  BigInt(const BigInt &bi) {
    for (int i = 0; i < MAX_DIGITS; ++i) {
      digit[i] = bi.digit[i];
    }
    len = bi.len;
  }
  BigInt(const BigInt *bi) {
    for (int i = 0; i < MAX_DIGITS; ++i) {
      digit[i] = bi->digit[i];
    }
    len = bi->len;
  }

  BigInt operator+(int n) const &;
  bool operator<(const BigInt &bi) const &;
  bool operator<=(const BigInt &bi) const &;
  BigInt operator+(const BigInt &bi) const &;
  BigInt operator/(int n) const &;
  BigInt operator*(const BigInt &bi) const &;
  void operator*=(int n) &;

  void relax();
};

std::istream &operator>>(std::istream &in, BigInt &bi);
std::ostream &operator<<(std::ostream &out, const BigInt &bi);

BigInt square_root(const BigInt &bi);

int main() {
  BigInt x;
  std::cin >> x;
  std::cout << square_root(x) << std::endl;
  return 0;
}
BigInt square_root(const BigInt &bi) {
  BigInt start, end = 1;
  while (end * end <= bi) {
    end *= 2;
  }
  start = end / 2;
  while ((start + 1) < end) {
    BigInt m = (start + end) / 2;
    if (m * m <= bi) {
      start = m;
    } else {
      end = m;
    }
  }
  return start;
}

BigInt BigInt::operator+(int n) const & {
  BigInt r(this);
  r.digit[0] += n;
  r.relax();
  return r;
}

bool BigInt::operator<(const BigInt &bi) const & {
  if (len < bi.len) {
    return true;
  }
  if (len > bi.len) {
    return false;
  }
  for (int i = len - 1; i >= 0; --i) {
    if (digit[i] < bi.digit[i]) {
      return true;
    }
    if (digit[i] > bi.digit[i]) {
      return false;
    }
  }
  return false;
}

bool BigInt::operator<=(const BigInt &bi) const & {
  return (bi < this) ? false : true;
}

BigInt BigInt::operator+(const BigInt &bi) const & {
  BigInt r;
  for (int i = 0; i < MAX_DIGITS; ++i) {
    r.digit[i] = digit[i] + bi.digit[i];
  }
  r.relax();
  return r;
}
void BigInt::operator*=(int n) & {
  for (int i = 0; i < len; ++i) {
    digit[i] *= n;
  }
  relax();
}

BigInt BigInt::operator/(int n) const & {
  BigInt r(this);
  for (int i = len - 1; i > 0; --i) {
    r.digit[i - 1] += (r.digit[i] % n) * BASE;
    r.digit[i] /= n;
  }
  r.digit[0] /= n;
  r.relax();
  return r;
}

BigInt BigInt::operator*(const BigInt &bi) const & {
  BigInt r;

  for (int i = 0; i < bi.len; ++i) {
    for (int j = 0; j < len; ++j) {
      if ((i + j) < MAX_DIGITS) {
        r.digit[i + j] += bi.digit[i] * digit[j];
      }
    }
  }
  r.relax();
  return r;
}

std::istream &operator>>(std::istream &in, BigInt &bi) {
  std::string number;
  in >> number;
  int number_of_digits = number.size() / BASE_LEN;
  std::vector<std::string> splited_number;

  std::reverse(number.begin(), number.end());
  for (int i = 0; i < number_of_digits; i++) {
    splited_number.push_back(number.substr(i * BASE_LEN, BASE_LEN));
  }
  if (number.size() % BASE_LEN != 0) {
    splited_number.push_back(number.substr(BASE_LEN * number_of_digits));
  }

  for (int i = splited_number.size() - 1; i >= 0; --i) {
    std::reverse(splited_number[i].begin(), splited_number[i].end());
    bi.digit[i] = std::stoi(splited_number[i]);
  }
  bi.relax();
  return in;
}

std::ostream &operator<<(std::ostream &out, const BigInt &bi) {
  if (bi.len == 0) {
    out << 0;
    return out;
  }
  out << bi.digit[bi.len - 1];
  for (int i = bi.len - 2; i >= 0; --i) {
    if (bi.digit[i] < 100) {
      out << 0;
    }
    if (bi.digit[i] < 10) {
      out << 0;
    }
    out << bi.digit[i];
  }
  return out;
}

void BigInt::relax() {
  for (int i = 0; i < MAX_DIGITS - 1; ++i) {
    digit[i + 1] += digit[i] / BASE;
    digit[i] %= BASE;
  }
  len = MAX_DIGITS;
  while (len > 0 && !digit[len - 1]) {
    --len;
  }
}
