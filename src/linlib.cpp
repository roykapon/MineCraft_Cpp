#include "linlib.h"

#define SWITCH(M, i, i2, tmp)                                                  \
  tmp = M[i2];                                                                 \
  M[i2] = M[i];                                                                \
  M[i] = tmp;

using namespace std;

// =========================== Vector ===========================

SCALAR Vector::operator[](int index) const {
  switch (index) {
  case X_AXIS:
    return x;
  case Y_AXIS:
    return y;
  case Z_AXIS:
    return z;
  case W:
    return w;
  default:
    cout << "Invalid index: " << index;
    return x;
  }
}

SCALAR &Vector::operator[](int index) {
  switch (index) {
  case X_AXIS:
    return x;
  case Y_AXIS:
    return y;
  case Z_AXIS:
    return z;
  case W:
    return w;
  default:
    cout << "Invalid index: " << index;
    return x;
  }
}

Vector operator+(const Vector &v1, const Vector &v2) {
  // sets w to be the same as v1
  return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w);
}

Vector &Vector::operator+=(const Vector &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector operator-(const Vector &v1, const Vector &v2) {
  // sets w to be the same as v1
  return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w);
}

Vector &Vector::operator-=(const Vector &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector operator*(const Vector &v, SCALAR a) {
  // sets w to be the same as v1
  return Vector(v.x * a, v.y * a, v.z * a, v.w);
}

Vector &Vector::operator*=(SCALAR a) {
  x *= a;
  y *= a;
  z *= a;
  return *this;
}

Vector operator*(SCALAR a, const Vector &v) {
  // sets w to be the same as v1
  return Vector(v.x * a, v.y * a, v.z * a, v.w);
}

SCALAR operator*(const Vector &v1, const Vector &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector interpolate(const Vector &v1, const Vector &v2, SCALAR ratio) {
  return Vector(INTERPOLATE(v1.x, v2.x, ratio), INTERPOLATE(v1.y, v2.y, ratio),
                INTERPOLATE(v1.z, v2.z, ratio), v1.w);
}

void interpolate(const Vector &v1, const Vector &v2, SCALAR ratio,
                 Vector &res) {
  res.x = INTERPOLATE(v1.x, v2.x, ratio);
  res.y = INTERPOLATE(v1.y, v2.y, ratio);
  res.z = INTERPOLATE(v1.z, v2.z, ratio);
  res.w = v1.w;
}

ostream &operator<<(ostream &os, const Vector &v) {
  os << "(x = " << v.x << ", y = " << v.y << ", z = " << v.z << ", w = " << v.w
     << ")\n";
  return os;
}

size_t Vector_hash::operator()(const Vector &key) const {
  return hash<string>()(to_string(key.x) + to_string(key.y) + to_string(key.z));
}

SCALAR norm(const Vector &v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

SCALAR distance(const Vector &v1, const Vector &v2) {
  Vector diff = v1 - v2;
  return norm(diff);
}

Vector normalized(const Vector &v) {
  SCALAR n = norm(v);
  if (n == 0) {
    return v;
  }
  return v * (1.0f / n);
}

Vector normalized_horizontal(const Vector &v) {
  SCALAR n = sqrt(v.x * v.x + v.z * v.z);
  if (n == 0) {
    return v;
  }
  return Vector(v.x * (1.0f / n), v.y, v.z * (1.0f / n));
}

Vector cross(const Vector &v1, const Vector &v2) {
  // this is a direction vector so w=0
  return normalized(Vector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                           v1.x * v2.y - v1.y * v2.x, 0));
}

bool Vector::operator==(const Vector &v2) const {
  return x == v2.x && y == v2.y && z == v2.z;
}

bool Vector::operator!=(const Vector &v2) const {
  return !(x == v2.x && y == v2.y && z == v2.z);
}

// Vector round(Vector &v) {
//   return Vector((SCALAR)round(v.x / 2) * 2, (SCALAR)round(v.y / 2) * 2,
//                 (SCALAR)round(v.z / 2) * 2);
// }

// =========================== Matrix ===========================

Vector &Matrix::operator[](int index) { return vectors[index]; }

const Vector &Matrix::operator[](int index) const { return vectors[index]; }

Matrix operator+(const Matrix &M1, const Matrix &M2) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    res[i] = M1[i] + M2[i];
  }
  return res;
}

Matrix operator-(const Matrix &M1, const Matrix &M2) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    res[i] = M1[i] - M2[i];
  }
  return res;
}

Matrix operator*(const Matrix &M, SCALAR a) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    res[i] = M[i] * a;
  }
  return res;
}

Vector operator*(const Matrix &M, const Vector &v) {
  Vector res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    SCALAR sum = 0.0f;
    for (int j = 0; j < EXTENDED_AXES; j++) {
      sum += v[j] * M[i][j];
    }
    res[i] = sum;
  }
  return res;
}

Vector operator*(const Vector &v, const Matrix &M) {
  Vector res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    SCALAR sum = 0.0f;
    for (int j = 0; j < EXTENDED_AXES; j++) {
      sum += M[j][i] * v[j];
    }
    res[i] = sum;
  }
  return res;
}

Matrix operator*(const Matrix &M1, const Matrix &M2) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    for (int j = 0; j < EXTENDED_AXES; j++) {
      SCALAR sum = 0;
      for (int t = 0; t < EXTENDED_AXES; t++) {
        sum += M1[i][t] * M2[t][j];
      }
      res[i][j] = sum;
    }
  }
  return res;
}

ostream &operator<<(ostream &os, const Matrix &M) {
  for (int i = 0; i < EXTENDED_AXES; i++) {
    for (int j = 0; j < EXTENDED_AXES; j++) {
      os << i << "," << j << " = " << left << setw(MAX_NUM_LENGTH) << M[i][j];
    }
    os << "\n";
  }
  return os;
};

Matrix Matrix::rotate(int axis, SCALAR angle) {
  Matrix res;
  res.vectors[3] = Vector(0, 0, 0, 1);
  switch (axis) {
  case X_AXIS:
    res.vectors[0] = Vector(1, 0, 0, 0);
    res.vectors[1] = Vector(0, cos(angle), -sin(angle), 0);
    res.vectors[2] = Vector(0, sin(angle), cos(angle), 0);
    return res;
  case Y_AXIS:
    res.vectors[0] = Vector(cos(angle), 0, sin(angle), 0);
    res.vectors[1] = Vector(0, 1, 0, 0);
    res.vectors[2] = Vector(-sin(angle), 0, cos(angle), 0);
    return res;
  case Z_AXIS:
    res.vectors[0] = Vector(cos(angle), -sin(angle), 0, 0);
    res.vectors[1] = Vector(sin(angle), cos(angle), 0, 0);
    res.vectors[2] = Vector(0, 0, 1, 0);
    return res;
  }
  cout << "Invalid axis: " << axis;
  return res;
};

Matrix Matrix::shift(const Vector &pos) {
  Matrix res;
  res.vectors[0] = Vector(1, 0, 0, 0);
  res.vectors[1] = Vector(0, 1, 0, 0);
  res.vectors[2] = Vector(0, 0, 1, 0);
  res.vectors[3] = pos;
  return res;
}

Matrix I() {
  Matrix res;
  res.vectors[0] = Vector(1, 0, 0, 0);
  res.vectors[1] = Vector(0, 1, 0, 0);
  res.vectors[2] = Vector(0, 0, 1, 0);
  res.vectors[3] = Vector(0, 0, 0, 0);
  return res;
}

/** helper function for finding a pivot row.
 * returns whether there exists such row or not */
bool pivot(int i, int len, Matrix &copy, Matrix &res) {
  Vector tmp;
  for (int i2 = i + 1; i2 < len; i2++) {
    if (copy[i2][i] > EPSILON || copy[i2][i] < -EPSILON) {
      SWITCH(copy, i, i2, tmp);
      SWITCH(res, i, i2, tmp);
      return true;
    }
  }
  return false;
}

bool inverse(const Matrix &M, Matrix &res, int len) {
  res = I();
  Matrix copy = M;
  SCALAR a;
  bool invalid;
  for (int i = 0; i < len; i++) {
    if (copy[i][i] < EPSILON && copy[i][i] > -EPSILON) {
      if (!pivot(i, len, copy, res)) {
        return false;
      }
    }
    a = copy[i][i];
    copy[i] = copy[i] * (1 / a);
    res[i] = res[i] * (1 / a);
    for (int i2 = 0; i2 < len; i2++) {
      if (i2 != i) {
        a = copy[i2][i];
        copy[i2] = copy[i2] - a * copy[i];
        res[i2] = res[i2] - a * res[i];
      }
    }
    // cout << i << ": \n" << copy << endl;
    // cout << i << ": \n" << res << endl;
  }
  return true;
}
