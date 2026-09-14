[![wakatime](https://wakatime.com/badge/user/ebdd5199-39f1-41e1-aa46-73f4e53797cb/project/1dcad521-d86d-4822-bbe4-343bef89364f.svg)](https://wakatime.com/badge/user/ebdd5199-39f1-41e1-aa46-73f4e53797cb/project/1dcad521-d86d-4822-bbe4-343bef89364f)

Started September 14, 2026

# Very Simple Math (Library) (VSM)

Another very creative name for a relatively simple-to-use single file math library, with Vectors and Matrices. And 0 dependencies!

This is basically the math stuff I wrote for VGE just all in one .hpp file.

# Features

- Vector3 in both integer and float forms
- Vector2 in both integer and float forms
- Any size matrix with transformations, perpective projection and look at functions
- General math functions like `ToDegree` and `ToRadian`, `Lerp`, `Min` and `Max`, `Sin`, `Tan` and `Cos`.

## How to use

Download [`vsm.hpp`](https://github.com/Skelebyte/VSM/releases) then include it.

## Example

```cpp
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
```
