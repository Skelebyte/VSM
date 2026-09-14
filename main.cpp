#include "vsm.hpp"
#include <iostream>

#define WIDTH 1920.0f
#define HEIGHT 1080.0f
#define ASPECT WIDTH / HEIGHT

int main() {
  vsm::Matrix<4, 4> projection;
  projection.Perspective(75.0f, ASPECT, 0.01f, 100.0f);

  std::cout << "Perspective projection: \n"
            << projection.ToString() << std::endl;

  return 0;
}
