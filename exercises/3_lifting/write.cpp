typedef unsigned long long uint64_t;

extern "C" void test_write_bug(uint64_t n, uint64_t *m) {
  n >>= 3;
  n *= 3;
  n += 1337;
  *m = n;
}
