typedef unsigned long long uint64_t;

extern "C" uint64_t test2_linear_flow(uint64_t n, uint64_t m) {
  n >>= 3;
  n *= 3;
  m += 1337;
  return n + m;
}

int main() {
}
