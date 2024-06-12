typedef unsigned long long uint64_t;

extern "C" uint64_t test4_memory(uint64_t n, uint64_t *m) {
  n >>= 3;
  n *= 3;
  n += 1337;
  *m = n;
  return 42;
}
