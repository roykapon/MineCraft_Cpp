#include "linlib.h"

using namespace std;

// =========================== Vector ===========================

double Vector::operator[](int index) const {
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

double &Vector::operator[](int index) {
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
  Vector res;
  res.x = v1.x + v2.x;
  res.y = v1.y + v2.y;
  res.z = v1.z + v2.z;
  return res;
}

Vector operator-(const Vector &v1, const Vector &v2) {
  Vector res;
  res.x = v1.x - v2.x;
  res.y = v1.y - v2.y;
  res.z = v1.z - v2.z;
  return res;
}

Vector operator*(const Vector &v, double a) {
  Vector res;
  res.x = v.x * a;
  res.y = v.y * a;
  res.z = v.z * a;
  res.w = v.w;
  return res;
}

double operator*(const Vector &v1, const Vector &v2) {
  double sum = 0.0f;
  sum += v1.x * v2.x;
  sum += v1.y * v2.y;
  sum += v1.z * v2.z;
  return sum;
}

ostream &operator<<(ostream &os, const Vector &v) {
  os << "(x = " << v.x << ", y = " << v.y << ", z = " << v.z << ", w = " << v.w
     << ")\n";
  return os;
}

size_t Vector_hash::operator()(const Vector &key) const {
  return hash<string>()(to_string(key.x) + to_string(key.y) + to_string(key.z));
}

double norm(const Vector &v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

double distance(const Vector &v1, const Vector &v2) {
  Vector diff = v1 - v2;
  return norm(diff);
}

Vector normalized(const Vector &v) {
  double n = norm(v);
  if (n == 0) {
    return v * 0;
  }
  return v * (1.0f / n);
}

Vector cross(const Vector &v1, const Vector &v2) {
  Vector res;
  res.x = v1.y * v2.z - v1.z * v2.y;
  res.y = v1.z * v2.x - v1.x * v2.z;
  res.z = v1.x * v2.y - v1.y * v2.x;
  // this is a direction vector so w=0
  res.w = 0;
  return normalized(res);
}

bool Vector::operator==(const Vector &b2) const {
  return x == b2.x && (y == b2.y && z == b2.z);
}

// =========================== Matrix ===========================

Vector &Matrix::operator[](int index) { return vectors[index]; }

Matrix operator+(Matrix &M1, Matrix &M2) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    res[i] = M1[i] + M2[i];
  }
  return res;
}

Matrix operator-(Matrix &M1, Matrix &M2) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    res[i] = M1[i] - M2[i];
  }
  return res;
}

Matrix operator*(Matrix &M, double a) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    res[i] = M[i] * a;
  }
  return res;
}

Vector operator*(Matrix &M, Vector &v) {
  Vector res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    double sum = 0.0f;
    for (int j = 0; j < EXTENDED_AXES; j++) {
      sum += v[j] * M[i][j];
    }
    res[i] = sum;
  }
  return res;
}

Vector operator*(Vector &v, Matrix &M) {
  Vector res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    double sum = 0.0f;
    for (int j = 0; j < EXTENDED_AXES; j++) {
      sum += M[j][i] * v[j];
    }
    res[i] = sum;
  }
  return res;
}

Matrix operator*(Matrix &M1, Matrix &M2) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    for (int j = 0; j < EXTENDED_AXES; j++) {
      double sum = 0;
      for (int t = 0; t < EXTENDED_AXES; t++) {
        sum += M1[i][t] * M2[t][j];
      }
      res[i][j] = sum;
    }
  }
  return res;
}

ostream &operator<<(ostream &os, Matrix &M) {
  for (int i = 0; i < EXTENDED_AXES; i++) {
    for (int j = 0; j < EXTENDED_AXES; j++) {
      os << i << "," << j << " = " << left << setw(MAX_NUM_LENGTH) << M[i][j];
    }
    os << "\n";
  }
  return os;
};

Matrix Matrix::rotate(int axis, double angle) {
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

Matrix Matrix::shift(Vector &pos) {
  Matrix res;
  res.vectors[0] = Vector(1, 0, 0, 0);
  res.vectors[1] = Vector(0, 1, 0, 0);
  res.vectors[2] = Vector(0, 0, 1, 0);
  res.vectors[3] = pos;
  return res;
}
// =========================== Pixel ===========================

int &Pixel::operator[](int index) {
  switch (index) {
  case X_AXIS:
    return x;
  case Y_AXIS:
    return y;
  case TEXTURE_X:
    return texture_x;
  case TEXTURE_Y:
    return texture_y;
  default:
    cout << "Invalid index: " << index;
    return x;
  }
}

Pixel operator+(Pixel &p1, Pixel &p2) {
  Pixel res;
  res.x = p1.x + p2.x;
  res.y = p1.y + p2.y;
  res.source = p1.source + p2.source;
  res.texture_x = p1.texture_x + p2.texture_x;
  res.texture_y = p1.texture_y + p2.texture_y;
  return res;
}

Pixel operator-(Pixel &p1, Pixel &p2) {
  Pixel res;
  res.x = p1.x - p2.x;
  res.y = p1.y - p2.y;
  res.source = p1.source - p2.source;
  res.texture_x = p1.texture_x - p2.texture_x;
  res.texture_y = p1.texture_y - p2.texture_y;
  return res;
}

Pixel operator*(Pixel &p, double a) {
  Pixel res;
  res.x = p.x * a;
  res.y = p.y * a;
  res.source = p.source * a;
  res.texture_x = p.texture_x * a;
  res.texture_y = p.texture_y * a;
  return res;
}

ostream &operator<<(ostream &os, Pixel &p) {
  os << "x = " << p.x << ", y = " << p.y << ", source = " << p.source << '\n';
  return os;
}

void get_extremum(Pixel *pixels, int len, int *extremum, int lower_bound,
                  int upper_bound) {
  extremum[0] = INT_MAX;
  extremum[1] = INT_MIN;
  Pixel *p;
  for (int i = 0; i < len; i++) {
    p = &pixels[i];
    extremum[0] = max(min(extremum[0], p->y), lower_bound);
    extremum[1] = min(max(extremum[1], p->y), upper_bound);
  }
}
