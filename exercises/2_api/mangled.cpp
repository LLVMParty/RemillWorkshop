#include <cstdio>

class MyNumber {
  int mValue;

  MyNumber(int value)
    : mValue(value) {
  }

public:
  static MyNumber *create(int value) {
    return new MyNumber(value);
  }

  int getValue() const {
    return mValue;
  }
};

int main() {
  return MyNumber::create(43)->getValue();
}
