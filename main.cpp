// other includes...
#include "vsm.hpp"

int main() {
  // other program code...

  vsm::Vector3f startPos(-10); // x: -10, y: -10, z: -10

  vsm::Vector3f endPos(10); // x: 10, y: 10, z: 10

  vsm::Vector3f pos = startPos;

  while (true) {
    // typically you would multiply 0.5f by deltaTime
    pos = vsm::Vector3f::Lerp(pos, endPos, 0.5f);

    // ...
  }

  return 0;
}
