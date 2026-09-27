#include "vsm.hpp"
#include <iostream>

using namespace vsm;

int main() {
  const Vec2f v1(1.14f, 2.14f);
  Vec2f v2(0.14f, 2.14f);

  Vec<5, float> v5{};
  v5[0] = 1;
  v5[1] = 2;
  v5[2] = 3;
  v5[3] = 4;
  v5[4] = 5;

  std::cout << v5.Length() << std::endl;

  v2 -= v1;

  std::cout << v2.ToString() << std::endl;

  return 0;
}
