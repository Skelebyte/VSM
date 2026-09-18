#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

#define VSM_VERSION_STR "1.0.3"

namespace vsm {

struct Mathf {
  template <typename T> static T ToRadians(const T _degrees) {
    // make sure noone is sneaking a Vector or something in
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    const T result = _degrees * static_cast<T>(M_PI) / static_cast<T>(180.0f);

    assert(result == result); // NAN check

    return result;
  }

  template <typename T> static T ToDegrees(const T _radians) {
    // make sure nobody is sneaking a Vector or something in
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");

    const T result = _radians / static_cast<T>(M_PI) * static_cast<T>(180.0f);

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

  static bool IsZeroApprox(const float _a) { return Mathf::Abs(_a) < 0.00001f; }

  static bool IsZeroApprox(const double _a) {
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

template <uint S, typename T = float> struct Vector {
  using TType = T;

  virtual ~Vector() = default;
  virtual T *GetData() { return data; }

protected:
  T data[S]{};
};

// TODO attempt to find a workaround for specifying x y z and so on each new
// vector struct made.
struct Vector3f : Vector<3, float> {
  TType x, y, z;

  Vector3f() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }

  explicit Vector3f(const TType _xyz) {
    x = _xyz;
    y = _xyz;
    z = _xyz;
  }

  Vector3f(const TType _x, const TType _y, const TType _z) {
    x = _x;
    y = _y;
    z = _z;
  }

  TType *GetData() override {
    data[0] = x;
    data[1] = y;
    data[2] = z;

    return data;
  }

public:
  static Vector3f Cross(const Vector3f &_a, const Vector3f &_b) {
    return {_a.y * _b.z - _a.z * _b.y, _a.z * _b.x - _a.x * _b.z,
            _a.x * _b.y - _a.y * _b.x};
  }
  static float Dot(const Vector3f &_a, const Vector3f &_b) {
    return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z;
  }

  [[nodiscard]]  float Length() const {
    return Mathf::Sqrt(x * x + y * y + z * z);
  }


  [[nodiscard]] Vector3f Normalized() const {
    Vector3f norm = *this;
    const float len = this->Length();

    norm.x /= len;
    norm.y /= len;
    norm.z /= len;

    // make sure this is valid
    assert(norm.Length() != NAN);
    // make sure its actually normalized
    assert(norm.Length() == 1.0f);

    return norm;
  }

  [[nodiscard]] std::string ToString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }

  Vector3f operator+(const Vector3f &_other) const {
    return {+_other.x, y + _other.y, z + _other.z};
  }

  template <typename T> Vector3f operator+(const T _other) const {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");
    const T castValue = static_cast<T>(_other);
    return {x + castValue, y + castValue, z + castValue};
  }

  // TODO finish these new functions for the vector you can add subtract and
  // multiply by scalar values

  template <typename T> Vector3f operator-(const T _other) const {
    static_assert(
        std::is_fundamental_v<T> && typeid(T) != typeid(bool),
        "This function only works if T is a float, double, or an integer!");
    const T castValue = static_cast<T>(_other);
    return {x - castValue, y - castValue, z - castValue};
  }

  Vector3f operator-(const Vector3f &_other) const {
    return { x - _other.x, y - _other.y, z - _other.z};
  }

  Vector3f operator*(const Vector3f &_other) const {
    return { x * _other.x, y * _other.y, z * _other.z};
  }

  Vector3f operator*(const float &_other) const {
    return { x * _other, y * _other, z * _other};
  }

  Vector3f &operator=(const Vector3f &_other) {
    x = _other.x;
    y = _other.y;
    z = _other.z;

    return *this;
  }

  void operator+=(const Vector3f &_other) {
    x += _other.x;
    y += _other.y;
    z += _other.z;
  }

  void operator-=(const Vector3f &_other) {
    x -= _other.x;
    y -= _other.y;
    z -= _other.z;
  }

  bool operator==(const Vector3f &_other) const {
    return (x == _other.x && y == _other.y && z == _other.z);
  }
};

struct Vector3i : Vector<3, int32_t> {
  TType x, y, z;

  Vector3i() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }

  explicit Vector3i(const TType _xyz) {
    x = _xyz;
    y = _xyz;
    z = _xyz;
  }

  Vector3i(const TType _x, const TType _y, const TType _z) {
    x = _x;
    y = _y;
    z = _z;
  }

  int *GetData() override {
    data[0] = x;
    data[1] = y;
    data[2] = z;

    return data;
  }

public:
  static Vector3i Cross(const Vector3i &_a, const Vector3i &_b) {
    return {
      _a.y * _b.z - _a.z * _b.y, _a.z * _b.x - _a.x * _b.z,
            _a.x * _b.y - _a.y * _b.x};
  }
  static float Dot(const Vector3i &_a, const Vector3i &_b) {
    return static_cast<float>(_a.x * _b.x + _a.y * _b.y + _a.z * _b.z);
  }

  [[nodiscard]] float Length() const {
    return Mathf::Sqrt(static_cast<float>(x * x + y * y + z * z));
  }

  [[nodiscard]] std::string ToString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }

  Vector3i operator+(const Vector3i &_other) const {
    return { x + _other.x, y + _other.y, z + _other.z};
  }

  Vector3i operator-(const Vector3i &_other) const {
    return { x - _other.x, y - _other.y, z - _other.z};
  }

  Vector3i operator*(const Vector3i &_other) const {
    return { x * _other.x, y * _other.y, z * _other.z};
  }

  Vector3i operator*(const int &_other) const {
    return { x * _other, y * _other, z * _other};
  }

  Vector3i &operator=(const Vector3i &_other) {
    x = _other.x;
    y = _other.y;
    z = _other.z;

    return *this;
  }

  void operator+=(const Vector3i &_other) {
    x += _other.x;
    y += _other.y;
    z += _other.z;
  }

  void operator-=(const Vector3i &_other) {
    x -= _other.x;
    y -= _other.y;
    z -= _other.z;
  }

  bool operator==(const Vector3i &_other) const {
    return (x == _other.x && y == _other.y && z == _other.z);
  }
};

struct Vector2f : Vector<2, float> {
  TType x, y;

  Vector2f() {
    x = 0.0f;
    y = 0.0f;
  }

  explicit Vector2f(const TType _xy) {
    x = _xy;
    y = _xy;
  }

  Vector2f(const TType _x, const TType _y) {
    x = _x;
    y = _y;
  }

  float *GetData() override {
    data[0] = x;
    data[1] = y;

    return data;
  }

public:
  static float Dot(const Vector2f &_a, const Vector2f &_b) {
    return _a.x * _b.x + _a.y * _b.y;
  }

  [[nodiscard]] float Length() const { return Mathf::Sqrt(x * x + y * y); }


  [[nodiscard]] Vector2f Normalized() const {
    Vector2f norm = *this;
    const float len = this->Length();

    norm.x /= len;
    norm.y /= len;

    return norm;
  }

  [[nodiscard]] std::string ToString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }

  Vector2f operator+(const Vector2f &_other) const {
    return { x + _other.x, y + _other.y};
  }

  Vector2f operator-(const Vector2f &_other) const {
    return { x - _other.x, y - _other.y};
  }

  Vector2f operator*(const Vector2f &_other) const {
    return { x * _other.x, y * _other.y};
  }

  Vector2f operator*(const float &_other) const {
    return { x * _other, y * _other};
  }

  Vector2f &operator=(const Vector2f &_other) {
    x = _other.x;
    y = _other.y;

    return *this;
  }

  void operator+=(const Vector2f &_other) {
    x += _other.x;
    y += _other.y;
  }

  void operator-=(const Vector2f &_other) {
    x -= _other.x;
    y -= _other.y;
  }

  bool operator==(const Vector2f &_other) const {
    return (x == _other.x && y == _other.y);
  }
};

struct Vector2i : Vector<2, int> {
  int x, y;

  Vector2i() {
    x = 0.0f;
    y = 0.0f;
  }

  explicit Vector2i(const int _xy) {
    x = _xy;
    y = _xy;
  }

  Vector2i(const int _x, const int _y) {
    x = _x;
    y = _y;
  }

  int *GetData() override {
    data[0] = x;
    data[1] = y;

    return data;
  }

public:
  static float Dot(const Vector2f &_a, const Vector2f &_b) {
    return _a.x * _b.x + _a.y * _b.y;
  }

  [[nodiscard]] float Length() const {
    return Mathf::Sqrt(static_cast<float>(x * x + y * y));
  }

  [[nodiscard]] std::string ToString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }

  Vector2i operator+(const Vector2i &_other) const {
    return { x + _other.x, y + _other.y};
  }

  Vector2i operator-(const Vector2i &_other) const {
    return { x - _other.x, y - _other.y};
  }

  Vector2i operator*(const Vector2i &_other) const {
    return { x * _other.x, y * _other.y};
  }

  Vector2i operator*(const float &_other) const {
    return {
      static_cast<int>(static_cast<float>(x) * _other),
            static_cast<int>(static_cast<float>(y) * _other)};
  }

  Vector2i &operator=(const Vector2i &_other) {
    x = _other.x;
    y = _other.y;

    return *this;
  }

  void operator+=(const Vector2i &_other) {
    x += _other.x;
    y += _other.y;
  }

  void operator-=(const Vector2i &_other) {
    x -= _other.x;
    y -= _other.y;
  }

  bool operator==(const Vector2i &_other) const {
    return (x == _other.x && y == _other.y);
  }
};

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
      // Logger::LOG("Matrix is not square! Cant make identity matrix.");
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

  // ! not entirely happy with calling the 4th arg "major" as i dont think its
  // ! entirely accurate (as in if its columns major use ROWS, if row major use
  // ! COLUMNS).
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

  Matrix operator*(float other) {
    Matrix<R, C> out;

    for (int i = 0; i < ENTRIES; i++) {
      out.data[i] = data[i] * other;
    }

    return out;
  }

  // https://stackoverflow.com/a/22149009 -  M Oehm Mar 3, 2014. (CC
  // BY-SA 3.0)
  template <uint R2 = R, uint C2 = 1>
  Matrix<R2, C2> operator*(const Matrix<C, C2> &other) {

    // AMOUNT OF COLUMNS OF THE LEFT MATRIX MUST BE EQUAL TO THE AMOUNT OF ROWS
    // OF THE LEFT MATRIX
    assert(COLUMNS == other.ROWS);

    // resulting matrix has the amount of columns of the right matrix and
    // the amount of rows the left matrix
    Matrix<ROWS, C2> out;

    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < other.COLUMNS; col++) {
        out.data[row * ROWS + col] = 0;
        float sum = 0.0f;
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

    for (int i = 0; i < 16; i++) {
      this->data[i] = other.data[i];
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

  void Transform(const Vector3f &position, const Vector3f &rotation,
                 const Vector3f &scale) {

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

  void SetTranslation(const Vector3f &position) {
    // This function only works with 4x4 matrices!
    assert(R == 4 && C == 4);

    Identity();

    SetEntry(3, 0, position.x);
    SetEntry(3, 1, position.y);
    SetEntry(3, 2, position.z);
  }

  // https://github.com/g-truc/glm/blob/6f14f4792a0cde5d0cf2c910506724d61cb95834/glm/ext/matrix_transform.inl#L153
  void LookAt(const Vector3f &eye, const Vector3f &target,
              const Vector3f &eyeUp) {
    // This function only works with 4x4 matrices!
    assert(R == 4 && C == 4);

    const Vector3f fwd = (target - eye).Normalized(); // forward
    const Vector3f rht = Vector3f::Cross(fwd, eyeUp); // right
    const Vector3f up = Vector3f::Cross(rht, fwd);    // up

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

    SetEntry(3, 0, -Vector3f::Dot(rht, eye));
    SetEntry(3, 1, -Vector3f::Dot(up, eye));
    SetEntry(3, 2, Vector3f::Dot(fwd, eye));
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

  void SetRotation(const Vector3f &rotation) {
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
  void SetScale(const Vector3f &scale) {
    // This function only works with 3x3 or larger matrices!
    assert(R >= 3 && C >= 3);

    Identity();
    SetEntry(0, 0, scale.x);
    SetEntry(1, 1, scale.y);
    SetEntry(2, 2, scale.z);
  }
};

} // namespace vsm
