#include "linlib.h"

using namespace std;

// =========================== Vector ===========================

float &Vector::operator[](int index) {
  switch (index) {
  case X:
    return x;
  case Y:
    return y;
  case Z:
    return z;
  case W:
    return w;
  default:
    cout << "Invalid index: " << index;
    return x;
  }
}

Vector operator+(Vector &v1, Vector &v2) {
  Vector res;
  for (int axis = 0; axis < AXES; axis++) {
    res[axis] = v1[axis] + v2[axis];
  }
  return res;
}

Vector operator-(Vector &v1, Vector &v2) {
  Vector res;
  for (int axis = 0; axis < AXES; axis++) {
    res[axis] = v1[axis] - v2[axis];
  }
  return res;
}

Vector operator*(Vector &v1, float a) {
  Vector res;
  for (int axis = 0; axis < AXES; axis++) {
    res[axis] = v1[axis] * a;
  }
  return res;
}

float operator*(Vector &v1, Vector &v2) {
  float sum = 0.0f;
  for (int axis = 0; axis < AXES; axis++) {
    sum += v1[axis] * v2[axis];
  }
  return sum;
}

ostream &operator<<(ostream &os, Vector &v) {
  os << "(x = " << v.x << ", y = " << v.y << ", z = " << v.z << ", w = " << v.w
     << ")\n";
  return os;
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

Matrix operator*(Matrix &M, float a) {
  Matrix res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    res[i] = M[i] * a;
  }
  return res;
}

Vector operator*(Matrix &M, Vector &v) {
  Vector res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    float sum = 0.0f;
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
    float sum = 0.0f;
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
      float sum = 0;
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

Matrix Matrix::rotate(int axis, float angle) {
  Matrix res;
  res.vectors[3] = Vector(0, 0, 0, 1);
  switch (axis) {
  case X:
    res.vectors[0] = Vector(1, 0, 0, 0);
    res.vectors[1] = Vector(0, cos(angle), -sin(angle), 0);
    res.vectors[2] = Vector(0, sin(angle), cos(angle), 0);
    return res;
  case Y:
    res.vectors[0] = Vector(cos(angle), 0, sin(angle), 0);
    res.vectors[1] = Vector(0, 1, 0, 0);
    res.vectors[2] = Vector(-sin(angle), 0, cos(angle), 0);
    return res;
  case Z:
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
  case X:
    return x;
  case Y:
    return y;
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

Pixel operator*(Pixel &p, float a) {
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

Pixel interpolate(Pixel &p1, Pixel &p2, float ratio) {
  Pixel frac_1 = p1 * ratio;
  Pixel frac_2 = p2 * (1 - ratio);
  return frac_1 + frac_2;
}

void get_extremum(Pixel *pixels, int len, int *extremum, int lower_bound,
                  int upper_bound) {
  Pixel *p;
  for (int i = 0; i < len; i++) {
    p = &pixels[i];
    extremum[0] = max(min(extremum[0], p->y), lower_bound);
    extremum[1] = min(max(extremum[1], p->y), upper_bound);
  }
}
