#include <cstdint>

#define TEST_FUNCTION extern "C"

TEST_FUNCTION uint64_t test1_linear_flow(uint64_t n) {
  n >>= 3;
  n *= 3;
  n += 1337;
  return n;
}

TEST_FUNCTION uint64_t test2_if_else(uint64_t n) {
  if ((n & 1) == 0)
    return n >> 1;
  else
    return n << 1;
}

TEST_FUNCTION uint64_t test3_complex_cfg(uint64_t n, uint64_t m) {
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

TEST_FUNCTION uint64_t test4_simple_loop(uint64_t n) {
  uint64_t factorial = 1;
  for (uint64_t i = 2; i <= n; i++)
    factorial *= i;
  return factorial;
}

TEST_FUNCTION
uint64_t test5_simple_loop_phi(uint64_t n) {
  uint64_t x = 1;
  for (int i = 0; i < n; i++) {
    if ((i & 2) == 0) {
      x = 100;
    } else {
      x = 200;
    }
    // phi_x = { 100: True, 100: phi_i&2==0, 200: phi_i&2!=0 }
  }
  // phi_x = { 1: True, 100: True, 100: phi_i&2==0, 200: phi_i&2!=0 }
  if (n > 0 && x == 1) {
    // opaque
    return 0x1337;
  }
  return x;
}

TEST_FUNCTION uint64_t test6_nested_phi(uint64_t n) {
  uint64_t x = 0;
  if ((n & 2) == 0) {
    if ((n >> 32) == 0) {
      x = 1;
    } else {
      x = 2;
    }
  } else {
    if ((n >> 16) == 0) {
      x = 3;
    } else {
      x = 4;
    }
  }
  return x;
}

TEST_FUNCTION uint64_t test10_switch(uint64_t n) {
  switch (n) {
  case 1:
    return 1234;
  case 2:
    return 4567;
  case 3:
    return 8901;
  case 4:
    return n + n;
  case 5:
    return n * 55;
  case 6:
    return n + 68;
  case 7:
    return 8473;
  case 8:
    return 8473 ^ n;
  case 9:
    return 6748 & n;
  case 10:
    return 948;
  case 11:
    return 4827;
  default:
    return n & 0x1337;
  }
}
