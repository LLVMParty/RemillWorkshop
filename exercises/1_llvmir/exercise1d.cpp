#include <stdint.h>
#include <stdio.h>

extern "C" {
uint32_t ps3_random_number();
uint32_t add_42(uint32_t x);
uint32_t bytes_to_bits(uint32_t bytes);
uint32_t vm_operation(bool do_add, uint32_t x, uint32_t y);
void pass_to_store_value(uint32_t n);
uint32_t upper_32_bits(uint64_t n);
}

#define EXPECT_EQ(name, expr, expected)                                                            \
  {                                                                                                \
    uint64_t result = (expr);                                                                      \
    printf("[%s] %s -> %llu\n", name, #expr, result);                                              \
    if (result == expected) {                                                                      \
      printf("[%s] SUCCESS\n", name);                                                              \
    } else {                                                                                       \
      printf("[%s] FAILURE (expected: %llu, got: %llu)\n", name, (uint64_t)expected, result);      \
    }                                                                                              \
  }

static uint32_t g_stored = 0;

extern "C" void store_value(uint32_t n, uint32_t key) {
  if ((n & 1) == 0) {
    if (key == 99) {
      g_stored = n;
    } else {
      g_stored = 0;
    }
  } else {
    if (key == 512) {
      g_stored = n;
    } else {
      g_stored = 0;
    }
  }
}

int main() {
  EXPECT_EQ("TEST1", ps3_random_number(), 42);
  puts("");
  EXPECT_EQ("TEST2", add_42(0), 42);
  EXPECT_EQ("TEST2", add_42(66), 42 + 66);
  puts("");
  EXPECT_EQ("TEST3", bytes_to_bits(0), 0);
  EXPECT_EQ("TEST3", bytes_to_bits(1), 8);
  EXPECT_EQ("TEST3", bytes_to_bits(2), 16);
  EXPECT_EQ("TEST3", bytes_to_bits(8), 64);
  puts("");
  EXPECT_EQ("TEST5", vm_operation(true, 1, 2), 3);
  EXPECT_EQ("TEST5", vm_operation(true, 55, 11), 66);
  EXPECT_EQ("TEST5", vm_operation(false, 55, 11), 44);
  EXPECT_EQ("TEST5", vm_operation(false, 42, 42), 0);
  puts("");
  pass_to_store_value(55);
  EXPECT_EQ("TEST4", g_stored, 55);
  pass_to_store_value(54);
  EXPECT_EQ("TEST4", g_stored, 54);
  puts("");
  EXPECT_EQ("TEST6", upper_32_bits(0x12345678BBBBBBDD), 0x12345678);
  EXPECT_EQ("TEST6", upper_32_bits(0x1337), 0);
  EXPECT_EQ("TEST6", upper_32_bits(0x0000000800000012), 8);
}