#include "vsm.hpp"
#include <iostream>

using namespace vsm;

int main() {

  Decimal d;

  const Vector3f v1(1.14f, 2.14f, 3.14f);
  Vector3f v2(0.14f, 2.14f, 3.14f);

  v2 -= v1;

  for (auto i = 0; i < 3; i++) {
    std::cout << v2.Data()[i] << std::endl;
  }

  return 0;
}
