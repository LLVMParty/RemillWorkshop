typedef unsigned long long uint64_t;

extern "C" uint64_t test1_linear_flow(uint64_t n) {
  n >>= 3;
  n *= 3;
  n += 1337;
  return n;
}
