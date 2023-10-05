typedef unsigned long long u64;

extern "C" u64 test1_linear_flow(u64 n) {
  n >>= 3;
  n *= 3;
  n += 1337;
  return n;
}
