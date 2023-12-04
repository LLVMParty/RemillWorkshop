typedef unsigned long long uint64_t;

extern "C" __attribute__((noinline)) uint64_t test3_complex_cfg(uint64_t n, uint64_t m) {
  if ((n & 3) == 0) {
    n += 5;
    if (n < m) {
      m += (n + n);
    } else {
      n += (m + m);
    }
  } else if (n * 2 < m) {
    m -= n;
  } else {
    n -= m;
  }
  return (n << 5) ^ m;
}

int main() {
  return test3_complex_cfg(13, 37);
}
