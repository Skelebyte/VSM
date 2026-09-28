#include "vsm.hpp"
#include <iostream>

using namespace vsm;

int main() {
  const Vec<3, float> a{{1, 2, 3}};
  const Vec3f b{4, 5, 6};

  Vec<3, float> c{a.Cross(b)};

  return 0;
}
