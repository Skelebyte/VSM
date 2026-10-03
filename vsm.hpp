/*
 * zlib License
 *
 * (C) 2026 Skelebyte
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * . This notice may not be removed or altered from any source distribution.
 */

#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <string>

#define VSM_VERSION_STR "1.1.5"

#ifndef VSM_DECIMAL
#ifdef VSM_DECIMAL_AS_DOUBLE
#define VSM_DECIMAL double
#define VSM_DECIMAL_ZERO 0.0
#define VSM_DECIMAL_ONE 1.0
#else
#define VSM_DECIMAL float
#define VSM_DECIMAL_ZERO 0.0f
#define VSM_DECIMAL_ONE 1.0f
#endif
#endif

namespace vsm {

// Define VSM_DECIMAL_AS_DOUBLE to use double instead of float
typedef VSM_DECIMAL Decimal;

struct Mathf {
  template <typename T> static T ToRadians(const T degrees) {
    // make sure noone is sneaking a Vector or something in
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    const Decimal result = static_cast<Decimal>(degrees) *
                           static_cast<Decimal>(M_PI) / static_cast<T>(180.0f);

    assert(result == result); // NAN check

    return result;
  }

  template <typename T> static T ToDegrees(const T radians) {
    // make sure nobody is sneaking a Vector or something in
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    const T result = radians / M_PI * static_cast<T>(180.0f);

    assert(result == result); // NAN check

    return result;
  }

  template <typename T> static T Lerp(const T a, const T b, const float t) {
    static_assert(
        typeid(T) != typeid(bool),
        "You cant use bools in this function! what are you thinking??");

    // in theory this function should work with Vectors, as long as they have
    // the + and - operators function defined.
    return a + t * (b - a);
  }

  template <typename T> static bool IsZeroApprox(const T a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return Mathf::Abs(a) < 0.00001f;
  }

  template <typename T> static T Min(const T a, const T b) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    if (a <= b) {
      return a;
    } else {
      return b;
    }
  }

  template <typename T> static T Max(const T a, const T b) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    if (a >= b) {
      return a;
    } else {
      return b;
    }
  }

  template <typename T> static T Abs(const T a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    if (a < 0) {
      return a - (a * 2);
    } else {
      return a;
    }
  }

  template <typename T> static T Sin(const T a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return std::sin(a);
  }

  template <typename T> static T Cos(const T a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return std::cos(a);
  }

  template <typename T> static T Tan(const T a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return std::tan(a);
  }

  template <typename T> static T Sqrt(const T a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return std::sqrt(a);
  }
};

template <uint S, typename T> struct Vec {
  using TType = T;
  std::array<T, S> arr;

  Vec() {
    static_assert(std::is_fundamental_v<T> && typeid(T) != typeid(bool),
                  "T can only be a float, double, or an integer!");
  }

  // i think theres some conversion happening when this func gets called...
  Vec(const std::array<T, S> &list) : arr(list) {}

  explicit Vec(const T values) {
    for (uint i{0}; i < S; i++) {
      arr[i] = values;
    }
  }

  virtual ~Vec() = default;
  T *Data() { return arr.data(); }

  [[nodiscard]] Decimal Length() const {
    Decimal result{};

    for (uint i{0}; i < S; i++) {
      result += arr[i] * arr[i];
    }

    return Mathf::Sqrt(result);
  }

  [[nodiscard]] Vec<S, T> Normalized() const {
    Vec<S, T> result{};
    const Decimal len{Length()};
    for (uint i{0}; i < S; i++) {
      result[i] = static_cast<Decimal>(arr[i]) / len;
    }

    // make sure this is valid
    assert(result.Length() != NAN);
    // make sure its actually normalized
    assert(result.Length() == VSM_DECIMAL_ONE);

    return result;
  }

  [[nodiscard]] std::string ToString() const {
    std::string result{"("};

    for (uint i{0}; i < S; i++) {
      result.append(std::to_string(arr[i]) +
                    (i != S - 1 ? ", " : "") // should prevent a comma from
                                             // appearing after the last number
      );
    }

    result.append(")");

    return result;
  }

  [[nodiscard]] Decimal Dot(const Vec<S, T> &other) const {
    Decimal result{};

    for (uint i{0}; i < S; i++) {
      result += arr[i] * other.arr[i];
    }

    return result;
  }

  [[nodiscard]] Vec<3, T> Cross(const Vec<3, T> &other) const {
    static_assert(S == 3,
                  "This function only works on Vec types where S is 3.");

    return {{arr[1] * other[2] - arr[2] * other[1],
             arr[2] * other[0] - arr[0] * other[2],
             arr[0] * other[1] - arr[1] * other[0]}};
  }

  T &operator[](const uint i) {
    // index out of bounds check
    assert(i < S);

    return arr[i];
  }

  const T &operator[](const uint i) const {
    // index out of bounds check
    assert(i < S);

    return arr[i];
  }

  // ------ Addition ------

  Vec<S, T> operator+(const Vec<S, T> &other) const {
    Vec<S, T> result{};

    for (uint i{0}; i < S; i++) {
      result.arr[i] = arr[i] + other.arr[i];
    }

    return result;
  }

  Vec<S, T> operator+(const Decimal &other) const {
    Vec<S, T> result{};

    for (uint i{0}; i < S; i++) {
      result.arr[i] = arr[i] + other;
    }

    return result;
  }

  Vec<S, T> &operator+=(const Vec<S, T> &other) {
    for (uint i{0}; i < S; i++) {
      arr[i] += other[i];
    }

    return *this;
  }

  Vec<S, T> &operator+=(const Decimal &other) {
    for (uint i{0}; i < S; i++) {
      arr[i] += other;
    }

    return *this;
  }

  // ------ Subtraction ------

  Vec<S, T> operator-(const Vec<S, T> &other) const {
    Vec<S, T> result{};

    for (uint i{0}; i < S; i++) {
      result.arr[i] = arr[i] - other.arr[i];
    }

    return result;
  }

  Vec<S, T> operator-(const Decimal &other) const {
    Vec<S, T> result{};

    for (uint i{0}; i < S; i++) {
      result.arr[i] = arr[i] - other;
    }

    return result;
  }

  Vec<S, T> &operator-=(const Vec<S, T> &other) {
    for (uint i{0}; i < S; i++) {
      arr[i] -= other[i];
    }

    return *this;
  }

  Vec<S, T> &operator-=(const Decimal &other) {
    for (uint i{0}; i < S; i++) {
      arr[i] -= other;
    }

    return *this;
  }

  // ------ Multiplication ------

  Vec<S, T> operator*(const Vec<S, T> &other) const {
    Vec<S, T> result{};

    for (uint i{0}; i < S; i++) {
      result.arr[i] = arr[i] * other.arr[i];
    }

    return result;
  }

  Vec<S, T> operator*(const Decimal &other) const {
    Vec<S, T> result{};

    for (uint i{0}; i < S; i++) {
      result.arr[i] = arr[i] * other;
    }

    return result;
  }

  Vec<S, T> &operator*=(const Vec<S, T> &other) {
    for (uint i{0}; i < S; i++) {
      arr[i] *= other[i];
    }

    return *this;
  }

  Vec<S, T> &operator*=(const Decimal &other) {
    for (uint i{0}; i < S; i++) {
      arr[i] *= other;
    }

    return *this;
  }

  // ------ ------

  Vec<S, T> &operator=(const Vec<S, T> &other) = default;

  // unsure about this function, may cause issues?? not sure (27/09/26)
  Vec<S, T> &operator=(const Decimal &other) {
    for (uint i{0}; i < S; i++) {
      arr[i] = other;
    }

    return *this;
  }

  bool operator==(const Vec<S, T> &other) const {
    for (uint i{0}; i < S; i++) {
      if (arr[i] != other.arr[i]) {
        return false;
      }
    }

    return true;
  }
};

/*
 * TODO attempt to find a workaround for specifying x y z and so on each new
 *  vector struct made.
 */
// ------ Vec3f ------
struct Vec3f : Vec<3, float> {
  float &x{arr[0]};
  float &y{arr[1]};
  float &z{arr[2]};

  Vec3f() = default;
  explicit Vec3f(const Vec<3, float> &other) : Vec<3, float>(other) {}

  explicit Vec3f(const float xyz) {
    x = xyz;
    y = xyz;
    z = xyz;
  }

  Vec3f(const float x, const float y, const float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

// ------ Vec2f ------
struct Vec2f : Vec<2, float> {
  float &x{arr[0]};
  float &y{arr[1]};

  Vec2f() = default;
  explicit Vec2f(const Vec<2, float> &other) : Vec<2, float>(other) {}

  explicit Vec2f(const float xy) {
    x = xy;
    y = xy;
  }

  Vec2f(const float x, const float y) {
    this->x = x;
    this->y = y;
  }
};

// TODO quaternions
// struct Quaternion : Vec<4, float> {
//   TType x, y, z, w;
//
//   Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
//
//   TType *GetData() override {
//     data[0] = x;
//     data[1] = y;
//     data[2] = z;
//     data[3] = w;
//
//     return data;
//   }
// };

// ------ Matrix ------
template <uint R, uint C> struct Matrix {

  std::array<float, R * C> data{};

  static constexpr uint ROWS{R};
  static constexpr uint COLUMNS{C};
  static constexpr uint ENTRIES{R * C};

  explicit Matrix(const bool identity = false) {
    Zero();
    if (identity) {
      Identity();
    }
  }

  void Zero() {
    for (uint i{0}; i < ENTRIES; i++) {
      data[i] = 0;
    }
  }

  void Identity() {
    if (!IsSquareMatrix()) {
      return;
    }

    int n{0};
    for (uint i{0}; i < ENTRIES; i++) {
      if (n == 0) {
        data[i] = 1;
        n = COLUMNS;
      } else {
        data[i] = 0;
        n--;
      }
    }
  }

  static bool IsSquareMatrix() { return COLUMNS == ROWS; }

  /*
   * TODO not entirely happy with calling the 4th arg "major" as i dont think
   *  entirely accurate (as in if its columns major use ROWS, if row major use
   *  COLUMNS).
   */
  [[nodiscard]] float GetEntry(const uint cIdx, const uint rIdx,
                               const uint major = ROWS) const {
    static_assert(cIdx < COLUMNS, "cIdx must be less than COLUMNS");
    static_assert(rIdx < ROWS, "rIdx must be less than ROWS");

    return data[rIdx + major * cIdx];
  }

  /* !
   * not entirely happy with calling the 4th arg "major" as i dont think its
   * entirely accurate (as in if its columns major use ROWS, if row major use
   * COLUMNS).
   */
  void SetEntry(const uint cIdx, const uint rIdx, float value,
                const uint major = ROWS) {
    static_assert(cIdx < COLUMNS, "cIdx must be less than COLUMNS");
    static_assert(rIdx < ROWS, "rIdx must be less than ROWS");

    data[rIdx + major * cIdx] = value;
  }

  [[nodiscard]] std::string ToString() const {
    std::string out{};

    for (int row = 0; row < ROWS; row++) {
      out += "[ ";
      for (int col = 0; col < COLUMNS; col++) {
        out += std::to_string(GetEntry(col, row)) + " ";
      }
      out += "]\n";
    }

    return out;
  }

  Matrix operator*(const float other) {
    Matrix<R, C> out{};

    for (int i = 0; i < ENTRIES; i++) {
      out.data[i] = data[i] * other;
    }

    return out;
  }

  // https://stackoverflow.com/a/22149009 -  M Oehm Mar 3, 2014. (CC BY-SA 3.0)
  template <uint R2 = R, uint C2 = 1>
  Matrix<R2, C2> operator*(const Matrix<C, C2> &other) {

    // AMOUNT OF COLUMNS OF THE LEFT MATRIX MUST BE EQUAL TO THE AMOUNT OF ROWS
    // OF THE LEFT MATRIX
    assert(COLUMNS == other.ROWS);

    // resulting matrix has the amount of columns of the right matrix and
    // the amount of rows the left matrix
    Matrix<ROWS, C2> out{};

    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < other.COLUMNS; col++) {
        out.data[row * ROWS + col] = 0;
        float sum{0.0f};
        for (int i = 0; i < COLUMNS; i++) {
          sum += data[i * ROWS + col] * other.data[row * other.ROWS + i];
        }
        out.data[row * ROWS + col] = sum;
      }
    }

    return out;
  }

  Matrix &operator=(const Matrix<R, C> &other) {
    if (this == other) {
      return *this;
    }

    for (uint i{0}; i < ENTRIES; i++) {
      this->data[i] = other.data[i];
    }

    return *this;
  }

  float &operator[](const uint i) {
    assert(i > ENTRIES);

    return data[i];
  }

  float &operator[](const uint i) const {
    assert(i > ENTRIES);

    return data[i];
  }

  /* ------------ 4x4 Matrix Specific Functions ------------ */

  void Transform(const Vec3f &position, const Vec3f &rotation,
                 const Vec3f &scale) {

    static_assert(R == 4 && C == 4,
                  "This function only works with 4x4 matrices!");

    Identity();

    Matrix<4, 4> pos{true};
    pos.SetTranslation(position);
    Matrix<4, 4> rot{true};
    rot.SetRotation(rotation);
    Matrix<4, 4> sca{true};
    sca.SetScale(scale);

    *this = (pos * rot * sca);
  }

  void SetTranslation(const Vec3f &position) {
    // This function only works with 4x4 matrices!
    assert(R == 4 && C == 4);

    Identity();

    SetEntry(3, 0, position.x);
    SetEntry(3, 1, position.y);
    SetEntry(3, 2, position.z);
  }

  // https://github.com/g-truc/glm/blob/6f14f4792a0cde5d0cf2c910506724d61cb95834/glm/ext/matrix_transform.inl#L153
  void LookAt(const Vec3f &eye, const Vec3f &target, const Vec3f &eyeUp) {
    // This function only works with 4x4 matrices!
    assert(R == 4 && C == 4);

    const Vec3f fwd{(target - eye).Normalized()}; // forward
    const Vec3f rht{fwd.Cross(eyeUp)};            // right
    const Vec3f up{rht.Cross(fwd)};               // up

    Identity();
    SetEntry(0, 0, rht.x);
    SetEntry(0, 1, rht.y);
    SetEntry(0, 2, rht.z);

    SetEntry(1, 0, up.x);
    SetEntry(1, 1, up.y);
    SetEntry(1, 2, up.z);

    SetEntry(2, 0, -fwd.x);
    SetEntry(2, 1, -fwd.y);
    SetEntry(2, 2, -fwd.z);

    SetEntry(3, 0, -rht.Dot(eye));
    SetEntry(3, 1, -up.Dot(eye));
    SetEntry(3, 2, fwd.Dot(eye));
  }

  // https://stackoverflow.com/a/53366142 - Pmsmm Nov 18, 2018 (CC BY-SA 4.0)
  void Perspective(const float fovDeg, const float aspect, const float near,
                   const float far) {
    // This function only works with 4x4 matrices!
    static_assert(R == 4 && C == 4,
                  "This function only works with 4x4 matrices!");

    const float fovRad{Mathf::ToRadians(fovDeg)};
    const float tanFov{Mathf::Tan(fovRad / 2)};

    Zero();
    SetEntry(0, 0, 1 / (aspect * tanFov));
    SetEntry(1, 1, 1 / tanFov);
    SetEntry(2, 2, -((far + near) / (far - near)));
    SetEntry(2, 3, -1);
    SetEntry(3, 2, -((2 * far * near) / (far - near)));
  }

  /* ------------ 3x3 and larger Matrix Specific Functions ------------ */

  void SetRotation(const Vec3f &rotation) {
    // This function only works with 3x3 or larger matrices!
    assert(R >= 3 && C >= 3);

    Matrix<ROWS, COLUMNS> xRot(true);
    xRot.SetEntry(1, 1, Mathf::Cos(rotation.x));
    xRot.SetEntry(2, 1, -Mathf::Sin(rotation.x));

    xRot.SetEntry(1, 2, Mathf::Sin(rotation.x));
    xRot.SetEntry(2, 2, Mathf::Cos(rotation.x));

    Matrix<ROWS, COLUMNS> yRot(true);
    yRot.SetEntry(0, 0, Mathf::Cos(rotation.y));
    yRot.SetEntry(2, 0, Mathf::Sin(rotation.y));

    yRot.SetEntry(0, 2, -Mathf::Sin(rotation.y));
    yRot.SetEntry(2, 2, Mathf::Cos(rotation.y));

    Matrix<ROWS, COLUMNS> zRot(true);
    zRot.SetEntry(0, 0, Mathf::Cos(rotation.z));
    zRot.SetEntry(1, 0, -Mathf::Sin(rotation.z));
    zRot.SetEntry(0, 1, Mathf::Sin(rotation.z));
    zRot.SetEntry(1, 1, Mathf::Cos(rotation.z));

    *this = (xRot * yRot * zRot);
  }
  void SetScale(const Vec3f &scale) {
    // This function only works with 3x3 or larger matrices!
    assert(R >= 3 && C >= 3);

    Identity();
    SetEntry(0, 0, scale.x);
    SetEntry(1, 1, scale.y);
    SetEntry(2, 2, scale.z);
  }
};

} // namespace vsm
