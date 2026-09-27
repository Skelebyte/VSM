#include "vsm.hpp"
#include <iostream>

using namespace vsm;

int main() {

  const Vec<4, float> v1{1.14f};
  Vec<4, float> v2{{0.14f, 2.14f, 3.04f, 5.92f}};

  v2 -= v1;

  std::cout << v2.ToString() << std::endl;

  return 0;
}
