typedef unsigned long long uint64_t;

extern "C" uint64_t test_read_bug(uint64_t *n) {
  return *n + 1337;
}
