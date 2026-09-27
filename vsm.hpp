#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#define VSM_VERSION_STR "1.1.3"

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
  template <typename T> static T ToRadians(const T _degrees) {
    // make sure noone is sneaking a Vector or something in
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    const Decimal result = static_cast<Decimal>(_degrees) *
                           static_cast<Decimal>(M_PI) / static_cast<T>(180.0f);

    assert(result == result); // NAN check

    return result;
  }

  template <typename T> static T ToDegrees(const T _radians) {
    // make sure nobody is sneaking a Vector or something in
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    const T result = _radians / M_PI * static_cast<T>(180.0f);

    assert(result == result); // NAN check

    return result;
  }

  template <typename T> static T Lerp(const T _a, const T _b, const float _t) {
    static_assert(
        typeid(T) != typeid(bool),
        "You cant use bools in this function! what are you thinking??");

    // in theory this function should work with Vectors, as long as they have
    // the + and - operators function defined.
    return _a + _t * (_b - _a);
  }

  template <typename T> static bool IsZeroApprox(const T _a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return Mathf::Abs(_a) < 0.00001f;
  }

  template <typename T> static T Min(const T _a, const T _b) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    if (_a <= _b) {
      return _a;
    } else {
      return _b;
    }
  }

  template <typename T> static T Max(const T _a, const T _b) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    if (_a >= _b) {
      return _a;
    } else {
      return _b;
    }
  }

  template <typename T> static T Abs(const T _a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    if (_a < 0) {
      return _a - (_a * 2);
    } else {
      return _a;
    }
  }

  template <typename T> static T Sin(const T _a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return std::sin(_a);
  }

  template <typename T> static T Cos(const T _a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return std::cos(_a);
  }

  template <typename T> static T Tan(const T _a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return std::tan(_a);
  }

  template <typename T> static T Sqrt(const T _a) {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    return std::sqrt(_a);
  }
};

template <uint S, typename T> struct Vec {
  using TType = T;

  Vec() {
    static_assert(std::is_fundamental_v<T> && typeid(T) != typeid(bool),
                  "T can only be a float, double, or an integer!");
  }

  virtual ~Vec() = default;
  T *Data() { return arr.data(); }

  [[nodiscard]] Decimal Length() const {
    Decimal result{};

    for (uint i = 0; i < S; i++) {
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

    for (uint i = 0; i < S; i++) {
      result.append(
          std::to_string(arr[i]) +
          (i != S - 1
               ? ", "
               : "") // should prevent a , from appearing after the last number
      );
    }

    result.append(")");

    return result;
  }

  // [[nodiscard]] Decimal Dot() {
  //   Decimal result {};
  //
  //
  // }

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

  Vec<S, T> operator+(const Vec<S, T> &_other) const {
    Vec<S, T> result;

    for (uint i = 0; i < S; i++) {
      result.arr[i] = arr[i] + _other.arr[i];
    }

    return result;
  }

  Vec<S, T> operator+(const Decimal &_other) const {
    Vec<S, T> result;

    for (uint i = 0; i < S; i++) {
      result.arr[i] = arr[i] + _other;
    }

    return result;
  }

  Vec<S, T> &operator+=(const Vec<S, T> &_other) {
    for (uint i = 0; i < S; i++) {
      arr[i] += _other[i];
    }

    return *this;
  }

  Vec<S, T> &operator+=(const Decimal &_other) {
    for (uint i = 0; i < S; i++) {
      arr[i] += _other;
    }

    return *this;
  }

  // ------ Subtraction ------

  Vec<S, T> operator-(const Vec<S, T> &_other) const {
    Vec<S, T> result;

    for (uint i = 0; i < S; i++) {
      result.arr[i] = arr[i] - _other.arr[i];
    }

    return result;
  }

  Vec<S, T> operator-(const Decimal &_other) const {
    Vec<S, T> result;

    for (uint i = 0; i < S; i++) {
      result.arr[i] = arr[i] - _other;
    }

    return result;
  }

  Vec<S, T> &operator-=(const Vec<S, T> &_other) {
    for (uint i = 0; i < S; i++) {
      arr[i] -= _other[i];
    }

    return *this;
  }

  Vec<S, T> &operator-=(const Decimal &_other) {
    for (uint i = 0; i < S; i++) {
      arr[i] -= _other;
    }

    return *this;
  }

  // ------ Multiplication ------

  Vec<S, T> operator*(const Vec<S, T> &_other) const {
    Vec<S, T> result;

    for (uint i = 0; i < S; i++) {
      result.arr[i] = arr[i] * _other.arr[i];
    }

    return result;
  }

  Vec<S, T> operator*(const Decimal &_other) const {
    Vec<S, T> result;

    for (uint i = 0; i < S; i++) {
      result.arr[i] = arr[i] * _other;
    }

    return result;
  }

  Vec<S, T> &operator*=(const Vec<S, T> &_other) {
    for (uint i = 0; i < S; i++) {
      arr[i] *= _other[i];
    }

    return *this;
  }

  Vec<S, T> &operator*=(const Decimal &_other) {
    for (uint i = 0; i < S; i++) {
      arr[i] *= _other;
    }

    return *this;
  }

  // ------ ------

  Vec<S, T> &operator=(const Vec<S, T> &_other) = default;

  // unsure about this function, may cause issues?? not sure (27/09/26)
  Vec<S, T> &operator=(const Decimal &_other) {
    for (uint i = 0; i < S; i++) {
      arr[i] = _other;
    }

    return *this;
  }

  bool operator==(const Vec<S, T> &_other) const {
    for (uint i = 0; i < S; i++) {
      if (arr[i] != _other.arr[i]) {
        return false;
      }
    }

    return true;
  }

protected:
  std::array<T, S> arr{};
};

/*
 * TODO attempt to find a workaround for specifying x y z and so on each new
 *  vector struct made.
 */
// ------ Vec3f ------
struct Vec3f : Vec<3, float> {
  float &x = arr[0];
  float &y = arr[1];
  float &z = arr[2];

  Vec3f() = default;
  Vec3f(const Vec<3, float> &_other) : Vec<3, float>(_other) {}

  explicit Vec3f(const float _xyz) {
    x = _xyz;
    y = _xyz;
    z = _xyz;
  }

  Vec3f(const float _x, const float _y, const float _z) {
    x = _x;
    y = _y;
    z = _z;
  }

  static Vec3f Cross(const Vec3f &_a, const Vec3f &_b) {
    return {_a.y * _b.z - _a.z * _b.y, _a.z * _b.x - _a.x * _b.z,
            _a.x * _b.y - _a.y * _b.x};
  }
  static Decimal Dot(const Vec3f &_a, const Vec3f &_b) {
    return static_cast<Decimal>(_a.x * _b.x + _a.y * _b.y + _a.z * _b.z);
  }
};

// ------ Vec2f ------
struct Vec2f : Vec<2, float> {
  float &x = arr[0];
  float &y = arr[1];

  Vec2f() = default;
  Vec2f(const Vec<2, float> &_other) : Vec<2, float>(_other) {}

  explicit Vec2f(const float _xy) {
    x = _xy;
    y = _xy;
  }

  Vec2f(const float _x, const float _y) {
    x = _x;
    y = _y;
  }

public:
  // static float Dot(const Vec2f &_a, const Vec2f &_b) {
  //   return static_cast<Decimal>(_a.x * _b.x + _a.y * _b.y);
  // }
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

  float data[R * C] { };

  static constexpr uint ROWS = R;
  static constexpr uint COLUMNS = C;
  static constexpr uint ENTRIES = R * C;

  explicit Matrix(const bool identity = false) {
    Zero();
    if (identity) {
      Identity();
    }
  }

  void Zero() {
    for (int i = 0; i < ENTRIES; i++) {
      data[i] = 0;
    }
  }

  void Identity() {
    if (!IsSquareMatrix()) {
      return;
    }

    int n = 0;
    for (int i = 0; i < ENTRIES; i++) {
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
    if (cIdx > COLUMNS) {
      return 0.0f;
    }

    if (rIdx > ROWS) {
      return 0.0f;
    }

    return data[rIdx + major * cIdx];
  }

  /* !
   * not entirely happy with calling the 4th arg "major" as i dont think its
   * entirely accurate (as in if its columns major use ROWS, if row major use
   * COLUMNS).
   */
  void SetEntry(const uint cIdx, const uint rIdx, float value,
                const uint major = ROWS) {
    if (cIdx > COLUMNS) {
      // Logger::LOG("There are only " + ToString(COLUMNS) + " (highest index: "
      // +
      //             ToString(COLUMNS - 1) + ") columns in this matrix! Column "
      //             + ToString(cIdx) + " is out of bounds!");
      return;
    }

    if (rIdx > ROWS) {
      // Logger::LOG("There are only " + ToString(ROWS) + " (highest index: " +
      //             ToString(ROWS - 1) + ") rows in this matrix! Row " +
      //             ToString(rIdx) + " is out of bounds!");
      return;
    }

    data[rIdx + major * cIdx] = value;
  }

  [[nodiscard]] std::string ToString() const {
    std::string out;

    for (int row = 0; row < ROWS; row++) {
      out += "[ ";
      for (int col = 0; col < COLUMNS; col++) {
        out += std::to_string(GetEntry(col, row)) + " ";
      }
      out += "]\n";
    }

    return out;
  }

  Matrix operator*(float _other) {
    Matrix<R, C> out;

    for (int i = 0; i < ENTRIES; i++) {
      out.data[i] = data[i] * _other;
    }

    return out;
  }

  // https://stackoverflow.com/a/22149009 -  M Oehm Mar 3, 2014. (CC
  // BY-SA 3.0)
  template <uint R2 = R, uint C2 = 1>
  Matrix<R2, C2> operator*(const Matrix<C, C2> &_other) {

    // AMOUNT OF COLUMNS OF THE LEFT MATRIX MUST BE EQUAL TO THE AMOUNT OF ROWS
    // OF THE LEFT MATRIX
    assert(COLUMNS == _other.ROWS);

    // resulting matrix has the amount of columns of the right matrix and
    // the amount of rows the left matrix
    Matrix<ROWS, C2> out;

    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < _other.COLUMNS; col++) {
        out.data[row * ROWS + col] = 0;
        float sum = 0.0f;
        for (int i = 0; i < COLUMNS; i++) {
          sum += data[i * ROWS + col] * _other.data[row * _other.ROWS + i];
        }
        out.data[row * ROWS + col] = sum;
      }
    }

    return out;
  }

  Matrix &operator=(const Matrix<R, C> &_other) {
    if (this == _other) {
      return *this;
    }

    for (int i = 0; i < 16; i++) {
      this->data[i] = _other.data[i];
    }

    return *this;
  }

  float &operator[](uint i) {
    if (i < 0)
      return data[0];
    if (i > ENTRIES)
      return data[0];

    return data[i];
  }

  float &operator[](uint i) const {
    if (i < 0)
      return data[0];
    if (i > ENTRIES)
      return data[0];

    return data[i];
  }

  /* ------------ 4x4 Matrix Specific Functions ------------ */

  void Transform(const Vec3f &position, const Vec3f &rotation,
                 const Vec3f &scale) {

    static_assert(R == 4 && C == 4,
                  "This function only works with 4x4 matrices!");

    Identity();

    Matrix<4, 4> pos(true);
    pos.SetTranslation(position);
    Matrix<4, 4> rot(true);
    rot.SetRotation(rotation);
    Matrix<4, 4> sca(true);
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

    const Vec3f fwd = (target - eye).Normalized(); // forward
    const Vec3f rht = Vec3f::Cross(fwd, eyeUp);    // right
    const Vec3f up = Vec3f::Cross(rht, fwd);       // up

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

    SetEntry(3, 0, -Vec3f::Dot(rht, eye));
    SetEntry(3, 1, -Vec3f::Dot(up, eye));
    SetEntry(3, 2, Vec3f::Dot(fwd, eye));
  }

  // https://stackoverflow.com/a/53366142 - Pmsmm Nov 18, 2018 (CC BY-SA 4.0)
  void Perspective(const float fovDeg, const float aspect, const float near,
                   const float far) {
    // This function only works with 4x4 matrices!
    static_assert(R == 4 && C == 4,
                  "This function only works with 4x4 matrices!");

    const float fovRad = Mathf::ToRadians(fovDeg);
    const float tanFov = Mathf::Tan(fovRad / 2);

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
